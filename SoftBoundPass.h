#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <map>




using namespace llvm;
namespace{
    struct SoftBoundPass : public PassInfoMixin<SoftBoundPass>
    {
        std::map<Value *, TinyPtrVector<Value *>> MValueBaseMap;
        std::map<Value *, TinyPtrVector<Value *>> MValueBoundMap;

        PointerType *MVoidPtrTy;
        FunctionCallee setMetaData;
        FunctionCallee printMetadata;
        FunctionCallee getMetaData;
        FunctionCallee boundCheck;
        FunctionCallee printMetadataTable;
        ArrayRef<Value *> getAssociatedBases(Value *Val);
        Value *getAssociatedBase(Value *pointer_operand);
    };
    
}