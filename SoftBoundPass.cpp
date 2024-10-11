// SoftBoundPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

namespace
{
  struct SoftBoundPass : public ModulePass
  {
    static char ID;
    SoftBoundPass() : ModulePass(ID) {}

    bool runOnModule(Module &M) override
    {
      Function *setMetaData = M.getFunction("set_metadata");
      Function *printMetadataTable = M.getFunction("print_metadata_table");

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

                builder.CreateCall(setMetaData, {mallocPtr, mallocSize});
              }
            }
          }
        }
      }
      Function *mainFunc = M.getFunction("main");
      if (mainFunc)
      {
        // main 함수의 마지막 BasicBlock에 삽입
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