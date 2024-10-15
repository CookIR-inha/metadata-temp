// SoftBoundPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <unordered_set>

using namespace llvm;

namespace
{
  struct SoftBoundPass : public ModulePass
  {
    static char ID;
    SoftBoundPass() : ModulePass(ID) {}

    std::unordered_set<Value *> mallocPointers;

    bool runOnModule(Module &M) override
    {
      Function *setMetaData = M.getFunction("set_metadata");
      Function *printMetadataTable = M.getFunction("print_metadata_table");
      Function *boundCheck = M.getFunction("bound_check");
      Function *getMetaData = M.getFunction("get_metadata");

      if (!setMetaData || !printMetadataTable)
      {
        errs() << "Error: Couldn't find function 'set_metadata' or 'check_bounds'. Make sure hello.c is linked.\n";
        return false;
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
              Value *basePtr = gepInst->getPointerOperand();
              IRBuilder<> builder(&I);

              Value *castedPtr = builder.CreateBitCast(basePtr, Type::getInt8PtrTy(M.getContext()));

              errs() << "gep found: malloced?" << " basePtr: " << basePtr << " castedPtr: " << castedPtr << "\n";
              if (mallocPointers.find(basePtr) != mallocPointers.end())
              {
                errs() << "true\n";
              }
              else
              {
                errs() << "false\n";
              }
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

      return true;
    }
  };
}

char SoftBoundPass::ID = 0;
static RegisterPass<SoftBoundPass> X("softbound", "Insert Before malloc", false, false);