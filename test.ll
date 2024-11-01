; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [12 x i8] c"1. malloc.\0A\00", align 1
@.str.1 = private unnamed_addr constant [26 x i8] c"2. Allocated pointer: %p\0A\00", align 1
@.str.2 = private unnamed_addr constant [31 x i8] c"3. Accessing allocated memory\0A\00", align 1
@.str.3 = private unnamed_addr constant [13 x i8] c"pointer: %p\0A\00", align 1
@.str.4 = private unnamed_addr constant [22 x i8] c"Accessing arr[5]: %d\0A\00", align 1
@.str.5 = private unnamed_addr constant [28 x i8] c"4. Accessing out-of-bound.\0A\00", align 1
@.str.6 = private unnamed_addr constant [15 x i8] c"input index : \00", align 1
@.str.7 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@arr_global = dso_local global [100 x i32] zeroinitializer, align 16, !dbg !0
@.str.8 = private unnamed_addr constant [4 x i8] c"5.\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !20 {
  %1 = alloca i32, align 4
  %2 = alloca [100 x i32], align 16
  %3 = alloca i32*, align 8
  %4 = alloca i32*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32*, align 8
  store i32 0, i32* %1, align 4
  %8 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0)), !dbg !24
  call void @llvm.dbg.declare(metadata [100 x i32]* %2, metadata !25, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.declare(metadata i32** %3, metadata !27, metadata !DIExpression()), !dbg !28
  %9 = call noalias i8* @malloc(i64 noundef 40) #4, !dbg !29
  %10 = bitcast i8* %9 to i32*, !dbg !30
  store i32* %10, i32** %3, align 8, !dbg !28
  call void @llvm.dbg.declare(metadata i32** %4, metadata !31, metadata !DIExpression()), !dbg !32
  %11 = call noalias i8* @malloc(i64 noundef 40) #4, !dbg !33
  %12 = bitcast i8* %11 to i32*, !dbg !34
  store i32* %12, i32** %4, align 8, !dbg !32
  %13 = load i32*, i32** %3, align 8, !dbg !35
  %14 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1, i64 0, i64 0), i32* noundef %13), !dbg !36
  %15 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i64 0, i64 0)), !dbg !37
  call void @llvm.dbg.declare(metadata i32* %5, metadata !38, metadata !DIExpression()), !dbg !40
  store i32 0, i32* %5, align 4, !dbg !40
  br label %16, !dbg !41

16:                                               ; preds = %32, %0
  %17 = load i32, i32* %5, align 4, !dbg !42
  %18 = icmp slt i32 %17, 10, !dbg !44
  br i1 %18, label %19, label %35, !dbg !45

19:                                               ; preds = %16
  %20 = load i32, i32* %5, align 4, !dbg !46
  %21 = mul nsw i32 %20, 10, !dbg !48
  %22 = load i32*, i32** %3, align 8, !dbg !49
  %23 = load i32, i32* %5, align 4, !dbg !50
  %24 = sext i32 %23 to i64, !dbg !49
  %25 = getelementptr inbounds i32, i32* %22, i64 %24, !dbg !49
  store i32 %21, i32* %25, align 4, !dbg !51
  %26 = load i32*, i32** %3, align 8, !dbg !52
  %27 = load i32, i32* %5, align 4, !dbg !53
  %28 = sext i32 %27 to i64, !dbg !52
  %29 = getelementptr inbounds i32, i32* %26, i64 %28, !dbg !52
  %30 = bitcast i32* %29 to i8*, !dbg !54
  %31 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([13 x i8], [13 x i8]* @.str.3, i64 0, i64 0), i8* noundef %30), !dbg !55
  br label %32, !dbg !56

32:                                               ; preds = %19
  %33 = load i32, i32* %5, align 4, !dbg !57
  %34 = add nsw i32 %33, 1, !dbg !57
  store i32 %34, i32* %5, align 4, !dbg !57
  br label %16, !dbg !58, !llvm.loop !59

