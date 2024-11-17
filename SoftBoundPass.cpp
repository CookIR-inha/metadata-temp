// SoftBoundPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <map>

using namespace llvm;

// using softbound's shadow space method
/* Book-keeping structures for identifying original instructions in
 * the program, pointers and their corresponding base and bound
 */

namespace
{
  struct SoftBoundPass : public PassInfoMixin<SoftBoundPass>
  {
    static char ID;
    struct Metadata
    {
      Value *Base;
      Value *Bound;
    };
    std::map<Value *, Value *> MValueBaseMap;
    std::map<Value *, Value *> MValueBoundMap;

    LLVMContext *C;
    const DataLayout *DL;
    PointerType *MVoidPtrTy;
    FunctionCallee setMetaData;
    FunctionCallee printMetadata;
    FunctionCallee getMetaData;
    FunctionCallee boundCheck;
    FunctionCallee printMetadataTable;
    FunctionCallee getBaseAddr;
    FunctionCallee getBoundAddr;
    FunctionCallee initTable;

    // For constants containing multiple pointers use getAssociatedBaseArray.
    Value *getAssociatedBase(Value *pointer_operand)
    {
      errs() << "get assoc : " << pointer_operand << "\n";
      if (!MValueBaseMap.count(pointer_operand))
      {
        errs() << "no element\n";
        return 0;
      }
      return MValueBaseMap[pointer_operand];
    }

    Value *getAssociatedBound(Value *pointer_operand)
    {
      // TODO: 구조체 내부 out-of-bound 탐지 구현
      if (MValueBoundMap.find(pointer_operand) == MValueBoundMap.end())
      {
        errs() << "getAssociatedBound: No bound found for " << *pointer_operand << "\n";
        return nullptr;
      }
      return MValueBoundMap[pointer_operand];
    }

    void dissociateBaseBound(Value *pointer_operand)
    {
      if (MValueBaseMap.count(pointer_operand))
      {
        MValueBaseMap.erase(pointer_operand);
      }
      if (MValueBoundMap.count(pointer_operand))
      {
        MValueBoundMap.erase(pointer_operand);
      }
      assert((MValueBaseMap.count(pointer_operand) == 0) &&
             "dissociating base failed\n");
      assert((MValueBoundMap.count(pointer_operand) == 0) &&
             "dissociating bound failed");
    }
    inline void associateBaseBound(Value *Val, Value *Base,
                                   Value *Bound)
    {
      if (MValueBaseMap.count(Val))
      {
        errs() << "disassociate\n";
        dissociateBaseBound(Val);
      }

      MValueBaseMap[Val] = Base;
      MValueBoundMap[Val] = Bound;
    }

    bool isTypeWithPointers(Type *Ty)
    {
      switch (Ty->getTypeID())
      {
      case Type::PointerTyID:
        return true;
      case Type::StructTyID:
      {
        for (Type::subtype_iterator I = Ty->subtype_begin(), E = Ty->subtype_end();
             I != E; ++I)
        {
          if (isTypeWithPointers(*I))
            return true;
        }
        return false;
      }
      case Type::ArrayTyID:
      {
        ArrayType *ArrayTy = cast<ArrayType>(Ty);
        return isTypeWithPointers(ArrayTy->getElementType());
      }
      case Type::FixedVectorTyID:
      {
        FixedVectorType *FVTy = cast<FixedVectorType>(Ty);
        return isTypeWithPointers(FVTy->getElementType());
      }
      case Type::ScalableVectorTyID:
      {
        assert(0 && "Counting pointers for scalable vectors not yet handled.");
      }
      default:
        return false;
      }
    }

    // using softbound getnextinstruction
    Instruction *getNextInstruction(Instruction *I)
    {
      if (I->isTerminator())
        return I;
      return I->getNextNode();
    }

    // using softbound typecasting function
    Value *castToVoidPtr(Value *Ptr, IRBuilder<> &IRB)
    {
      if (Ptr->getType() == MVoidPtrTy)
        return Ptr;

      if (Constant *C = dyn_cast<Constant>(Ptr))
        return ConstantExpr::getBitCast(C, MVoidPtrTy);

      return IRB.CreateBitCast(Ptr, MVoidPtrTy, Ptr->getName() + ".voidptr");
    }

    void handle_alloca(Instruction &I)
    {
      auto *AI = dyn_cast<AllocaInst>(&I);
      IRBuilder<> builder(AI->getNextNode());
      Type *allocTy = AI->getAllocatedType();

      Value *base = castToVoidPtr(AI, builder);
      Value *Idx;
      if (AI->getNumOperands() == 0)
      {
        // 64비트 또는 32비트 시스템에 따라 상수를 설정
        Idx = ConstantInt::get(Type::getInt64Ty(AI->getType()->getContext()), 1, false);
      }
      else
      {
        // `alloca` 명령어에 지정된 크기를 Idx로 설정
        Idx = AI->getOperand(0);
      }

      // Bound 주소 계산: GEP 명령어로 base 주소에 Idx를 더해 bound 주소 계산
      Value *boundGEP = builder.CreateGEP(AI->getAllocatedType(), AI, Idx, "mtmp");
      Value *bound = castToVoidPtr(boundGEP, builder); // Bound를 void*로 캐스팅
      associateBaseBound(AI, base, bound);
    };

