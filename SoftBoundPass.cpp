// SoftBoundPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
<<<<<<< HEAD
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include <unordered_set>
#include <unordered_map>
=======
#include <map>
>>>>>>> simple-detect

using namespace llvm;

// using softbound's shadow space method
/* Book-keeping structures for identifying original instructions in
 * the program, pointers and their corresponding base and bound
 */

namespace
{
  struct SoftBoundPass : public PassInfoMixin<SoftBoundPass>
  {
<<<<<<< HEAD
    std::unordered_set<Value *> mallocPointers;            // pi2
    std::unordered_map<Value *, Value *> pointerLocations; // pi_offset : pi2
    std::unordered_map<Value *, Value *> derivedPointers;  // GEP로 생성된 파생 포인터 : 원본 포인터

    void trackPointer(Value *ptr, Value *location = nullptr)
    {
      if (!mallocPointers.count(ptr))
      {
        mallocPointers.insert(ptr);
        errs() << "Tracking pointer: " << *ptr << "\n";
        printPointerStructures();
      }
      if (location)
      {
        pointerLocations[location] = ptr; // 포인터가 저장된 위치도 추적
        errs() << "Tracking pointer storage location: " << *location << " -> " << *ptr << "\n";
        printPointerStructures();
      }
    }

    void printPointerStructures()
    {
      errs() << "===== Current Pointer Structures =====\n";

      // mallocPointers에 저장된 원본 포인터 출력
      errs() << "Malloc Pointers:\n";
      for (auto *ptr : mallocPointers)
      {
        errs() << "  " << *ptr << "\n";
      }

      // pointerLocations에 저장된 파생 포인터 출력
      errs() << "Pointer Locations:\n";
      for (auto &entry : pointerLocations)
      {
        errs() << "  Location: " << *entry.first << " -> Points to: " << *entry.second << "\n";
      }

      errs() << "======================================\n";
    }