35:                                               ; preds = %16
  %36 = load i32*, i32** %3, align 8, !dbg !62
  %37 = getelementptr inbounds i32, i32* %36, i64 5, !dbg !62
  %38 = load i32, i32* %37, align 4, !dbg !62
  %39 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([22 x i8], [22 x i8]* @.str.4, i64 0, i64 0), i32 noundef %38), !dbg !63
  %40 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([28 x i8], [28 x i8]* @.str.5, i64 0, i64 0)), !dbg !64
  %41 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([15 x i8], [15 x i8]* @.str.6, i64 0, i64 0)), !dbg !65
  call void @llvm.dbg.declare(metadata i32* %6, metadata !66, metadata !DIExpression()), !dbg !67
  store i32 68, i32* %6, align 4, !dbg !67
  %42 = call i32 (i8*, ...) @__isoc99_scanf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.7, i64 0, i64 0), i32* noundef %6), !dbg !68
  %43 = load i32*, i32** %3, align 8, !dbg !69
  %44 = load i32, i32* %6, align 4, !dbg !70
  %45 = sext i32 %44 to i64, !dbg !69
  %46 = getelementptr inbounds i32, i32* %43, i64 %45, !dbg !69
  store i32 100, i32* %46, align 4, !dbg !71
  store i32 100, i32* getelementptr inbounds ([100 x i32], [100 x i32]* @arr_global, i64 0, i64 36), align 16, !dbg !72
  %47 = load i32*, i32** %3, align 8, !dbg !73
  %48 = bitcast i32* %47 to i8*, !dbg !73
  call void @free(i8* noundef %48) #4, !dbg !74
  %49 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.8, i64 0, i64 0)), !dbg !75
  call void @llvm.dbg.declare(metadata i32** %7, metadata !76, metadata !DIExpression()), !dbg !77
  %50 = call noalias i8* @malloc(i64 noundef 4) #4, !dbg !78
  %51 = bitcast i8* %50 to i32*, !dbg !79
  store i32* %51, i32** %7, align 8, !dbg !77
  %52 = load i32*, i32** %7, align 8, !dbg !80
  %53 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([13 x i8], [13 x i8]* @.str.3, i64 0, i64 0), i32* noundef %52), !dbg !81
  %54 = load i32*, i32** %7, align 8, !dbg !82
  store i32 20, i32* %54, align 4, !dbg !83
  %55 = load i32*, i32** %7, align 8, !dbg !84
  %56 = getelementptr inbounds i32, i32* %55, i64 1, !dbg !85
  store i32 10, i32* %56, align 4, !dbg !86
  ret i32 0, !dbg !87
}

declare i32 @printf(i8* noundef, ...) #1

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #2

; Function Attrs: nounwind
declare noalias i8* @malloc(i64 noundef) #3

declare i32 @__isoc99_scanf(i8* noundef, ...) #1