    void handle_store(Instruction &I)
    {
      StoreInst *SI = dyn_cast<StoreInst>(&I);
      IRBuilder<> builder(SI->getNextNode());

      Value *src = SI->getOperand(0);
      Value *dst = SI->getOperand(1);
      Type *type = src->getType();
      Value *base = getAssociatedBase(dst);
      Value *bound = getAssociatedBound(dst);
      Value *access = castToVoidPtr(dst, builder);
      if (!isTypeWithPointers(src->getType()))
      {
        builder.CreateCall(boundCheck,{base, access});
        errs() << "returning\n";
        return;
      }
        builder.CreateCall(printMetadata, {base, access});
      

      if (isa<PointerType>(type))
      {
        builder.CreateCall(setMetaData, {src, base, bound});
        
      }

      // vector의 경우 아직 고려하지 않음
    };

    void handle_GEP(Instruction &I)
    {
      GetElementPtrInst *GEPI = dyn_cast<GetElementPtrInst>(&I);

      Value *GEPPtrOp = GEPI->getPointerOperand(); // GEP의 포인터 피연산자
      IRBuilder<> IRB(GEPI->getNextNode());

      // 기존의 포인터에 대한 메타데이터 가져오기
      Value *base = getAssociatedBase(GEPPtrOp);
      Value *bound = getAssociatedBound(GEPPtrOp);
      if (!base || !bound)
      {
        errs() << "Error: Base or Bound metadata not found for GEP!\n";
        return;
      }
      // GEP연산에서 base와 offset을 더해서 나온 access하는 주소를 구하는데, 여기서 base랑 bound를 구해서 access로 assoc 함
      associateBaseBound(GEPI, base, bound); // %ptr = base+offset, %ptr에 대한 base, bound
      Value *test_base = getAssociatedBase(GEPI);
      Value *test_bound = getAssociatedBound(GEPI);
      errs() << "GEP base : " << test_base << " bound : " << test_bound <<"\n"; 

      // 새로운 GEP 명령어에 메타데이터 연결
    };

    void handle_load(Instruction &I)
    {
      LoadInst *LI = dyn_cast<LoadInst>(&I);
      Type *LoadTy = LI->getType();
      Metadata data;

      int typeID = LoadTy->getTypeID();

      Value *pointer_operand = LI->getPointerOperand();
      Value *base = getAssociatedBase(pointer_operand);
      Value *bound = getAssociatedBound(pointer_operand);
      Instruction *new_inst = getNextInstruction(LI);
      IRBuilder<> IRB(new_inst);
      Value *access = castToVoidPtr(pointer_operand, IRB);
      IRB.CreateCall(printMetadata, {base, bound});

      if (isa<PointerType>(LoadTy))
      {
        if (!MValueBaseMap.count(pointer_operand) && !MValueBoundMap.count(pointer_operand))
        {
          errs() << "load : " << *LI << "\n";
          data.Base = IRB.CreateCall(getBaseAddr, {pointer_operand});
          data.Bound = IRB.CreateCall(getBoundAddr, {pointer_operand});
          associateBaseBound(LI, data.Base, data.Bound);
        }
      }
      IRB.CreateCall(boundCheck, {base, access});
    };

    void handle_bitcast(Instruction &I)
    {
      BitCastInst *BCI = dyn_cast<BitCastInst>(&I);
      if (!BCI)
        return;

      Value *SrcPtr = BCI->getOperand(0); // 원본 포인터
      Value *DstPtr = &I;                 // 변환된 포인터
      IRBuilder<> IRB(BCI->getNextNode());

      // 원본 포인터의 메타데이터 가져오기
      errs() << "assoc in bitcast : " << *SrcPtr << "\n";
      Value *Base = getAssociatedBase(SrcPtr);
      Value *Bound = getAssociatedBound(SrcPtr);

      if (!Base || !Bound)
      {
        errs() << "Error: Base or Bound metadata not found for bitcast!\n";
        return;
      }

      // 새로운 포인터에 메타데이터 연관
      associateBaseBound(DstPtr, Base, Bound);
    }

