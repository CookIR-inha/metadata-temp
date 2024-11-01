// SoftBoundPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

namespace
{
  struct SoftBoundPass : public PassInfoMixin<SoftBoundPass>
  {
    static char ID;
    void metadata_Alloca(Instruction &I, FunctionCallee printMetadata)
    {
      auto *allocaInst = dyn_cast<AllocaInst>(&I);
      IRBuilder<> builder(allocaInst->getNextNode());
      Value *base = builder.CreateBitCast(allocaInst, Type::getInt8PtrTy(allocaInst->getContext()));

      // Compute the size of the allocated type
      uint64_t typeSize = I.getModule()->getDataLayout().getTypeAllocSize(allocaInst->getAllocatedType());

      // Get the array size (number of elements)
      Value *arraySize = allocaInst->getArraySize();

      // Handle both constant and variable array sizes
      Value *typeSizeValue = ConstantInt::get(arraySize->getType(), typeSize);
      Value *endPtr;
      // Check if arraySize is a constant integer
      if (ConstantInt *constArraySize = dyn_cast<ConstantInt>(arraySize))
      {
        uint64_t numElements = constArraySize->getZExtValue();
        uint64_t totalSize = typeSize * numElements;

        // Create a constant for the total size
        Value *sizeValue = ConstantInt::get(Type::getInt64Ty(I.getContext()), totalSize);

        // Compute the end address (base + totalSize)
        endPtr = builder.CreateGEP(Type::getInt8Ty(I.getContext()), base, sizeValue);

        // Call printMetadata with the end address
      }
      else
      {
        // arraySize is variable; compute total size at runtime

        // Multiply arraySize * typeSize to get total size
        Value *totalSize = builder.CreateMul(arraySize, typeSizeValue);

        // Ensure totalSize is of type i64
        totalSize = builder.CreateZExtOrTrunc(totalSize, Type::getInt64Ty(I.getContext()));

        // Compute the end address (base + totalSize)
        endPtr = builder.CreateGEP(Type::getInt8Ty(I.getContext()), base, totalSize);

        // Call printMetadata with the end address
      }
      builder.CreateCall(printMetadata, {base, endPtr});
    };
    void metadata_GEP(Instruction &I, FunctionCallee printMetadata) {
      
    };

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM)
    {
      FunctionCallee setMetaData = M.getOrInsertFunction(
          "set_metadata",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                        // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt64Ty(M.getContext())}, // 인자: (void* ptr, size_t size)
              false                                                                   // 가변 인자 여부: false
              ));
      FunctionCallee printMetadata = M.getOrInsertFunction(
          "print_metadata",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                          // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* base, void* bound)
              false                                                                     // 가변 인자 여부: false
              ));
      // get_metadata 함수 선언 또는 삽입
      FunctionCallee getMetaData = M.getOrInsertFunction(
          "get_metadata",
          FunctionType::get(
              Type::getInt1Ty(M.getContext()),      // 반환 타입: bool (int1)
              {Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr)
              false                                 // 가변 인자 여부: false
              ));

      // bound_check 함수 선언 또는 삽입
      FunctionCallee boundCheck = M.getOrInsertFunction(
          "bound_check",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                          // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr, void* ptr)
              false                                                                     // 가변 인자 여부: false
              ));

      // print_metadata_table 함수 선언 또는 삽입
      FunctionCallee printMetadataTable = M.getOrInsertFunction(
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
            switch (I.getOpcode())
            {
            case Instruction::Alloca:
              metadata_Alloca(I, printMetadata);
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