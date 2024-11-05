// SoftBoundPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "SoftBoundPass.h"
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

    std::map<Value *, TinyPtrVector<Value *>> MValueBaseMap;
    std::map<Value *, TinyPtrVector<Value *>> MValueBoundMap;

    PointerType *MVoidPtrTy;
    FunctionCallee setMetaData;
    FunctionCallee printMetadata;
    FunctionCallee getMetaData;
    FunctionCallee boundCheck;
    FunctionCallee printMetadataTable;

    inline void associateAggregateBase(Value *Val,
                                              TinyPtrVector<Value *> &Bases) {
      auto NumMetadata = countMetadata(Val->getType());
      assert(NumMetadata == Bases.size() &&
            "Bases size is not equal to number of metadata in type");

      for (auto &Base : Bases) {
        assert(isValidMetadata(Base, MetadataType::Base) &&
              "Invalid base metadata");
      }
      MValueBaseMap[Val] = Bases;
    }

    ArrayRef<Value *> getAssociatedBases(Value *Val) {
      if (!MValueBaseMap.count(Val)) {
        if (auto *Const = dyn_cast<Constant>(Val)) {
          auto Bases = createConstantBases<Value>(Const);
          associateAggregateBase(Val, Bases);
        }
      }
      return MValueBaseMap[Val];
    }

    // For constants containing multiple pointers use getAssociatedBaseArray.
    Value *getAssociatedBase(Value *pointer_operand) {
      ArrayRef<Value *> base_array = getAssociatedBases(pointer_operand);
      assert(base_array.size() == 1 && "getAssociatedBase called on aggregate");
      Value *pointer_base = base_array[0];

      return pointer_base;
    }

    Value *getAssociatedBound(Value *pointer_operand) {
      ArrayRef<Value *> bound_array = getAssociatedBounds(pointer_operand);
      assert(bound_array.size() == 1 && "getAssociatedBound called on aggregate");
      Value *pointer_bound = bound_array[0];

      return pointer_bound;
    }

    void dissociateBaseBound(Value *pointer_operand) {
      if (MValueBaseMap.count(pointer_operand)) {
        MValueBaseMap.erase(pointer_operand);
      }
      if (MValueBoundMap.count(pointer_operand)) {
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
        dissociateBaseBound(Val);
      }

      MValueBaseMap[Val] = {Base};
      MValueBoundMap[Val] = {Bound};
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

    void handle_alloca(Instruction &I, FunctionCallee printMetadata)
    {
      auto *AI = dyn_cast<AllocaInst>(&I);
      IRBuilder<> builder(AI->getNextNode());

      Value *base = castToVoidPtr(AI, builder);

      Value *index = AI->getOperand(0);
      Value *bound = builder.CreateGEP(AI->getAllocatedType(), AI, index, "mtmp");

      bound = castToVoidPtr(bound, builder);

      associateBaseBound(AI, base, bound);

      builder.CreateCall(printMetadata, {base, bound});
    };

    void handle_store(Instruction &I)
    {
      StoreInst *SI = dyn_cast<StoreInst>(&I);
      Type *type = SI->getType();

      Value *src = SI->getOperand(0);
      Value *dst = SI->getOperand(1);

      Instruction *insertPoint = getNextInstruction(SI);
      if (!isTypeWithPointers(type))
      {
        return;
      }
      else if (isa<PointerType>(SI))
      {
        Value *base = getAssociatedBase(src);
        Value *bound = getAssociatedBound(src);
      }

      // vector의 경우 아직 고려하지 않음
    };

    void metadata_GEP(Instruction &I, FunctionCallee printMetadata) {

    };

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM)
    {
      MVoidPtrTy = PointerType::getInt8PtrTy(M.getContext());

      setMetaData = M.getOrInsertFunction(
          "set_metadata",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                        // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt64Ty(M.getContext())}, // 인자: (void* ptr, size_t size)
              false                                                                   // 가변 인자 여부: false
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

      for (Function &F : M)
      {
        for (BasicBlock &BB : F)
        {
          for (Instruction &I : BB)
          {
            AllocaInst *AI = dyn_cast<AllocaInst>(&I);

            // 1. 사용자(user) 분석을 통한 필터링
            bool isVariableAllocation = false;

            for (User *U : AI->users())
            {
              if (isa<StoreInst>(U) || isa<LoadInst>(U) || isa<GetElementPtrInst>(U))
              {
                isVariableAllocation = true;
                break; // 변수로 사용됨을 확인한 경우
              }
            }
            errs() << "Instrumenting base/bound for: " << *AI << "\n";
            // 메타데이터 관리 작업 수행

            switch (I.getOpcode())
            {
            case Instruction::Alloca:
              handle_alloca(I, printMetadata);
            case Instruction::GetElementPtr:
              metadata_GEP(I, printMetadata);
            }
          }
        }
      }
      return PreservedAnalyses::none();
    }
  };
}

llvm::PassPluginLibraryInfo getSoftBoundPassPluginInfo()
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