    static void appendToGlobalArray(const char *Array, Module &M, Function *F,
                                    int Priority, Constant *Data)
    {
      IRBuilder<> IRB(M.getContext());
      FunctionType *FnTy = FunctionType::get(IRB.getVoidTy(), false);

      // Get the current set of static global constructors and add the new ctor
      // to the list.
      SmallVector<Constant *, 16> CurrentCtors;
      StructType *EltTy = StructType::get(
          IRB.getInt32Ty(), PointerType::getUnqual(FnTy), IRB.getInt8PtrTy());
      if (GlobalVariable *GVCtor = M.getNamedGlobal(Array))
      {
        if (Constant *Init = GVCtor->getInitializer())
        {
          unsigned n = Init->getNumOperands();
          CurrentCtors.reserve(n + 1);
          for (unsigned i = 0; i != n; ++i)
            CurrentCtors.push_back(cast<Constant>(Init->getOperand(i)));
        }
        GVCtor->eraseFromParent();
      }

      // Build a 3 field global_ctor entry.  We don't take a comdat key.
      Constant *CSVals[3];
      CSVals[0] = IRB.getInt32(Priority);
      CSVals[1] = F;
      CSVals[2] = Data ? ConstantExpr::getPointerCast(Data, IRB.getInt8PtrTy())
                       : Constant::getNullValue(IRB.getInt8PtrTy());
      Constant *RuntimeCtorInit =
          ConstantStruct::get(EltTy, makeArrayRef(CSVals, EltTy->getNumElements()));

      CurrentCtors.push_back(RuntimeCtorInit);

      // Create a new initializer.
      ArrayType *AT = ArrayType::get(EltTy, CurrentCtors.size());
      Constant *NewInit = ConstantArray::get(AT, CurrentCtors);

      // Create the new global variable and replace all uses of
      // the old global variable with the new one.
      (void)new GlobalVariable(M, NewInit->getType(), false,
                               GlobalValue::AppendingLinkage, NewInit, Array);
    }

    void appendToGlobalCtors(Module &M, Function *F, int Priority, Constant *Data)
    {
      appendToGlobalArray("llvm.global_ctors", M, F, Priority, Data);
    }

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM)
    {
      MVoidPtrTy = PointerType::getInt8PtrTy(M.getContext());
      DL = &M.getDataLayout();
      C = &M.getContext();
      setMetaData = M.getOrInsertFunction(
          "set_metadata",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                                                              // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr, size_t size)
              false                                                                                                         // 가변 인자 여부: false
              ));
      printMetadata = M.getOrInsertFunction(
          "print_metadata",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                          // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* base, void* bound)
              false                                                                     // 가변 인자 여부: false
              ));
      // get_metadata 함수 선언 또는 삽입
      getMetaData = M.getOrInsertFunction(
          "get_metadata",
          FunctionType::get(
              Type::getInt1Ty(M.getContext()),      // 반환 타입: bool (int1)
              {Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr)
              false                                 // 가변 인자 여부: false
              ));

      // bound_check 함수 선언 또는 삽입
      boundCheck = M.getOrInsertFunction(
          "bound_check",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                          // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr, void* ptr)
              false                                                                     // 가변 인자 여부: false
              ));

      // print_metadata_table 함수 선언 또는 삽입
      printMetadataTable = M.getOrInsertFunction(
          "print_metadata_table",
          FunctionType::get(
              Type::getVoidTy(M.getContext()), // 반환 타입: void
              false                            // 인자 없음
              ));
      getBaseAddr = M.getOrInsertFunction(
          "get_base_addr",
          FunctionType::get(
              Type::getInt8PtrTy(M.getContext()),
              {Type::getInt8PtrTy(M.getContext())},
              false));
      getBoundAddr = M.getOrInsertFunction(
          "get_bound_addr",
          FunctionType::get(
              Type::getInt8PtrTy(M.getContext()),
              {Type::getInt8PtrTy(M.getContext())},
              false));
      initTable = M.getOrInsertFunction(
          "_init_metadata_table",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),
              false));
      FunctionCallee initTable = M.getOrInsertFunction(
          "_init_metadata_table",
          FunctionType::get(Type::getVoidTy(M.getContext()), false));

      Function *CtorFunc = Function::Create(
          FunctionType::get(Type::getVoidTy(M.getContext()), false), // 함수 타입: void()
          GlobalValue::InternalLinkage, "__global_init", &M);        // 함수 이름 및 링키지

      // 기본 블록 생성
      BasicBlock *BB = BasicBlock::Create(M.getContext(), "entry", CtorFunc);

      // IRBuilder 생성
      IRBuilder<> IRB(BB);

      // _init_metadata_table 호출 삽입
      IRB.CreateCall(initTable);
      // 함수 종료 코드 추가
      IRB.CreateRetVoid();

      // __global_init을 전역 생성자에 등록
      appendToGlobalCtors(M, CtorFunc, 0, nullptr);
      for (Function &F : M)
      {
        for (BasicBlock &BB : F)
        {
          for (Instruction &I : BB)
          {
            errs() << "About Instruction : " << I << "\n";
            IRBuilder<> IRB(&I);
            switch (I.getOpcode())
            {
            case Instruction::Alloca:
            {
              handle_alloca(I);
              break;
            }
            case Instruction::GetElementPtr:
            {
              handle_GEP(I);
              break;
            }
            case Instruction::Load:
            {
              handle_load(I);
              break;
            }
            case Instruction::Store:
            {
              handle_store(I);
              break;
            }
            case Instruction::BitCast:
              handle_bitcast(I);
            }
          }
        }
      }
      return PreservedAnalyses::none();
    };
  };
}

llvm::PassPluginLibraryInfo
getSoftBoundPassPluginInfo()
{
  return {LLVM_PLUGIN_API_VERSION, "SoftBoundPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB)
          {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>)
                {
                  if (Name == "softbound")
                  {
                    MPM.addPass(SoftBoundPass());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo()
{
  return getSoftBoundPassPluginInfo();
}