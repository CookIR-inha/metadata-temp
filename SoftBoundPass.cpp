// SoftBoundPass.cpp

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/Debug.h"

#include "llvm/Support/Debug.h"
#include <map>
#include <cstddef>
#include <regex>

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
    typedef struct
    {
      void *base;
      void *bound;
    } __softbound_metadata;
    std::map<Value *, TinyPtrVector<Value *>> MValueBaseMap;
    std::map<Value *, TinyPtrVector<Value *>> MValueBoundMap;
    StringMap<bool> m_func_transformed;
    SmallSet<Function *, 32> InstrumentedFunctions;

    LLVMContext *C;
    const DataLayout *DL;
    Type *MSizetTy;
    Type *MArgNoTy;
    Constant *MInfiniteBoundPtr;
    Type *MetadataStructPtrTy;
    bool ClAssociateZeroInitializedGlobalsWithOmnivalidMetadata;
    bool ClAssociateIntToPtrCastsWithOmnivalidMetadata;
    bool ClAssociateVaargPointerWithOmnivalidMetadata;
    bool ClIntraObjectBounds;

    ConstantPointerNull *MVoidNullPtr;
    PointerType *MVoidPtrTy;
    FunctionCallee setMetaData;
    FunctionCallee printMetadata;
    FunctionCallee getMetaData;
    FunctionCallee boundCheck;
    FunctionCallee printMetadataTable;
    FunctionCallee getBaseAddr;
    FunctionCallee getBoundAddr;
    FunctionCallee SBinitTable;
    FunctionCallee StoreMetadataShadowStackFn;
    FunctionCallee AllocateShadowStackFn;
    FunctionCallee LoadMetadataPtrShadowStackFn;
    FunctionCallee LoadMetadataPtrFn;
    FunctionCallee DeallocateShadowStackFn;
    FunctionCallee ASaninit;

    StringMap<bool> MFunctionWrappersAvailable = {
        {"system", true},
        {"setreuid", true},
        {"mkstemp", true},
        {"getrlimit", true},
        {"setrlimit", true},
        {"fread", true},
        {"mkdir", true},
        {"chroot", true},
        {"rmdir", true},
        {"stat", true},
        {"fputc", true},
        {"fileno", true},
        {"fgetc", true},
        {"strncmp", true},
        {"fwrite", true},
        {"atof", true},
        {"feof", true},
        {"remove", true},
        {"tmpfile", true},
        {"ferror", true},
        {"ftell", true},
        {"fstat", true},
        {"fflush", true},
        {"fputs", true},
        {"fopen", true},
        {"fdopen", true},
        {"fseek", true},
        {"ftruncate", true},
        {"popen", true},
        {"fclose", true},
        {"pclose", true},
        {"rewind", true},
        {"readdir", true},
        {"opendir", true},
        {"closedir", true},
        {"rename", true},
        {"getcwd", true},
        {"chown", true},
        {"chdir", true},
        {"strcmp", true},
        {"strcasecmp", true},
        {"strncasecmp", true},
        {"strlen", true},
        {"strpbrk", true},
        {"gets", true},
        {"fgets", true},
        {"perror", true},
        {"strspn", true},
        {"strcspn", true},
        {"memcmp", true},
        {"memchr", true},
        {"rindex", true},
        {"strtoul", true},
        {"strtod", true},
        {"strtol", true},
        {"strchr", true},
        {"strrchr", true},
        {"strcpy", true},
        {"atoi", true},
        {"strtok", true},
        {"strdup", true},
        {"strcat", true},
        {"strncat", true},
        {"strncpy", true},
        {"strstr", true},
        {"signal", true},
        {"atol", true},
        {"realloc", true},
        {"calloc", true},
        {"malloc", true},
        {"mmap", true},

        {"times", true},
        {"strftime", true},
        {"localtime", true},
        {"time", true},
        {"free", true},
        {"ctime", true},
        {"setbuf", true},
        {"getenv", true},
        {"atexit", true},
        {"strerror", true},
        {"unlink", true},
        // TODO[orthen]: fix wrapper to check for arguments as functions with same
        // name can have different declarations
        // {"open", true},
        {"read", true},
        {"write", true},
        {"gettimeofday", true},
        {"select", true},
        {"__errno_location", true},
        {"__ctype_b_loc", true},
        {"__ctype_toupper_loc", true},
        {"__ctype_tolower_loc", true},
        {"qsort", true},
        {"puts", true},
        {"setlocale", true},
        // string.h wrappers
        {"__mempcpy", true},
        {"__stpcpy", true},
        {"__stpncpy", true},
        {"basename", true},
        {"explicit_bzero", true},
        {"memccpy", true},
        {"memfrob", true},
        {"memmem", true},
        {"memmove", true},
        {"memset", true},
        {"rawmemchr", true},
        {"stpncpy", true},
        {"strcasestr", true},
        {"strcoll", true},
        {"strcoll_l", true},
        {"strerror_l", true},
        {"strfry", true},
        {"strnlen", true},
        {"strsep", true},
        {"strsignal", true},
        {"strtok_r", true},
        {"strverscmp", true},
        {"strxfrm", true},
        {"strxfrm_l", true},
        // strings.h wrappers
        {"bcmp", true},
        {"bcopy", true},
        {"bzero", true},
        {"index", true},
        {"strcasecmp_l", true},
        {"strncasecmp_l", true},
        // stdio.h wrappers
        {"__asprintf", true},
        {"__overflow", true},
        {"__uflow", true},
        {"asprintf", true},
        {"clearerr", true},
        {"clearerr_unlocked", true},
        {"ctermid", true},
        {"cuserid", true},
        {"dprintf", true},
        {"feof_unlocked", true},
        {"ferror_unlocked", true},
        {"fflush_unlocked", true},
        {"fgetc_unlocked", true},
        {"fgetpos", true},
        {"fgetpos64", true},
        {"fgets_unlocked", true},
        {"fileno_unlocked", true},
        {"flockfile", true},
        {"fmemopen", true},
        {"fopen64", true},
        {"fopencookie", true},
        {"fprintf", true},
        {"fputc_unlocked", true},
        {"fputs_unlocked", true},
        {"freopen", true},
        {"freopen64", true},
        {"fscanf", true},
        {"fseeko64", true},
        {"fsetpos", true},
        {"fsetpos64", true},
        {"ftello", true},
        {"ftello64", true},
        {"ftrylockfile", true},
        {"funlockfile", true},
        {"fwrite_unlocked", true},
        {"getc", true},
        {"getc_unlocked", true},
        {"getdelim", true},
        {"getline", true},
        {"getw", true},
        {"obstack_printf", true},
        {"obstack_vprintf", true},
        {"open_memstream", true},
        // {"printf", false},
        {"putc", true},
        {"putc_unlocked", true},
        {"putw", true},
        {"renameat2", true},
        {"scanf", true},
        {"setbuffer", true},
        {"setlinebuf", true},
        {"setvbuf", true},
        {"snprintf", true},
        {"sprintf", true},
        {"sscanf", true},
        {"tempnam", true},
        {"tmpfile64", true},
        {"tmpnam", true},
        {"tmpnam_r", true},
        {"vasprintf", true},
        {"vdprintf", true},
        {"vfprintf", true},
        {"vfscanf", true},
        {"vprintf", true},
        {"vscanf", true},
        {"vsnprintf", true},
        {"vsprintf", true},
        {"vsscanf", true},
        {"wcscpy", true},
        // Wrappers for unistd.h
        {"access", true},
        {"brk", true},
        {"confstr", true},
        {"copy_file_range", true},
        // {"crypt", true}, TODO: The wrapper did not compile
        {"eaccess", true},
        {"euidaccess", true},
        // {"execl", true},
        // {"execle", true},
        // {"execlp", true},
        {"execv", true},
        {"execve", true},
        {"execveat", true},
        {"execvp", true},
        {"execvpe", true},
        {"faccessat", true},
        {"fexecve", true},
        {"get_current_dir_name", true},
        {"getdomainname", true},
        {"getgroups", true},
        {"gethostname", true},
        {"getlogin", true},
        {"getlogin_r", true},
        {"getpass", true},
        {"getresgid", true},
        {"getresuid", true},
        {"getusershell", true},
        {"getwd", true},
        {"lchown", true},
        {"link", true},
        {"pipe", true},
        {"pipe2", true},
        {"pread", true},
        {"pread64", true},
        {"profil", true},
        {"pwrite", true},
        {"pwrite64", true},
        {"readlink", true},
        {"revoke", true},
        {"sbrk", true},
        {"setdomainname", true},
        {"sethostname", true},
        {"setlogin", true},
        {"swab", true},
        {"symlink", true},
        {"truncate", true},
        {"truncate64", true},
        {"ttyname", true},
        {"ttyname_r", true},
        {"acct", true},
        {"getentropy", true},
        // stdlib.h
        {"a64l", true},
        {"aligned_alloc", true},
        {"at_quick_exit", true},
        {"atoll", true},
        {"canonicalize_file_name", true},
        {"drand48_r", true},
        {"ecvt", true},
        {"ecvt_r", true},
        {"erand48", true},
        {"erand48_r", true},
        {"fcvt", true},
        {"fcvt_r", true},
        {"gcvt", true},
        {"getloadavg", true},
        {"getsubopt", true},
        {"initstate", true},
        {"initstate_r", true},
        {"jrand48", true},
        {"jrand48_r", true},
        {"l64a", true},
        {"lcong48", true},
        {"lcong48_r", true},
        {"lrand48_r", true},
        {"mblen", true},
        {"mbstowcs", true},
        {"mbtowc", true},
        {"mkostemp64", true},
        {"mkostemps", true},
        {"mkostemps64", true},
        {"mkstemps", true},
        {"mkstemps64", true},
        {"mktemp", true},
        {"mrand48_r", true},
        {"nrand48", true},
        {"nrand48_r", true},
        {"on_exit", true},
        {"posix_memalign", true},
        {"ptsname", true},
        {"ptsname_r", true},
        {"putenv", true},
        {"qecvt", true},
        {"qecvt_r", true},
        {"qfcvt", true},
        {"qfcvt_r", true},
        {"qgcvt", true},
        {"qsort_r", true},
        {"rand_r", true},
        {"random_r", true},
        {"realpath", true},
        {"secure_getenv", true},
        {"seed48", true},
        {"seed48_r", true},
        {"setstate", true},
        {"setstate_r", true},
        {"srand48_r", true},
        {"srandom_r", true},
        {"strfromd", true},
        {"strfromf", true},
        {"strfromf32", true},
        {"strfromf32x", true},
        {"strfromf64", true},
        {"strfromf64x", true},
        {"strfroml", true},
        {"strtod_l", true},
        {"strtof", true},
        {"strtof32", true},
        {"strtof32_l", true},
        {"strtof32x", true},
        {"strtof32x_l", true},
        {"strtof64", true},
        {"strtof64_l", true},
        {"strtof64x", true},
        {"strtof64x_l", true},
        {"strtof_l", true},
        {"strtol_l", true},
        {"strtold", true},
        {"strtold_l", true},
        {"strtoll", true},
        {"strtoll_l", true},
        {"strtoq", true},
        {"strtoul_l", true},
        {"strtoull", true},
        {"strtoull_l", true},
        {"strtouq", true},
        {"wcstombs", true},
        {"wctomb", true},
        {"reallocarray", true},
        // malloc.h
        {"malloc_info", true},
        {"malloc_usable_size", true},
        {"memalign", true},
        {"pvalloc", true},
        {"valloc", true},
    };
    // For constants containing multiple pointers use getAssociatedBaseArray.
    // NOTE: summary for constants
    // just like getConstantExprBaseBound recursively
    // 1. handle ConstantData
    // a) ConstantPointerNull -> invalid metadata by createDummyMetadata
    // b) Undef -> invalid metadata by createDummyMetadata if it contains pointers
    // c) ConstantAggregateZero -> invalid metadata by createDummyMetadata if it
    // contains pointers
    // 2. handle ConstantAggregate
    // a) ConstantArray -> call getConstantExprBaseBound on one element, push times
    // Numelement on metadata array b) ConstantStruct -> call
    // getConstantExprBaseBound on each subelement c) ConstantVector -> if it
    // contains pointers, create metadata vector with invalid metadata
    // 3. Globals
    // a) Function -> create base=pointer, bound=pointer+DL.getPointerSize
    // b) Global Variable -> get underlying type -> get typesizeinbits
    template <typename T>
    TinyPtrVector<T *> createDummyMetadata(Type *Ty,
                                           Constant *Metadatum)
    {
      TinyPtrVector<T *> DummyMetadata;
      auto MetadataOrder = getMetadataOrder(Ty);

      for (auto &MetadataType : MetadataOrder)
      {
        if (std::get<0>(MetadataType) == 0)
        {
          DummyMetadata.push_back(Metadatum);
        }
        else if (std::get<0>(MetadataType) == 1)
        {
          auto VectorSize = std::get<1>(MetadataType);
          SmallVector<Constant *> MetadataArray(VectorSize, Metadatum);
          auto *MetadataVector = ConstantVector::get(MetadataArray);
          DummyMetadata.push_back(MetadataVector);
        }
      }
      return DummyMetadata;
    }
    bool isVectorWithPointers(Type *Ty)
    {
      auto *VectorTy = dyn_cast<VectorType>(Ty);
      if (VectorTy && VectorTy->getElementType()->isPointerTy())
      {
        return true;
      }
      return false;
    }
    size_t flattenAggregateIndices(Type *Ty,
                                   ArrayRef<unsigned> Indices)
    {
      switch (Ty->getTypeID())
      {
      case Type::PointerTyID:
      {
        // assert(Indices.empty() && "Too many indices for aggregate type!");
        return 0;
      }

      case Type::StructTyID:
      {
        // assert(!Indices.empty() && "Too few indices for aggregate type!");
        ArrayRef<Type *> Subtypes = Ty->subtypes();
        // assert(Indices[0] < Subtypes.size() && "Aggregate index out of bounds!");

        size_t Sum = 0;
        for (unsigned J = 0; J < Indices[0]; ++J)
        {
          Sum += countMetadata(Subtypes[J]);
        }
        Sum += flattenAggregateIndices(Subtypes[Indices[0]], Indices.drop_front(1));
        return Sum;
      }

      case Type::ArrayTyID:
      {
        // assert(!Indices.empty() && "Too few indices for aggregate type!");
        ArrayType *ArrayTy = cast<ArrayType>(Ty);
        // assert(Indices[0] < ArrayTy->getNumElements() &&
        //  "Aggregate index out of bounds!");
        Type *ElTy = ArrayTy->getElementType();

        return Indices[0] * countMetadata(ElTy) +
               flattenAggregateIndices(ElTy, Indices.drop_front(1));
      }

      case Type::FixedVectorTyID:
      {
        if (isVectorWithPointers(Ty))
        {
          return 0;
        }
        break;
      }
      default:
      {
        // assert(Indices.empty() && "Too many indices for aggregate type!");
        return 0;
      }
      }
      return 0;
    }
    template <typename T>
    TinyPtrVector<T *> createConstantBases(Constant *Const)
    {
      TinyPtrVector<T *> Bases;

      if (MValueBaseMap.count(Const))
      {
        auto ConstBases = MValueBaseMap[Const];
        for (auto *Base : ConstBases)
        {
          Bases.push_back(dyn_cast<Constant>(Base));
        }
        return Bases;
      }

      if (isa<ConstantPointerNull>(Const))
      {
        Bases.push_back(MVoidNullPtr);
        return Bases;
      }
      if (isa<Function>(Const))
      {
        auto *BaseCast = ConstantExpr::getBitCast(Const, MVoidPtrTy);
        Bases.push_back(BaseCast);
        return Bases;
      }
      if (auto *Global = dyn_cast<GlobalValue>(Const))
      {
        auto *ConstCast = ConstantExpr::getBitCast(Const, MVoidPtrTy);
        Bases.push_back(ConstCast);
        return Bases;
      }
      if (auto *Undefined = dyn_cast<UndefValue>(Const))
      {
        return createDummyMetadata<T>(Undefined->getType(), MVoidNullPtr);
      }
      if (auto *CAggZero = dyn_cast<ConstantAggregateZero>(Const))
      {
        return createDummyMetadata<T>(CAggZero->getType(), MVoidNullPtr);
      }
      if (ConstantExpr *Expr = dyn_cast<ConstantExpr>(Const))
      {

        // ignore all types that do not contain pointers
        if (!isTypeWithPointers(Expr->getType()))
          return Bases;

        switch (Expr->getOpcode())
        {
        case Instruction::GetElementPtr:
        {
          Constant *Op = cast<Constant>(Expr->getOperand(0));
          return createConstantBases<T>(Op);
        }

        case BitCastInst::BitCast:
        {
          Constant *Op = cast<Constant>(Expr->getOperand(0));
          return createConstantBases<T>(Op);
        }

        case Instruction::IntToPtr:
        {
          Bases.push_back(MVoidNullPtr);
          return Bases;
        }

        case Instruction::ExtractElement:
        {
          auto *VBases = dyn_cast<Constant>(getAssociatedBase(Expr->getOperand(0)));
          auto *Idx = Expr->getOperand(1);
          auto *EBase = ConstantExpr::getExtractElement(VBases, Idx);
          Bases.push_back(EBase);
          return Bases;
        }

        case Instruction::InsertElement:
        {
          auto *VBases = dyn_cast<Constant>(getAssociatedBase(Expr->getOperand(0)));
          auto *ElementBase = Expr->getOperand(1);
          auto *Idx = Expr->getOperand(2);
          auto *NewBases = ConstantExpr::getInsertElement(VBases, ElementBase, Idx);
          Bases.push_back(NewBases);
          return Bases;
        }

        case Instruction::ExtractValue:
        {
          Value *AggOp = Expr->getOperand(0);
          auto VBases = getAssociatedBases(AggOp);

          size_t StartIdx =
              flattenAggregateIndices(AggOp->getType(), Expr->getIndices());
          size_t MetadataCount = countMetadata(Expr->getType());

          for (size_t J = StartIdx; J < StartIdx + MetadataCount; ++J)
          {
            Value *Base = VBases[J];
            Bases.push_back(dyn_cast<T>(Base));
          }

          return Bases;
        }

        case Instruction::InsertValue:
        {
          Constant *AggOp = Expr->getOperand(0);
          Constant *ValOp = Expr->getOperand(1);

          auto AggBases = getAssociatedBases(AggOp);
          for (auto *Base : AggBases)
          {
            Bases.push_back(dyn_cast<T>(Base));
          }

          if (!isTypeWithPointers(ValOp->getType()))
            return Bases;

          auto ValBases = getAssociatedBases(ValOp);
          size_t Idx = flattenAggregateIndices(AggOp->getType(), Expr->getIndices());

          for (unsigned J = 0; J < ValBases.size(); ++J)
          {
            static_cast<MutableArrayRef<T *>>(Bases)[Idx + J] = dyn_cast<T>(ValBases[J]);
          }

          return Bases;
        }
        }
      }
      if (auto *CAgg = dyn_cast<ConstantAggregate>(Const))
      {
        if (auto *CArray = dyn_cast<ConstantArray>(CAgg))
        {
          for (size_t i = 0; i < CArray->getType()->getNumElements(); i++)
          {
            auto CBases =
                createConstantBases<T>(CArray->getAggregateElement((unsigned)i));
            for (auto *Base : CBases)
            {
              Bases.push_back(Base);
            }
          }
          return Bases;
        }

        if (auto *CVector = dyn_cast<ConstantVector>(CAgg))
        {
          auto *VectorTy = CVector->getType();
          if (isVectorWithPointers(VectorTy))
          {
            TinyPtrVector<Constant *> VecBases;
            for (size_t i = 0; i < VectorTy->getNumElements(); i++)
            {
              auto ElBases =
                  createConstantBases<Constant>(CVector->getAggregateElement(i));
              VecBases.push_back(ElBases.front());
            }
            auto *VecBase = ConstantVector::get(VecBases);
            Bases.push_back(VecBase);
          }
          return Bases;
        }

        if (auto *CStruct = dyn_cast<ConstantStruct>(CAgg))
        {
          for (size_t i = 0; i < CStruct->getType()->getNumElements(); i++)
          {
            auto ElBases = createConstantBases<T>(CStruct->getAggregateElement(i));
            for (auto *Base : ElBases)
            {
              Bases.push_back(Base);
            }
          }
          return Bases;
        }
      }

      return Bases;
    }
    size_t countMetadata(Type *Ty)
    {
      switch (Ty->getTypeID())
      {
      case Type::PointerTyID:
        return 1;
      case Type::StructTyID:
      {
        size_t Sum = 0;
        for (Type::subtype_iterator I = Ty->subtype_begin(), E = Ty->subtype_end();
             I != E; ++I)
        {
          Sum += countMetadata(*I);
        }
        return Sum;
      }
      case Type::ArrayTyID:
      {
        ArrayType *ArrayTy = cast<ArrayType>(Ty);
        return countMetadata(ArrayTy->getElementType()) * ArrayTy->getNumElements();
      }
      case Type::FixedVectorTyID:
      {
        return 1;
      }
      case Type::ScalableVectorTyID:
      {
        // assert(0 && "Counting metadata for scalable vectors not yet handled.");
      }
      default:
        return 0;
      }
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
      // assert((MValueBaseMap.count(pointer_operand) == 0) &&
      //  "dissociating base failed\n");
      // assert((MValueBoundMap.count(pointer_operand) == 0) &&
      //  "dissociating bound failed");
    }
    inline void associateBaseBound(Value *Val, Value *Base,
                                   Value *Bound)
    {
      if (MValueBaseMap.count(Val))
      {
        errs() << "disassociate\n";
        dissociateBaseBound(Val);
      }

      MValueBaseMap[Val] = {Base};
      MValueBoundMap[Val] = {Bound};
    }
    inline void associateAggregateBase(Value *Val,
                                       TinyPtrVector<Value *> &Bases)
    {
      auto NumMetadata = countMetadata(Val->getType());
      // assert(NumMetadata == Bases.size() &&
      //  "Bases size is not equal to number of metadata in type");
      MValueBaseMap[Val] = Bases;
    }
    inline void associateAggregateBound(Value *Val,
                                        TinyPtrVector<Value *> &Bounds)
    {
      auto NumMetadata = countMetadata(Val->getType());
      // assert(NumMetadata == Bounds.size() &&
      //  "Bounds size is not equal to number of metadata in type");
      MValueBoundMap[Val] = Bounds;
    }
    inline void associateAggregateBaseBound(
        Value *Val, TinyPtrVector<Value *> &Bases, TinyPtrVector<Value *> &Bounds)
    {
      if (MValueBaseMap.count(Val))
      {
        dissociateBaseBound(Val);
      }
      associateAggregateBase(Val, Bases);
      associateAggregateBound(Val, Bounds);
    }
    ArrayRef<Value *> getAssociatedBases(Value *Val)
    {
      if (!MValueBaseMap.count(Val))
      {
        if (auto *Const = dyn_cast<Constant>(Val))
        {
          // errs() << "constant pointers \n";
          auto Bases = createConstantBases<Value>(Const);
          associateAggregateBase(Val, Bases);
        }
        else if (true)
        {
          // errs() << "dummymetadata\n";
          auto DummyMetadata =
              createDummyMetadata<Value>(Val->getType(), MVoidNullPtr);
          associateAggregateBase(Val, DummyMetadata);
        }
        else
        {
          // assert(0 && "No associated base(s)");
        }
      }
      // errs() <<"returning exist data \n";
      return MValueBaseMap[Val];
    }

    // WARNING: This method only handles constants with a single pointer correctly.
    // For constants containing multiple pointers use getAssociatedBaseArray.
    Value *getAssociatedBase(Value *pointer_operand)
    {
      ArrayRef<Value *> base_array = getAssociatedBases(pointer_operand);

      // assert(base_array.size() == 1 && "getAssociatedBase called on aggregate");
      Value *pointer_base = base_array[0];

      return pointer_base;
    }
    void collectVarargPointerLoads(Instruction *Root, SmallVector<LoadInst *> &LoadInsts,
                                   std::set<Value *> &Visited)
    {
      if (Visited.count(Root))
        return; // Avoid cycles
      Visited.insert(Root);

      for (auto *Usr : Root->users())
      {
        Instruction *I = dyn_cast<Instruction>(Usr);

        if (!I || isa<CallBase>(I))
          continue;
        // Check if the user is a LoadInst and loads a pointer
        if (auto *LI = dyn_cast<LoadInst>(Usr))
        {
          if (LI->getType()->isPointerTy())
          {
            LoadInsts.push_back(LI); // Collect LoadInst
          }
        }

        // Continue the traversal for other users
        collectVarargPointerLoads(I, LoadInsts, Visited);
      }
    }
    void associateOmnivalidMetadata(Value *Val)
    {
      auto *ValTy = Val->getType();
      auto Bases = createDummyMetadata<Value>(ValTy, MVoidNullPtr);
      auto Bounds = createDummyMetadata<Value>(ValTy, MInfiniteBoundPtr);
      associateAggregateBaseBound(Val, Bases, Bounds);
    }
    void varargAssociatePointerLoads(Instruction *VarArgInst)
    {

      SmallVector<LoadInst *> LoadInsts;
      std::set<Value *> Visited;
      collectVarargPointerLoads(VarArgInst, LoadInsts, Visited);

      for (auto *LI : LoadInsts)
      {
        associateOmnivalidMetadata(LI);
      }
    }

    template <typename T>
    TinyPtrVector<T *> createConstantBounds(Constant *Const,
                                            bool isGlobal = false)
    {
      TinyPtrVector<T *> Bounds;

      if (MValueBoundMap.count(Const))
      {
        auto ConstBounds = MValueBoundMap[Const];
        for (auto *Bound : ConstBounds)
        {
          Bounds.push_back(dyn_cast<Constant>(Bound));
        }
        return Bounds;
      }

      if (isa<ConstantPointerNull>(Const))
      {
        if (isGlobal && ClAssociateZeroInitializedGlobalsWithOmnivalidMetadata)
          Bounds.push_back(MInfiniteBoundPtr);
        else
          Bounds.push_back(MVoidNullPtr);
        return Bounds;
      }
      if (isa<Function>(Const))
      {
        auto *BoundCast = ConstantExpr::getBitCast(Const, MVoidPtrTy);
        Bounds.push_back(BoundCast);
        return Bounds;
      }
      if (auto *Global = dyn_cast<GlobalValue>(Const))
      {
        Type *GlobalTy = Global->getValueType();

        Constant *GEPIdx =
            ConstantInt::get(Type::getInt32Ty(GlobalTy->getContext()), 1);
        Constant *Bound = ConstantExpr::getGetElementPtr(GlobalTy, Const, GEPIdx);
        auto *BoundCast = ConstantExpr::getBitCast(Bound, MVoidPtrTy);
        Bounds.push_back(BoundCast);
        return Bounds;
      }
      if (auto *Undefined = dyn_cast<UndefValue>(Const))
      {
        if (isGlobal && ClAssociateZeroInitializedGlobalsWithOmnivalidMetadata)
          return createDummyMetadata<T>(Undefined->getType(), MInfiniteBoundPtr);
        return createDummyMetadata<T>(Undefined->getType(), MVoidNullPtr);
      }
      if (auto *CAggZero = dyn_cast<ConstantAggregateZero>(Const))
      {
        if (isGlobal && ClAssociateZeroInitializedGlobalsWithOmnivalidMetadata)
          return createDummyMetadata<T>(CAggZero->getType(), MInfiniteBoundPtr);
        return createDummyMetadata<T>(CAggZero->getType(), MVoidNullPtr);
      }
      if (ConstantExpr *Expr = dyn_cast<ConstantExpr>(Const))
      {

        // ignore all types that do not contain pointers
        if (!isTypeWithPointers(Expr->getType()))
          return Bounds;

        switch (Expr->getOpcode())
        {
        case Instruction::GetElementPtr:
        {
          Constant *Op = cast<Constant>(Expr->getOperand(0));
          return createConstantBounds<T>(Op);
        }

        case BitCastInst::BitCast:
        {
          Constant *Op = cast<Constant>(Expr->getOperand(0));
          return createConstantBounds<T>(Op);
        }

        case Instruction::IntToPtr:
        {
          if (ClAssociateIntToPtrCastsWithOmnivalidMetadata)
            Bounds.push_back(MInfiniteBoundPtr);
          else
            Bounds.push_back(MVoidNullPtr);
          return Bounds;
        }

        case Instruction::ExtractElement:
        {
          auto *VBounds =
              dyn_cast<Constant>(getAssociatedBound(Expr->getOperand(0)));
          auto *Idx = Expr->getOperand(1);
          auto *EBound = ConstantExpr::getExtractElement(VBounds, Idx);
          Bounds.push_back(EBound);
          return Bounds;
        }

        case Instruction::InsertElement:
        {
          auto *VBounds =
              dyn_cast<Constant>(getAssociatedBound(Expr->getOperand(0)));
          auto *ElementBound = Expr->getOperand(1);
          auto *Idx = Expr->getOperand(2);
          auto *NewBounds =
              ConstantExpr::getInsertElement(VBounds, ElementBound, Idx);
          Bounds.push_back(NewBounds);
          return Bounds;
        }

        case Instruction::ExtractValue:
        {
          Value *AggOp = Expr->getOperand(0);
          auto VBounds = getAssociatedBounds(AggOp);

          size_t StartIdx =
              flattenAggregateIndices(AggOp->getType(), Expr->getIndices());
          size_t MetadataCount = countMetadata(Expr->getType());

          for (size_t J = StartIdx; J < StartIdx + MetadataCount; ++J)
          {
            Value *Bound = VBounds[J];
            Bounds.push_back(dyn_cast<T>(Bound));
          }

          return Bounds;
        }

        case Instruction::InsertValue:
        {
          Constant *AggOp = Expr->getOperand(0);
          Constant *ValOp = Expr->getOperand(1);

          auto AggBounds = getAssociatedBounds(AggOp);
          for (auto *Bound : AggBounds)
          {
            Bounds.push_back(dyn_cast<T>(Bound));
          }

          if (!isTypeWithPointers(ValOp->getType()))
            return Bounds;

          auto ValBounds = getAssociatedBounds(ValOp);
          size_t Idx =
              flattenAggregateIndices(AggOp->getType(), Expr->getIndices());

          for (unsigned J = 0; J < ValBounds.size(); ++J)
          {
            static_cast<MutableArrayRef<T *>>(Bounds)[Idx + J] =
                dyn_cast<T>(ValBounds[J]);
          }

          return Bounds;
        }
        }
      }
      if (auto *CAgg = dyn_cast<ConstantAggregate>(Const))
      {
        if (auto *CArray = dyn_cast<ConstantArray>(CAgg))
        {
          for (size_t i = 0; i < CArray->getType()->getNumElements(); i++)
          {
            auto CBounds =
                createConstantBounds<T>(CArray->getAggregateElement((unsigned)i));
            for (auto *Bound : CBounds)
            {
              Bounds.push_back(Bound);
            }
          }
          return Bounds;
        }

        if (auto *CVector = dyn_cast<ConstantVector>(CAgg))
        {
          auto *VectorTy = CVector->getType();
          if (isVectorWithPointers(VectorTy))
          {
            TinyPtrVector<Constant *> VecBounds;
            for (size_t i = 0; i < VectorTy->getNumElements(); i++)
            {
              auto ElBounds =
                  createConstantBounds<Constant>(CVector->getAggregateElement(i));
              VecBounds.push_back(ElBounds.front());
            }
            auto *VecBound = ConstantVector::get(VecBounds);
            Bounds.push_back(VecBound);
          }
          return Bounds;
        }

        if (auto *CStruct = dyn_cast<ConstantStruct>(CAgg))
        {
          for (size_t i = 0; i < CStruct->getType()->getNumElements(); i++)
          {
            auto ElBounds =
                createConstantBounds<T>(CStruct->getAggregateElement(i));
            for (auto *Bound : ElBounds)
            {
              Bounds.push_back(Bound);
            }
          }
          return Bounds;
        }
      }

      return Bounds;
    }
    ArrayRef<Value *> getAssociatedBounds(Value *Val)
    {
      if (!MValueBoundMap.count(Val))
      {
        if (auto *Const = dyn_cast<Constant>(Val))
        {
          auto Bounds = createConstantBounds<Value>(Const);
          associateAggregateBound(Val, Bounds);
        }
        else if (true)
        {
          Constant *Metadatum;
          if (true)
          {
            Metadatum = MInfiniteBoundPtr;
          }
          else
          {
            Metadatum = MVoidNullPtr;
          }
          auto DummyMetadata =
              createDummyMetadata<Value>(Val->getType(), Metadatum);
          MValueBoundMap[Val] = DummyMetadata;
        }
        else
        {

          // assert(0 && "No associated bound(s)");
        }
      }
      return MValueBoundMap[Val];
    }

    // WARNING: This method only handles constants with a single pointer correctly.
    // For constants containing multiple pointers use getAssociatedBoundArray.
    Value *getAssociatedBound(Value *pointer_operand)
    {
      ArrayRef<Value *> bound_array = getAssociatedBounds(pointer_operand);
      // assert(bound_array.size() == 1 && "getAssociatedBound called on aggregate");
      Value *pointer_bound = bound_array[0];

      return pointer_bound;
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
        // assert(0 && "Counting pointers for scalable vectors not yet handled.");
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
    Value *castToVoidPtr(Value *Op, Instruction *InsertAt)
    {
      IRBuilder<> IRB(InsertAt);
      return castToVoidPtr(Op, IRB);
    }
    size_t countPointersInType(Type *Ty)
    {
      switch (Ty->getTypeID())
      {
      case Type::PointerTyID:
        return 1;
      case Type::StructTyID:
      {
        size_t Sum = 0;
        for (Type::subtype_iterator I = Ty->subtype_begin(), E = Ty->subtype_end();
             I != E; ++I)
        {
          Sum += countPointersInType(*I);
        }
        return Sum;
      }
      case Type::ArrayTyID:
      {
        ArrayType *ArrayTy = cast<ArrayType>(Ty);
        return countPointersInType(ArrayTy->getElementType()) *
               ArrayTy->getNumElements();
      }
      case Type::FixedVectorTyID:
      {
        FixedVectorType *FVTy = cast<FixedVectorType>(Ty);
        return countPointersInType(FVTy->getElementType()) * FVTy->getNumElements();
      }
      // TODO[orthen]: enable this too with ElementCount, isScalar, getValue
      case Type::ScalableVectorTyID:
      {
        // assert(0 && "Counting pointers for scalable vectors not yet handled.");
      }
      default:
        return 0;
      }
    }
    SmallVector<Value *, 8> extractVectorValues(Value *MetadataVector,
                                                Instruction *InsertAt)
    {
      const VectorType *VectorTy = dyn_cast<VectorType>(MetadataVector->getType());
      // assert(VectorTy && "MetadataVector Value is not a VectorType");
      ElementCount NumElements = VectorTy->getElementCount();
      SmallVector<Value *, 8> Metadata;

      for (uint64_t I = 0; I < NumElements.getKnownMinValue(); I++) // changed in llvm-14
      {
        Constant *Idx =
            ConstantInt::get(Type::getInt32Ty(InsertAt->getContext()), I);
        Value *Metadatum =
            ExtractElementInst::Create(MetadataVector, Idx, "", InsertAt);
        Metadata.push_back(Metadatum);
      }
      return Metadata;
    }
    SmallVector<Value *> unpackMetadataArray(ArrayRef<Value *> MetadataVals,
                                             Instruction *InsertAt)
    {
      SmallVector<Value *> UnpackedMetadata;
      for (auto *MetadataVal : MetadataVals)
      {
        switch (MetadataVal->getType()->getTypeID())
        {
        case Type::FixedVectorTyID:
        {
          auto VectorMetadata = extractVectorValues(MetadataVal, InsertAt);
          UnpackedMetadata.append(VectorMetadata);
          break;
        }
        default:
          UnpackedMetadata.push_back(MetadataVal);
        }
      }
      return UnpackedMetadata;
    }
    size_t introduceShadowStackStores(Value *Val, Instruction *InsertAt, int ArgNo)
    {
      size_t NumPtrs = countPointersInType(Val->getType());
      if (NumPtrs == 0)
        return 0;

      SmallVector<Value *, 8> Args, UnpackedBases, UnpackedBounds, UnpackedKeys,
          UnpackedLocks;
      unsigned MetadataSize = 0;
      IRBuilder<> IRB(InsertAt);

      /** (ds)
       * Push metadata of each contained pointer on the shadow stack linearly.
       */

      auto AssociatedBases = getAssociatedBase(Val);
      auto AssociatedBounds = getAssociatedBound(Val);
      UnpackedBases = unpackMetadataArray(AssociatedBases, InsertAt);
      UnpackedBounds = unpackMetadataArray(AssociatedBounds, InsertAt);
      MetadataSize = UnpackedBases.size();

      for (unsigned Idx = 0; Idx < MetadataSize; ++Idx)
      {
        Args.clear();
        Value *BaseCast = castToVoidPtr(UnpackedBases[Idx], InsertAt);
        Args.push_back(BaseCast);
        Value *BoundCast = castToVoidPtr(UnpackedBounds[Idx], InsertAt);
        Args.push_back(BoundCast);

        Args.push_back(ConstantInt::get(MArgNoTy, ArgNo + Idx, false));
        IRB.CreateCall(StoreMetadataShadowStackFn, Args);
      }

      return NumPtrs;
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
      errs() << "alloca type id : " << AI->getAllocatedType()->getTypeID() << "\n";
      // Bound 주소 계산: GEP 명령어로 base 주소에 Idx를 더해 bound 주소 계산
      Value *Bound = builder.CreateGEP(AI->getAllocatedType(), AI, Idx, "mtmp");
      Bound = castToVoidPtr(Bound, builder);

      associateBaseBound(AI, base, Bound);
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
        // builder.CreateCall(boundCheck, {base, bound, access});
        return;
      }
      base = getAssociatedBase(src);
      bound = getAssociatedBound(src);
      if (!base || !bound)
      {
        errs() << "metadata not exist allocating temporal data\n";
      }
      if (isa<PointerType>(type))
      {
        builder.CreateCall(setMetaData, {access, base, bound});
      }

      // vector의 경우 아직 고려하지 않음
    };
    // void handleArrayGEP(GetElementPtrInst *GEPI, Value *&base, Value *&bound, IRBuilder<> &IRB)
    // {
    //   ArrayType *ATy = cast<ArrayType>(GEPI->getPointerOperandType()->getPointerElementType());

    //   // GEP의 인덱스 값을 가져옴
    //   Value *Index = GEPI->getOperand(2); // 배열 인덱스 (i32 또는 i64)

    //   // 배열 요소의 크기 계산
    //   DataLayout DL(GEPI->getModule());
    //   uint64_t ElementSize = DL.getTypeAllocSize(ATy->getElementType());

    //   // base와 bound를 배열 요소 수준으로 조정
    //   base = IRB.CreateGEP(base, IRB.CreateMul(Index, IRB.getInt64(ElementSize)));
    //   bound = IRB.CreateGEP(base, IRB.getInt64(ElementSize));

    //   // 새롭게 계산된 base와 bound를 GEP와 연관시킴
    //   associateBaseBound(GEPI, base, bound);
    // }
    // void handleStructGEP(GetElementPtrInst *GEPI, Value *&base, Value *&bound, IRBuilder<> &IRB)
    // {
    //   StructType *STy = cast<StructType>(GEPI->getPointerOperandType()->getPointerElementType());
    //   unsigned FieldIndex = cast<ConstantInt>(GEPI->getOperand(2))->getZExtValue(); // 필드 인덱스 가져오기

    //   DataLayout DL(GEPI->getModule()); // DataLayout에서 구조체 크기 계산
    //   const StructLayout *SL = DL.getStructLayout(STy);

    //   // 필드의 시작 위치와 끝 위치 계산
    //   uint64_t FieldStart = SL->getElementOffset(FieldIndex);
    //   uint64_t FieldSize = DL.getTypeAllocSize(STy->getElementType(FieldIndex));
    //   uint64_t FieldEnd = FieldStart + FieldSize;

    //   // 기존 base와 bound를 조정하여 필드 수준으로 설정
    //   base = IRB.CreateGEP(base, IRB.getInt64(FieldStart));
    //   bound = IRB.CreateGEP(base, IRB.getInt64(FieldEnd - FieldStart));

    //   // 새롭게 계산된 base와 bound를 GEP와 연관시킴
    //   associateBaseBound(GEPI, base, bound);
    // }
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
      // associateBaseBound(GEPI, base, bound); // %ptr = base+offset, %ptr에 대한 base, bound
      if (ClIntraObjectBounds)
      {
        Type *PtrType = GEPI->getPointerOperandType()->getPointerElementType();
        if (StructType *STy = dyn_cast<StructType>(PtrType))
        {
          // GEP가 구조체 필드를 참조하는 경우
          errs() << "handling struct type\n";
          // handleStructGEP(GEPI, base, bound, IRB);
        }
        else if (PtrType->isArrayTy())
        {
          // GEP가 배열 요소를 참조하는 경우
          errs() << "handling array type\n";
          // handleArrayGEP(GEPI, base, bound, IRB);
        }
        else
        {
          // GEP가 다른 타입을 참조할 경우 기본 처리
          associateBaseBound(GEPI, base, bound);
        }
      }
      else
      {
        // 기본 GEP 처리
        associateBaseBound(GEPI, base, bound);
      }
      // 새로운 GEP 명령어에 메타데이터 연결
    };
    bool isVaargGep(GetElementPtrInst *GepInst)
    {
      /* (av)
        We want to identify all getelementptr instructions that load the
        pointer to overflow_arg_area or the reg_save_area. That is e.g.
        %0 = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag*
        %arraydecay2, i32 0, i32 3 or %overflow_arg_area_p = getelementptr inbounds
        %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay2, i32 0, i32 2 As
        this struct could be part of another struct or array, we have to iteratively
        follow the indices of the instruction.
      */

      // return false;
      auto GEPIndices = GepInst->getNumIndices();
      if (GEPIndices < 2)
        return false;

      auto *CompTy = GepInst->getSourceElementType();

      auto *Idx = GepInst->idx_begin();

      Value *ThirdLastIdx = NULL;
      Value *SecondLastIdx = *Idx;

      Idx++;

      for (unsigned int NextIndexPos = 2; Idx != GepInst->idx_end();
           Idx++, NextIndexPos++)
      {

        if (!CompTy)
          return false;

        // integer value of index
        uint64_t LastIndex = dyn_cast<ConstantInt>(*Idx)
                                 ? dyn_cast<ConstantInt>(*Idx)->getZExtValue()
                                 : 0;

        // stop if we followed all but the last two indices
        // check if the second to last is 0 and the last is 2 or 3
        if (NextIndexPos >= GEPIndices)
        {
          if (LastIndex != 2 && LastIndex != 3)
            return false;
          break;
        }

        ThirdLastIdx = SecondLastIdx;
        SecondLastIdx = *Idx;

        if (NextIndexPos > 1)
        {
          CompTy = GetElementPtrInst::getTypeAtIndex(CompTy, ThirdLastIdx);
        }
      }

      StructType *StTy = dyn_cast_or_null<StructType>(CompTy);

      return StTy && StTy->hasName() &&
             StTy->getName().contains("struct.__va_list_tag");
    }

    void handle_load(Instruction &I)
    {
      LoadInst *LI = dyn_cast<LoadInst>(&I);
      Type *LoadTy = LI->getType();
      errs() << "Load instruction's type ID is : " << LI->getType()->getTypeID() << "\n";
      Metadata data;
      int typeID = LoadTy->getTypeID();

      Value *LoadSrc = LI->getPointerOperand();
      IRBuilder<> IRB(LI);
      Value *LoadSrcBitcast = castToVoidPtr(LoadSrc, IRB);
      if (!isTypeWithPointers(LoadTy))
      {
        return;
      }
      GetElementPtrInst *GepInst =
          dyn_cast<GetElementPtrInst>(LI->getPointerOperand());
      if (GepInst && isVaargGep(GepInst))
      {
        associateOmnivalidMetadata(LI);
        if (ClAssociateVaargPointerWithOmnivalidMetadata)
          varargAssociatePointerLoads(LI);
      }

      if (isa<PointerType>(LoadTy))
      {
        // handling pointer
        if (!(MValueBaseMap.count(LI) && MValueBoundMap.count(LI)))
        {
          errs() << "Working\n";
          auto *MetadataPtr = IRB.CreateCall(LoadMetadataPtrFn, {LoadSrcBitcast});
          auto *BasePtr = IRB.CreateStructGEP(MetadataPtr->getType()->getPointerElementType(), MetadataPtr, 0, "baseptr");
          data.Base = IRB.CreateLoad(BasePtr->getType()->getPointerElementType(), BasePtr, "base");
          auto *BoundPtr = IRB.CreateStructGEP(MetadataPtr->getType()->getPointerElementType(), MetadataPtr, 1, "boundptr");
          data.Bound = IRB.CreateLoad(BoundPtr->getType()->getPointerElementType(), BoundPtr, "base");
          associateBaseBound(LI, data.Base, data.Bound);
        }
      }
      else if (LoadTy->isAggregateType())
      {
        errs() << "THIS IS AGGREGATE TYPE\n";
        // handleAggregateLoad(Load);
      }
    };

    void handle_bitcast(Instruction &I)
    {
      BitCastInst *BCI = dyn_cast<BitCastInst>(&I);
      if (!BCI)
        return;

      Value *SrcPtr = BCI->getOperand(0); // 원본 포인터
      Value *DstPtr = &I;                 // 변환된 포인터

      // 원본 포인터의 메타데이터 가져오기
      // errs() << "assoc in bitcast : " << *SrcPtr << "\n";
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

    size_t getNumPointerArgs(const CallBase *CB)
    {
      size_t NumPointerArgs = 0;

      for (const Use &Arg : CB->args())
        NumPointerArgs += countPointersInType(Arg->getType());

      return NumPointerArgs;
    }
    void introduceShadowStackAllocation(CallBase *CallInst,
                                        int NumPtrs)
    {
      Value *NumTotalPtrArgs = ConstantInt::get(
          Type::getInt64Ty(CallInst->getType()->getContext()), NumPtrs, false);
      TinyPtrVector<Value *> Args;
      Args.push_back(NumTotalPtrArgs);
      errs() << "**** call instruction : " << *CallInst << " num of arguments : " << *NumTotalPtrArgs << "\n";
      CallInst::Create(AllocateShadowStackFn, Args, "", CallInst);
    }
    SmallVector<std::tuple<uint8_t, uint8_t>, 8> getMetadataOrder(Type *Ty)
    {
      // Inner visitor struct containing state.
      struct TypeVisitor
      {
        SmallVector<std::tuple<uint8_t, uint8_t>, 8> &MetadataOrder;

        void visit(Type *Ty)
        {
          switch (Ty->getTypeID())
          {
          case Type::PointerTyID:
          {
            MetadataOrder.push_back({0, 1});
            break;
          }

          case Type::StructTyID:
          {
            ArrayRef<Type *> SubTypes = Ty->subtypes();
            // visit each subtype in struct
            for (unsigned J = 0; J < SubTypes.size(); ++J)
            {
              visit(SubTypes[J]);
            }
            break;
          }

          case Type::ArrayTyID:
          {
            ArrayType *ArrayTy = cast<ArrayType>(Ty);
            Type *ElTy = ArrayTy->getElementType();
            // visit the array element type n times
            // this is rather inefficient for larger arrays as we duplicate work
            // a better solution would be to visit elt_type once and copy the
            // results
            for (unsigned J = 0; J < ArrayTy->getNumElements(); ++J)
            {
              visit(ElTy);
            }
            break;
          }

          case Type::FixedVectorTyID:
          {
            FixedVectorType *FVTy = cast<FixedVectorType>(Ty);
            if (FVTy->getElementType()->isPointerTy())
            {
              MetadataOrder.push_back({1, FVTy->getNumElements()});
            }
            break;
          }

          default:
          {
          }
          }
        }
      };

      SmallVector<std::tuple<uint8_t, uint8_t>, 8> MetadataOrder;
      struct TypeVisitor Visitor = {MetadataOrder};
      Visitor.visit(Ty);
      return MetadataOrder;
    }
    Value *createMetadataVector(ArrayRef<Value *> SingleMetadataVals,
                                Instruction *InsertAt)
    {

      uint64_t NumPtrs = SingleMetadataVals.size();
      FixedVectorType *MetadataVectorTy =
          FixedVectorType::get(SingleMetadataVals.front()->getType(), NumPtrs);
      Value *MetadataVector = UndefValue::get(MetadataVectorTy);
      for (uint64_t J = 0; J < NumPtrs; J++)
      {
        Constant *Idx =
            ConstantInt::get(Type::getInt32Ty(InsertAt->getContext()), J);
        MetadataVector = InsertElementInst::Create(
            MetadataVector, SingleMetadataVals[J], Idx, "", InsertAt);
      }
      return MetadataVector;
    }
    TinyPtrVector<Value *>
    packMetadataArray(ArrayRef<Value *> SingleMetadataVals,
                      Type *Ty, Instruction *InsertAt)
    {
      TinyPtrVector<Value *> PackedMetadata;
      auto MetadataOrder = getMetadataOrder(Ty);
      size_t MetadataValsIdx = 0;

      for (auto &MetadataType : MetadataOrder)
      {
        if (std::get<0>(MetadataType) == 0)
        {
          auto *SingleMetadatum = SingleMetadataVals[MetadataValsIdx];
          PackedMetadata.push_back(SingleMetadatum);
          MetadataValsIdx++;
        }
        else if (std::get<0>(MetadataType) == 1)
        {
          auto VectorSize = std::get<1>(MetadataType);
          auto *MetadataVector = createMetadataVector(
              SingleMetadataVals.slice(MetadataValsIdx, VectorSize), InsertAt);
          PackedMetadata.push_back(MetadataVector);
          MetadataValsIdx += VectorSize;
        }
      }
      return PackedMetadata;
    }
    size_t introduceShadowStackLoads(Value *Val,
                                     Instruction *InsertAt,
                                     int ArgNo)
    {
      size_t NumPtrs = countPointersInType(Val->getType());
      if (!NumPtrs)
        return 0;

      IRBuilder<> IRB(InsertAt);

      Type *ValTy = Val->getType();

      TinyPtrVector<Value *> Bases;
      TinyPtrVector<Value *> Bounds;

      unsigned KeyIdx = 0;
      unsigned LockIdx = 1;

      SmallVector<Value *, 8> Args;

      for (unsigned PtrIdx = 0; PtrIdx < NumPtrs; ++PtrIdx)
      {
        errs() << "loading metadata from shadow stack\n";
        auto *MetadataPtr =
            IRB.CreateCall(LoadMetadataPtrShadowStackFn,
                           {ConstantInt::get(MArgNoTy, ArgNo + PtrIdx, false)},
                           "shadow_stack_metadata_ptr");
        llvm::Type *StructType = MetadataPtr->getType()->getPointerElementType();

        auto *BasePtr = IRB.CreateStructGEP(StructType, MetadataPtr, 0, "baseptr");
        auto *Base = IRB.CreateLoad(BasePtr->getType()->getPointerElementType(), BasePtr, "base");
        auto *BoundPtr = IRB.CreateStructGEP(StructType, MetadataPtr, 1, "boundptr");
        auto *Bound = IRB.CreateLoad(BoundPtr->getType()->getPointerElementType(), BoundPtr, "bound");
        Bases.push_back(Base);
        Bounds.push_back(Bound);
      }
      auto PackedBases = packMetadataArray(Bases, ValTy, InsertAt);
      auto PackedBounds = packMetadataArray(Bounds, ValTy, InsertAt);
      associateAggregateBaseBound(Val, PackedBases, PackedBounds);
      return NumPtrs;
    }
    void introduceShadowStackDeallocation(
        CallBase *CallInst, Instruction *InsertAt)
    {
      TinyPtrVector<Value *> Args;
      CallInst::Create(DeallocateShadowStackFn, Args, "", InsertAt);
    }
    bool isFunctionNotToInstrument(const StringRef &str)
    {
      // errs() << "function name is : " << str << "\n";
      if (str.contains("softboundcets"))
      {
        return true;
      }
      if (str.find("llvm.") == 0)
      {
        // errs() << "not to instrumenting " << str << "\n";
        return true;
      }
      return false;
    }
    bool isExternalDefinitelyInstrumentedFunction(
        const StringRef &Str)
    {
      return isFunctionNotToInstrument(Str) ||
             (MFunctionWrappersAvailable.count(Str) > 0);
    }
    void handle_call(Instruction &I)
    {
      CallInst *CI = dyn_cast<CallInst>(&I);
      Instruction *InsertAt = getNextInstruction(CI);
      // errs() << "handling call instruction : " << *CI << "\n";
      if (auto *F = CI->getCalledFunction())
      {
        auto FName = F->getName();
        if (F->isDeclaration() &&
            !isExternalDefinitelyInstrumentedFunction(FName))
        {
          if (isTypeWithPointers(CI->getType()))
          {
            // errs() << "Handling call to external uninstrumented function: "
            //        << FName << "\n";
          }
          return;
        }
        if (isFunctionNotToInstrument(FName))
          return;
      }
      auto *ReturnTy = CI->getType();
      size_t NumPointerArgs = getNumPointerArgs(CI);
      // errs() << "Argument num : " << NumPointerArgs << "\n";
      size_t NumPointerRets = countPointersInType(ReturnTy);
      if (NumPointerArgs || NumPointerRets)
      {
        introduceShadowStackAllocation(CI, NumPointerArgs + NumPointerRets);
      }

      if (NumPointerArgs)
      {
        size_t PointerArgNo = NumPointerRets;

        for (Use &Arg : CI->args())
        {
          if (isTypeWithPointers(Arg->getType()))
          {
            PointerArgNo += introduceShadowStackStores(Arg, CI, PointerArgNo);
          }
        }
      }

      if (NumPointerRets)
      {
        // Shadow stack slots for return values start at index 0.
        introduceShadowStackLoads(CI, InsertAt, 0);
      }

      if (NumPointerArgs || NumPointerRets)
      {
        introduceShadowStackDeallocation(CI, InsertAt);
      }
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

    void collect_metadata(Function &F)
    {
      int ArgCount = countPointersInType(F.getReturnType());

      for (auto &Arg : F.args())
      {

        auto *ArgTy = Arg.getType();

        if (!isTypeWithPointers(ArgTy))
        {
          continue;
        }

        Instruction *InsertAt = nullptr;

        if (!F.empty() && !F.begin()->empty())
        {
          InsertAt = F.begin()->getFirstNonPHI();
        }

        if (!InsertAt)
        {
          errs() << "Error: No valid non-PHI instruction found.\n";
          return;
        }

        if (Arg.hasByValAttr())
        {
          errs() << "hasbyvalattr\n";
        }
        else
        {
          ArgCount += introduceShadowStackLoads(&Arg, InsertAt, ArgCount);
        }
      }

      for (BasicBlock &BB : F)
      {
        for (Instruction &I : BB)
        {
          // errs() << "handling instruction : " << I << "\n";
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
            break;
          case Instruction::Call:
            handle_call(I);
            break;
          }
        }
      }
    }

    void insert_func(Function &F)
    {
      Value *base = NULL;
      Value *bound = NULL;
      Value *value_operand = NULL;
      Value *pointer_operand = NULL;
      Type *pointee_type = NULL;
      Value *access = NULL;
      IRBuilder<> *builder = NULL;
      for (BasicBlock &BB : F)
      {
        for (Instruction &I : BB)
        {
          // errs() << "instruction : " << I << "\n";
          SmallVector<Value *, 8> args;
          if (StoreInst *SI = dyn_cast<StoreInst>(&I))
          {
            value_operand = SI->getValueOperand();
            pointer_operand = SI->getPointerOperand();
            // if (isa<ConstantPointerNull>(pointer_operand))
            // {
            //   errs() << "returning...\n";
            //   return;
            // }
            pointee_type = SI->getType();
            builder = new IRBuilder<>(SI->getNextNode());
            access = castToVoidPtr(pointer_operand, *builder);
            if (!isTypeWithPointers(value_operand->getType()))
            {
              base = getAssociatedBase(pointer_operand);
              bound = getAssociatedBound(pointer_operand);
              // if ((base == MVoidNullPtr) && (bound == MInfiniteBoundPtr))
              // {
              //   errs() << "Base is : " << *base << "bound is : " << *bound << "\n";
              //   return;
              // }
              args.push_back(base);
              args.push_back(bound);
              args.push_back(access);
              // for (auto &arg : args)
              // {
              //   errs() << "argument of boundcheck : " << *arg << "\n";
              // }
              // errs() << "inserting boundcheck to " << *SI << "\n";

              builder->CreateCall(boundCheck, args);
            }
          }
          if (LoadInst *LI = dyn_cast<LoadInst>(&I))
          {
            pointer_operand = LI->getPointerOperand();
            // if (isa<ConstantPointerNull>(pointer_operand))
            // {
            //   errs() << "returning...\n";
            //   return;
            // }
            pointee_type = LI->getType();
            builder = new IRBuilder<>(LI->getNextNode());
            access = castToVoidPtr(pointer_operand, *builder);
            base = getAssociatedBase(pointer_operand);
            bound = getAssociatedBound(pointer_operand);
            // if ((base == MVoidNullPtr) && (bound == MInfiniteBoundPtr))
            // {
            //   errs() << "Base is : " << *base << "bound is : " << *bound << "\n";
            //   return;
            // }
            args.push_back(base);
            args.push_back(bound);
            args.push_back(access);
            // errs() << "inserting boundcheck to " << *LI << "\n";
            builder->CreateCall(boundCheck, args);
          }
        }
      }
    }
    std::string transformFunctionName(const std::string &str)
    {
      // If the function name starts with this prefix, don't just
      // concatenate, but instead transform the string
      errs() << "rename complete\n";
      return "_softboundcets_" + str;
    }
    void renameFunctionName(Function *F, Module &M,
                            bool External)
    {
      Type *RetTy = F->getReturnType();
      const FunctionType *FTy = F->getFunctionType();
      std::vector<Type *> Params;
      auto FName = F->getName();

      if (!MFunctionWrappersAvailable.count(FName))
      {
        errs() << "returing......................" << FName << "\n";
        return;
      }

      SmallVector<AttributeSet, 8> ParamAttrsVec;

      for (Argument &Arg : F->args())
      {
        Params.push_back(Arg.getType());
      }
      errs() << "renaming function : " << FName << "\n";
      // check if we have loaded the rt-lib bitcode into the module already
      auto *FunctionWrapper = M.getFunction(transformFunctionName(FName.str()));

      // if not, we create a function declaration which can be resolved during
      // linking
      if (!FunctionWrapper)
      {
        errs() << "Defining wrapper function : " << FName.str() << "\n";
        FunctionType *FWrapperTy =
            FunctionType::get(RetTy, Params, FTy->isVarArg());
        FunctionWrapper = Function::Create(FWrapperTy, F->getLinkage(),
                                           transformFunctionName(FName.str()));
        FunctionWrapper->copyAttributesFrom(F);
        F->getParent()->getFunctionList().push_back(FunctionWrapper);
      }

      // reimplement doRAUW for Constant Users of Function
      // where we want to replace the Function with the FunctionWrapper
      // NOTE: replaceUsesWithIf does not work for Constants
      for (auto UI = F->use_begin(), E = F->use_end(); UI != E;)
      {
        Use &U = *UI++;
        User *Usr = U.getUser();

        if (auto *I = dyn_cast<Instruction>(Usr))
        {
          // Instruction 사용자 처리
          errs() << "Replacing instruction use: " << *I << "\n";
          U.set(FunctionWrapper); // Replace directly for instructions
        }
        else if (auto *C = dyn_cast<ConstantExpr>(Usr))
        {
          // Constant 사용자 처리
          errs() << "Replacing constant expression use.\n";

          // 새로운 ConstantExpr 생성
          if (C->getOpcode() == Instruction::BitCast)
          {
            auto *NewBitCast = ConstantExpr::getBitCast(FunctionWrapper, C->getType());
            C->handleOperandChange(F, NewBitCast);
          }
          else
          {
            errs() << "Unhandled constant expression opcode: " << C->getOpcode() << "\n";
          }
        }
      }

      F->replaceUsesWithIf(FunctionWrapper, [this](Use &U) -> bool
                           {
    auto *Usr = U.getUser();
    auto *I = dyn_cast<Instruction>(Usr);
    if (I) {
      return InstrumentedFunctions.contains(I->getFunction());
    }
    return false; });
    }

    void renamefunctions(Module &module)
    {
      bool change = false;

      do
      {
        change = false;
        for (Module::iterator ff_begin = module.begin(), ff_end = module.end();
             ff_begin != ff_end; ++ff_begin)
        {

          Function *func_ptr = dyn_cast<Function>(ff_begin);

          if (m_func_transformed.count(func_ptr->getName()) ||
              isFunctionNotToInstrument(func_ptr->getName()))
          {
            // errs() << "not to change function name : " << func_ptr->getName() << "\n";
            continue;
          }

          m_func_transformed[func_ptr->getName()] = true;
          m_func_transformed[transformFunctionName(func_ptr->getName().str())] =
              true;
          bool is_external = func_ptr->isDeclaration();
          renameFunctionName(func_ptr, module, is_external);
          change = true;
          break;
        }
      } while (change);
    }

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM)
    {

      DL = &M.getDataLayout();
      C = &M.getContext();
      MVoidPtrTy = PointerType::getInt8PtrTy(M.getContext());
      MVoidNullPtr = ConstantPointerNull::get(MVoidPtrTy);
      size_t InfBound;
      MSizetTy = Type::getInt64Ty(M.getContext());
      MArgNoTy = MSizetTy;
      Constant *InfiniteBound = ConstantInt::get(MSizetTy, InfBound, false);
      StructType *MetadataStruct =
          StructType::getTypeByName(M.getContext(), "struct.Metadata");

      if (!MetadataStruct)
      {
        MetadataStruct = StructType::create(M.getContext(), "struct.Metadata");

        MetadataStruct->setBody({MVoidPtrTy, MVoidPtrTy});
      }
      Type *MetadataStructPtrTy = PointerType::getUnqual(MetadataStruct);
      MInfiniteBoundPtr = ConstantExpr::getIntToPtr(InfiniteBound, MVoidPtrTy);

      ClAssociateZeroInitializedGlobalsWithOmnivalidMetadata = true;
      ClAssociateIntToPtrCastsWithOmnivalidMetadata = true;
      ClAssociateVaargPointerWithOmnivalidMetadata = true;
      ClIntraObjectBounds = true;
      errs() << "**** DEBUG ****\n";
      setMetaData = M.getOrInsertFunction(
          "_softboundcets_set_metadata",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                                                              // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr, size_t size)
              false                                                                                                         // 가변 인자 여부: false
              ));
      printMetadata = M.getOrInsertFunction(
          "_softboundcets_print_metadata",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                          // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* base, void* bound)
              false                                                                     // 가변 인자 여부: false
              ));

      // bound_check 함수 선언 또는 삽입
      boundCheck = M.getOrInsertFunction(
          "_softboundcets_bound_check",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),                                                                              // 반환 타입: void
              {Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())}, // 인자: (void* ptr, void* ptr)
              false                                                                                                         // 가변 인자 여부: false
              ));

      // print_metadata_table 함수 선언 또는 삽입
      printMetadataTable = M.getOrInsertFunction(
          "_softboundcets_print_metadata_table",
          FunctionType::get(
              Type::getVoidTy(M.getContext()), // 반환 타입: void
              false                            // 인자 없음
              ));
      getBaseAddr = M.getOrInsertFunction(
          "__softboundcets_get_base_addr",
          FunctionType::get(
              Type::getInt8PtrTy(M.getContext()),
              {Type::getInt8PtrTy(M.getContext())},
              false));
      getBoundAddr = M.getOrInsertFunction(
          "__softboundcets_get_bound_addr",
          FunctionType::get(
              Type::getInt8PtrTy(M.getContext()),
              {Type::getInt8PtrTy(M.getContext())},
              false));
      SBinitTable = M.getOrInsertFunction(
          "_softboundcets_init_metadata_table",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),
              false));
      ASaninit = M.getOrInsertFunction(
          "_ASan_init",
          FunctionType::get(
              Type::getVoidTy(M.getContext()),
              false));
      StoreMetadataShadowStackFn =
          M.getOrInsertFunction("__softboundcets_store_metadata_shadow_stack",
                                Type::getVoidTy(M.getContext()),
                                Type::getInt8PtrTy(M.getContext()),
                                Type::getInt8PtrTy(M.getContext()),
                                Type::getInt64Ty(M.getContext()));
      AllocateShadowStackFn = M.getOrInsertFunction(
          "__softboundcets_allocate_shadow_stack_space", Type::getVoidTy(M.getContext()),
          Type::getInt64Ty(M.getContext()));
      DeallocateShadowStackFn = M.getOrInsertFunction(
          "__softboundcets_deallocate_shadow_stack_space", Type::getVoidTy(M.getContext()));
      LoadMetadataPtrShadowStackFn =
          M.getOrInsertFunction("__softboundcets_shadow_stack_metadata_ptr",
                                MetadataStructPtrTy, MSizetTy);
      LoadMetadataPtrFn = M.getOrInsertFunction(
          "__softboundcets_shadowspace_metadata_ptr", MetadataStructPtrTy, MVoidPtrTy);

      Function *CtorFunc = Function::Create(
          FunctionType::get(Type::getVoidTy(M.getContext()), false), // 함수 타입: void()
          GlobalValue::InternalLinkage, "__global_init", &M);        // 함수 이름 및 링키지

      BasicBlock *BB = BasicBlock::Create(M.getContext(), "entry", CtorFunc);

      // IRBuilder 생성
      IRBuilder<> IRB(BB);
      // softbound_init_func
      IRB.CreateCall(SBinitTable);
      // ASan init
      IRB.CreateCall(ASaninit);
      // 함수 종료 코드 추가
      IRB.CreateRetVoid();

      // 기본 블록 생성

      // _init_metadata_table 호출 삽입

      // __global_init을 전역 생성자에 등록
      appendToGlobalCtors(M, CtorFunc, 0, nullptr);
      for (Function &F : M)
      {
        errs() << F.getName() << "\n";
        collect_metadata(F);
        insert_func(F);
      }
      renamefunctions(M);
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