    bool isMallocRelatedPointer(Value *ptr, Value *&originalPtr)
    {
      if (mallocPointers.count(ptr) > 0)
      {
        originalPtr = ptr; // ptr 자체가 malloc된 포인터인 경우
        return true;
      }
      if (pointerLocations.count(ptr) > 0)
      {
        Value *trackedPtr = pointerLocations[ptr];
        originalPtr = pointerLocations[ptr]; // 파생된 포인터인 경우 원래 포인터 찾기
        return mallocPointers.count(trackedPtr) > 0;
      }
      return false;
    }

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &)
    {
      Function *setMetaData = M.getFunction("set_metadata");
      Function *printMetadataTable = M.getFunction("print_metadata_table");
      Function *boundCheck = M.getFunction("bound_check");
      Function *getMetaData = M.getFunction("get_metadata");

      if (!setMetaData || !printMetadataTable || !boundCheck || !getMetaData)
      {
        errs() << "Error: Couldn't find function\n";
        return PreservedAnalyses::all();
      }
=======
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
      if (!MValueBaseMap.count(pointer_operand))
      {
        errs() << "getAssociatedBase: No base found for " << *pointer_operand << "\n";
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
      Value *base = NULL;
      Value *bound = NULL;
      Value *access = castToVoidPtr(dst, builder);
      if (!isTypeWithPointers(src->getType()))
      {
        base = getAssociatedBase(dst);
        bound = getAssociatedBound(dst);
        builder.CreateCall(boundCheck,{base, bound, access});
        return;
      }
      base = getAssociatedBase(src);
      bound = getAssociatedBound(src);
      if (isa<PointerType>(type))
      {
        builder.CreateCall(setMetaData, {access, base, bound});
        
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

      if (isa<PointerType>(LoadTy))
      {
        if (!MValueBaseMap.count(LI) && !MValueBoundMap.count(LI))
        {
          
          Value *loadsrc = castToVoidPtr(pointer_operand, IRB);
          data.Base = IRB.CreateCall(getBaseAddr, {loadsrc});
          data.Bound = IRB.CreateCall(getBoundAddr, {loadsrc});
          associateBaseBound(LI, data.Base, data.Bound);
        }
      }
      if(!base || !bound){
        errs() << *LI << "\n";
      }
      IRB.CreateCall(printMetadata, {base,bound});
      IRB.CreateCall(boundCheck, {base, bound, access});
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

      // bound_check 함수 선언 또는 삽입
      boundCheck = M.getOrInsertFunction(
          "bound_check",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                          // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext()),Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr, void* ptr)
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
>>>>>>> simple-detect

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
        if (F.getName() == "main")
        {
          Instruction &firstInst = *F.getEntryBlock().getFirstInsertionPt();
          IRBuilder<> builder(&firstInst);

          Function *initFunc = M.getFunction("initialize_metadata_table");
          if (!initFunc)
          {
            FunctionType *initFuncType = FunctionType::get(Type::getVoidTy(F.getContext()), false);
            initFunc = Function::Create(initFuncType, Function::ExternalLinkage, "initialize_metadata_table", M);
          }

          builder.CreateCall(initFunc);
          errs() << "Inserted initialize_metadata_table at the beginning of main func\n";
        }
        for (BasicBlock &BB : F)
        {
          for (Instruction &I : BB)
          {
            IRBuilder<> IRB(&I);
            errs() << "handling instruction : " << &I << "\n";
            switch (I.getOpcode())
            {
<<<<<<< HEAD
              Function *calledFunc = callInst->getCalledFunction();

              if (calledFunc && calledFunc->getName() == "malloc")
              {
                IRBuilder<> builder(callInst->getNextNode());

                Value *mallocPtr = callInst;
                Value *mallocSize = callInst->getArgOperand(0);

                errs() << "\nMalloc found: setMetaData at " << *mallocPtr << "\n";
                trackPointer(callInst);

                builder.CreateCall(setMetaData, {mallocPtr, mallocSize});
              }
            }
            if (auto *gepInst = dyn_cast<GetElementPtrInst>(&I))
            {
              Value *basePtr = gepInst->getPointerOperand();
              Value *originalPtr = nullptr;

              if (isMallocRelatedPointer(basePtr, originalPtr))
              {
                // GEP 연산 전 포인터와 연산 후 파생 포인터 모두 저장
                pointerLocations[basePtr] = originalPtr; // 연산 전 포인터를 기준으로 원본 포인터와 연관
                pointerLocations[gepInst] = originalPtr; // 연산 후 파생된 포인터를 원본 포인터와 연관

                errs() << "Tracking GEP-derived pointer:\n"
                       << "  Original base pointer: " << *basePtr << "\n"
                       << "  Derived pointer (result of GEP): " << *gepInst << "\n"
                       << "  Associated malloc-related pointer: " << *originalPtr << "\n";
              }
            }

            if (auto *bitcastInst = dyn_cast<BitCastInst>(&I))
            {
              {
                Value *basePtr = bitcastInst->getOperand(0);
                Value *originalPtr = nullptr;

                // basePtr이 malloc과 관련된 포인터인지 확인
                if (isMallocRelatedPointer(basePtr, originalPtr))
                {
                  // BitCast 연산의 결과 포인터를 원본 포인터와 연관 지어 추적
                  trackPointer(bitcastInst, originalPtr);
                  errs() << "Tracking BitCast-derived pointer: " << *bitcastInst << " from base pointer: " << *basePtr << "\n";
                }
              }
            }
            if (auto *loadInst = dyn_cast<LoadInst>(&I))
            {
              Value *basePtr = loadInst->getPointerOperand(); // 값을 읽어오는 위치
              Value *pointerOperand = loadInst->getPointerOperand();
              Value *originalPtr = nullptr; // 원래의 malloc된 포인터를 저장할 변수

              if (isMallocRelatedPointer(basePtr, originalPtr))
              {
                IRBuilder<> builder(loadInst);

                // Load 결과 값을 원본 malloc 포인터와 연관 지음
                pointerLocations[loadInst] = originalPtr;

                // basePtr이 가리키는 값을 로드하여 access로 사용
                Value *accessPtr = builder.CreateLoad(basePtr->getType()->getPointerElementType(), basePtr);

                Value *castedPtr = builder.CreateBitCast(originalPtr, Type::getInt8PtrTy(M.getContext()));
                Value *castedAccess = builder.CreateBitCast(accessPtr, Type::getInt8PtrTy(M.getContext()));

                errs() << "Load found for malloced-related pointer: " << *basePtr << "\n";
                Value *instStr = builder.CreateGlobalStringPtr("Load");
                // builder.CreateCall(boundCheck, {castedPtr, castedAccess, instStr}); // Bound Check 호출
                // errs() << "Inserted bound_check (Load)\n";
              }
              else
              {
                // errs() << "Load found for non-malloced pointer: " << *basePtr << "\n";
              }
            }

            if (auto *storeInst = dyn_cast<StoreInst>(&I))
            {
              Value *storedValue = storeInst->getValueOperand();       // 저장되는 값
              Value *storageLocation = storeInst->getPointerOperand(); // 저장되는 위치
              Value *originalPtr = nullptr;                            // 원래의 malloc된 포인터를 저장할 변수

              if (storedValue->getType()->isPointerTy() && isMallocRelatedPointer(storedValue, originalPtr))
              {
                IRBuilder<> builder(storeInst);

                Value *castedPtr = builder.CreateBitCast(originalPtr, Type::getInt8PtrTy(M.getContext()));
                Value *castedAccess = builder.CreateBitCast(storageLocation, Type::getInt8PtrTy(M.getContext()));

                errs() << "Store found for malloc-related pointer stored in storageLocation: " << *storedValue << "\n";
                Value *instStr = builder.CreateGlobalStringPtr("Store");
                // builder.CreateCall(boundCheck, {castedPtr, castedAccess, instStr}); // Bound Check 호출
                errs() << "Inserted bound_check (Store) on storageLocation\n";

                trackPointer(storedValue, storageLocation); // 저장 위치도 추적
              }
              // Case 2: storedValue가 포인터가 아닌 경우, storageLocation이 malloc 관련 메모리일 때
              else if (isMallocRelatedPointer(storageLocation, originalPtr))
              {
                IRBuilder<> builder(storeInst);

                Value *castedPtr = builder.CreateBitCast(originalPtr, Type::getInt8PtrTy(M.getContext()));
                Value *castedAccess = builder.CreateBitCast(storageLocation, Type::getInt8PtrTy(M.getContext()));

                errs() << "Store found for non-pointer value stored in malloc-related location: " << *storageLocation << "\n";
                Value *instStr = builder.CreateGlobalStringPtr("Store");
                builder.CreateCall(boundCheck, {castedPtr, castedAccess, instStr}); // Bound Check 호출
                errs() << "Inserted bound_check (Store) for non-pointer stored in malloc-related location\n";
              }
              else
              {
                // errs() << "Store found for non-malloced pointer or non-pointer value: " << *storedValue << "\n";
              }
=======
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
>>>>>>> simple-detect
            }
          }
        }
      }
<<<<<<< HEAD

      Function *mainFunc = M.getFunction("main");
      if (mainFunc)
      {
        for (BasicBlock &BB : *mainFunc)
        {
          if (ReturnInst *retInst = dyn_cast<ReturnInst>(BB.getTerminator()))
          {
            IRBuilder<> builder(retInst);
            builder.CreateCall(printMetadataTable);
            errs() << "Inserted print_metadata_table at the end of main func\n";
          }
        }
      }

      return PreservedAnalyses::all();
    }
=======
      return PreservedAnalyses::none();
    };
>>>>>>> simple-detect
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
<<<<<<< HEAD
                  if (Name == "softbound-pass")
=======
                  if (Name == "softbound")
>>>>>>> simple-detect
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