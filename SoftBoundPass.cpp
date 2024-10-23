// SoftBoundPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <unordered_set>

using namespace llvm;

namespace
{
  struct SoftBoundPass : public PassInfoMixin<SoftBoundPass>
  {
    static char ID;

    std::unordered_set<Value *> mallocPointers;

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
      FunctionCallee setMetaData = M.getOrInsertFunction(
      "set_metadata",
          FunctionType::get(
              Type::getVoidTy(M.getContext()), // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt64Ty(M.getContext())}, // 인자: (void* ptr, size_t size)
              false // 가변 인자 여부: false
          )
      );

      // get_metadata 함수 선언 또는 삽입
      FunctionCallee getMetaData = M.getOrInsertFunction(
          "get_metadata",
          FunctionType::get(
              Type::getInt1Ty(M.getContext()), // 반환 타입: bool (int1)
              {Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr)
              false // 가변 인자 여부: false
          )
      );

      // bound_check 함수 선언 또는 삽입
      FunctionCallee boundCheck = M.getOrInsertFunction(
          "bound_check",
          FunctionType::get(
              Type::getVoidTy(M.getContext()), // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr, void* ptr)
              false // 가변 인자 여부: false
          )
      );

      // print_metadata_table 함수 선언 또는 삽입
      FunctionCallee printMetadataTable = M.getOrInsertFunction(
          "print_metadata_table",
          FunctionType::get(
              Type::getVoidTy(M.getContext()), // 반환 타입: void
              false // 인자 없음
          )
      );
      if (!setMetaData || !printMetadataTable)
      {
        errs() << "Error: Couldn't find function 'set_metadata' or 'check_bounds'. Make sure hello.c is linked.\n";
        return PreservedAnalyses::none();
      }

      for (Function &F : M)
      {
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

                mallocPointers.insert(mallocPtr);

                errs() << "\nMalloc found: setMetaData at " << *mallocPtr << "\n\n";

                builder.CreateCall(setMetaData, {mallocPtr, mallocSize});
              }
            }
            if (auto *loadInst = dyn_cast<LoadInst>(&I))
            {
              Value *basePtr = loadInst->getPointerOperand();

              IRBuilder<> builder(loadInst);
              Value *castedPtr = builder.CreateBitCast(basePtr, Type::getInt8PtrTy(M.getContext()));

              errs() << "load found: malloced?" << " basePtr: " << basePtr << " castedPtr: " << castedPtr << "\n";
              if (mallocPointers.find(basePtr) != mallocPointers.end())
              {
                errs() << "true\n";
              }
              else
              {
                errs() << "false\n";
              }
            }
            if (auto *storeInst = dyn_cast<StoreInst>(&I))
            {
              Value *basePtr = storeInst->getPointerOperand();

              IRBuilder<> builder(storeInst);
              Value *castedPtr = builder.CreateBitCast(basePtr, Type::getInt8PtrTy(M.getContext()));

              errs() << "store found: malloced?" << " basePtr: " << basePtr << " castedPtr: " << castedPtr << "\n";
              if (mallocPointers.find(basePtr) != mallocPointers.end())
              {
                errs() << "true\n";
              }
              else
              {
                errs() << "false\n";
              }
            }
            if (auto *gepInst = dyn_cast<GetElementPtrInst>(&I))
            {
                IRBuilder<> builder(gepInst->getNextNode()); // Insert after GEP
                Value *basePtr = gepInst->getPointerOperand();
                Value *calculatedAddress = gepInst; // Use the GEP result directly

                // Cast pointers to i8* if required by boundCheck
                Value *basePtrCasted = builder.CreateBitCast(basePtr, Type::getInt8PtrTy(M.getContext()));
                Value *calculatedAddressCasted = builder.CreateBitCast(calculatedAddress, Type::getInt8PtrTy(M.getContext()));

                // Call the boundCheck function
                builder.CreateCall(boundCheck, {basePtrCasted, calculatedAddressCasted});
              
              /*
              for (auto *user : gepInst->users())
              {
                if (auto *loadInst = dyn_cast<LoadInst>(user))
                {
                  errs() << "load found\n";
                  IRBuilder<> builder(loadInst);

                  Value *ptr = loadInst->getPointerOperand();
                  Value *castedPtr = builder.CreateBitCast(ptr, Type::getInt8PtrTy(M.getContext()));
                  builder.CreateCall(boundCheck, {castedPtr});
                }
                if (auto *storeInst = dyn_cast<StoreInst>(user))
                {
                  errs() << "store found\n";
                  IRBuilder<> builder(storeInst);

                  Value *ptr = storeInst->getPointerOperand();
                  Value *castedPtr = builder.CreateBitCast(ptr, Type::getInt8PtrTy(M.getContext()));
                  builder.CreateCall(boundCheck, {castedPtr});
                }
              }*/
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
          }
        }
      }

      return PreservedAnalyses::none();
    }
  };
}

llvm::PassPluginLibraryInfo getSoftBoundPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "SoftBoundPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "softbound") {
                    MPM.addPass(SoftBoundPass());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getSoftBoundPassPluginInfo();
}