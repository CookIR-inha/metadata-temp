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