; Function Attrs: nounwind
declare void @free(i8* noundef) #3

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!12, !13, !14, !15, !16, !17, !18}
!llvm.ident = !{!19}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "arr_global", scope: !2, file: !3, line: 5, type: !9, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !4, globals: !8, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "69c9e29c4e1cdcde5b90e6815de1ad42")
!4 = !{!5, !7}
!5 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !6, size: 64)
!6 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!7 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!8 = !{!0}
!9 = !DICompositeType(tag: DW_TAG_array_type, baseType: !6, size: 3200, elements: !10)
!10 = !{!11}
!11 = !DISubrange(count: 100)
!12 = !{i32 7, !"Dwarf Version", i32 5}
!13 = !{i32 2, !"Debug Info Version", i32 3}
!14 = !{i32 1, !"wchar_size", i32 4}
!15 = !{i32 7, !"PIC Level", i32 2}
!16 = !{i32 7, !"PIE Level", i32 2}
!17 = !{i32 7, !"uwtable", i32 1}
!18 = !{i32 7, !"frame-pointer", i32 2}
!19 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!20 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 7, type: !21, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !23)
!21 = !DISubroutineType(types: !22)
!22 = !{!6}
!23 = !{}
!24 = !DILocation(line: 9, column: 3, scope: !20)
!25 = !DILocalVariable(name: "arr3", scope: !20, file: !3, line: 10, type: !9)
!26 = !DILocation(line: 10, column: 7, scope: !20)
!27 = !DILocalVariable(name: "arr", scope: !20, file: !3, line: 11, type: !5)
!28 = !DILocation(line: 11, column: 8, scope: !20)
!29 = !DILocation(line: 11, column: 21, scope: !20)
!30 = !DILocation(line: 11, column: 14, scope: !20)
!31 = !DILocalVariable(name: "arr2", scope: !20, file: !3, line: 12, type: !5)
!32 = !DILocation(line: 12, column: 8, scope: !20)
!33 = !DILocation(line: 12, column: 22, scope: !20)
!34 = !DILocation(line: 12, column: 15, scope: !20)
!35 = !DILocation(line: 13, column: 40, scope: !20)
!36 = !DILocation(line: 13, column: 3, scope: !20)
!37 = !DILocation(line: 15, column: 3, scope: !20)
!38 = !DILocalVariable(name: "i", scope: !39, file: !3, line: 16, type: !6)
!39 = distinct !DILexicalBlock(scope: !20, file: !3, line: 16, column: 3)
!40 = !DILocation(line: 16, column: 12, scope: !39)
!41 = !DILocation(line: 16, column: 8, scope: !39)
!42 = !DILocation(line: 16, column: 19, scope: !43)
!43 = distinct !DILexicalBlock(scope: !39, file: !3, line: 16, column: 3)
!44 = !DILocation(line: 16, column: 21, scope: !43)
!45 = !DILocation(line: 16, column: 3, scope: !39)
!46 = !DILocation(line: 18, column: 14, scope: !47)
!47 = distinct !DILexicalBlock(scope: !43, file: !3, line: 17, column: 3)
!48 = !DILocation(line: 18, column: 16, scope: !47)
!49 = !DILocation(line: 18, column: 5, scope: !47)
!50 = !DILocation(line: 18, column: 9, scope: !47)
!51 = !DILocation(line: 18, column: 12, scope: !47)
!52 = !DILocation(line: 19, column: 38, scope: !47)
!53 = !DILocation(line: 19, column: 42, scope: !47)
!54 = !DILocation(line: 19, column: 29, scope: !47)
!55 = !DILocation(line: 19, column: 5, scope: !47)
!56 = !DILocation(line: 20, column: 3, scope: !47)
!57 = !DILocation(line: 16, column: 28, scope: !43)
!58 = !DILocation(line: 16, column: 3, scope: !43)
!59 = distinct !{!59, !45, !60, !61}
!60 = !DILocation(line: 20, column: 3, scope: !39)
!61 = !{!"llvm.loop.mustprogress"}
!62 = !DILocation(line: 22, column: 36, scope: !20)
!63 = !DILocation(line: 22, column: 3, scope: !20)
!64 = !DILocation(line: 24, column: 3, scope: !20)
!65 = !DILocation(line: 26, column: 3, scope: !20)
!66 = !DILocalVariable(name: "index", scope: !20, file: !3, line: 27, type: !6)
!67 = !DILocation(line: 27, column: 7, scope: !20)
!68 = !DILocation(line: 28, column: 3, scope: !20)
!69 = !DILocation(line: 29, column: 3, scope: !20)
!70 = !DILocation(line: 29, column: 7, scope: !20)
!71 = !DILocation(line: 29, column: 14, scope: !20)
!72 = !DILocation(line: 30, column: 18, scope: !20)
!73 = !DILocation(line: 31, column: 8, scope: !20)
!74 = !DILocation(line: 31, column: 3, scope: !20)
!75 = !DILocation(line: 33, column: 3, scope: !20)
!76 = !DILocalVariable(name: "pi", scope: !20, file: !3, line: 34, type: !5)
!77 = !DILocation(line: 34, column: 8, scope: !20)
!78 = !DILocation(line: 34, column: 20, scope: !20)
!79 = !DILocation(line: 34, column: 13, scope: !20)
!80 = !DILocation(line: 35, column: 27, scope: !20)
!81 = !DILocation(line: 35, column: 3, scope: !20)
!82 = !DILocation(line: 36, column: 4, scope: !20)
!83 = !DILocation(line: 36, column: 7, scope: !20)
!84 = !DILocation(line: 37, column: 5, scope: !20)
!85 = !DILocation(line: 37, column: 8, scope: !20)
!86 = !DILocation(line: 37, column: 13, scope: !20)
!87 = !DILocation(line: 39, column: 3, scope: !20)
