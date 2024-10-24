// SoftBoundPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include <unordered_set>
#include <unordered_map>

using namespace llvm;

namespace
{
  struct SoftBoundPass : public PassInfoMixin<SoftBoundPass>
  {
    std::unordered_set<Value *> mallocPointers;
    std::unordered_map<Value *, Value *> pointerLocations;

    void trackPointer(Value *ptr, Value *location = nullptr)
    {
      if (!mallocPointers.count(ptr))
      {
        mallocPointers.insert(ptr);
        errs() << "Tracking pointer: " << *ptr << "\n";
      }
      if (location)
      {
        pointerLocations[location] = ptr; // 포인터가 저장된 위치도 추적
        errs() << "Tracking pointer storage location: " << *location << " -> " << *ptr << "\n";
      }
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
            if (auto *callInst = dyn_cast<CallInst>(&I))
            {
              Function *calledFunc = callInst->getCalledFunction();

              if (calledFunc && calledFunc->getName() == "malloc")
              {
                IRBuilder<> builder(callInst->getNextNode());

                Value *mallocPtr = callInst;
                Value *mallocSize = callInst->getArgOperand(0);

                trackPointer(callInst);

                errs() << "\nMalloc found: setMetaData at " << *mallocPtr << "\n";

                builder.CreateCall(setMetaData, {mallocPtr, mallocSize});
              }
            }
            if (auto *gepInst = dyn_cast<GetElementPtrInst>(&I))
            {
              Value *basePtr = gepInst->getPointerOperand();
              if (isMallocRelatedPointer(basePtr, basePtr))
              {
                trackPointer(gepInst); // gep의 결과 포인터도 추적
                errs() << "GEP: " << *gepInst << "\n";
                errs() << "Inserted bound_check (GEP)\n";
              }
            }
            if (auto *bitcastInst = dyn_cast<BitCastInst>(&I))
            {
              Value *basePtr = bitcastInst->getOperand(0);
              if (isMallocRelatedPointer(basePtr, basePtr))
              {
                trackPointer(bitcastInst); // bitcast의 결과 포인터도 추적
                errs() << "BITCAST: " << *bitcastInst << "\n";

                IRBuilder<> builder(bitcastInst->getNextNode());
                errs() << "Inserted bound_check (BitCast)\n";
              }
            }
            if (auto *loadInst = dyn_cast<LoadInst>(&I))
            {
              Value *basePtr = loadInst->getPointerOperand();
              Value *originalPtr = nullptr; // 원래의 malloc된 포인터를 저장할 변수

              if (isMallocRelatedPointer(basePtr, originalPtr))
              {
                IRBuilder<> builder(loadInst);
                Value *castedPtr = builder.CreateBitCast(originalPtr, Type::getInt8PtrTy(M.getContext()));
                Value *castedAccess = builder.CreateBitCast(basePtr, Type::getInt8PtrTy(M.getContext()));

                errs() << "Load found for malloced-related pointer: " << *basePtr << "\n";
                builder.CreateCall(boundCheck, {castedPtr, castedAccess}); // Bound Check 호출
                errs() << "Inserted bound_check (Load)\n";
                // malloc 포인터 목록 출력
                errs() << "Current mallocPointers contents (Load):\n";
                for (auto *ptr : mallocPointers)
                {
                  errs() << *ptr << "\n";
                }
              }
              else
              {
                errs() << "Load found for non-malloced pointer: " << *basePtr << "\n";
              }
            }

            if (auto *storeInst = dyn_cast<StoreInst>(&I))
            {
              Value *storedValue = storeInst->getValueOperand();       // 저장되는 값
              Value *storageLocation = storeInst->getPointerOperand(); // 저장되는 위치
              Value *originalPtr = nullptr;                            // 원래의 malloc된 포인터를 저장할 변수

              if (isMallocRelatedPointer(storedValue, originalPtr))
              {
                IRBuilder<> builder(storeInst);
                Value *castedPtr = builder.CreateBitCast(originalPtr, Type::getInt8PtrTy(M.getContext()));
                Value *castedAccess = builder.CreateBitCast(storageLocation, Type::getInt8PtrTy(M.getContext()));

                trackPointer(storedValue, storageLocation); // 저장 위치도 추적
                errs() << "Store found for malloc-related pointer: " << *storedValue << "\n";
                builder.CreateCall(boundCheck, {castedPtr, castedAccess}); // Bound Check 호출
                errs() << "Inserted bound_check (Store)\n";
                // malloc 포인터 목록 출력
                errs() << "Current mallocPointers contents (Store):\n";
                for (auto *ptr : mallocPointers)
                {
                  errs() << *ptr << "\n";
                }
              }
              else
              {
                errs() << "Store found for non-malloced pointer: " << *storedValue << "\n";
              }
            }
          }
        }
      }

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
                  if (Name == "softbound-pass")
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