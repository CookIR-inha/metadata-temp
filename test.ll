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
@.str.8 = private unnamed_addr constant [4 x i8] c"5.\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !14 {
  %1 = alloca i32, align 4
  %2 = alloca i32*, align 8
  %3 = alloca i32*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32*, align 8
  store i32 0, i32* %1, align 4
  %7 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0)), !dbg !18
  call void @llvm.dbg.declare(metadata i32** %2, metadata !19, metadata !DIExpression()), !dbg !20
  %8 = call noalias i8* @malloc(i64 noundef 40) #4, !dbg !21
  %9 = bitcast i8* %8 to i32*, !dbg !22
  store i32* %9, i32** %2, align 8, !dbg !20
  call void @llvm.dbg.declare(metadata i32** %3, metadata !23, metadata !DIExpression()), !dbg !24
  %10 = call noalias i8* @malloc(i64 noundef 40) #4, !dbg !25
  %11 = bitcast i8* %10 to i32*, !dbg !26
  store i32* %11, i32** %3, align 8, !dbg !24
  %12 = load i32*, i32** %2, align 8, !dbg !27
  %13 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1, i64 0, i64 0), i32* noundef %12), !dbg !28
  %14 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i64 0, i64 0)), !dbg !29
  call void @llvm.dbg.declare(metadata i32* %4, metadata !30, metadata !DIExpression()), !dbg !32
  store i32 0, i32* %4, align 4, !dbg !32
  br label %15, !dbg !33

15:                                               ; preds = %31, %0
  %16 = load i32, i32* %4, align 4, !dbg !34
  %17 = icmp slt i32 %16, 10, !dbg !36
  br i1 %17, label %18, label %34, !dbg !37

18:                                               ; preds = %15
  %19 = load i32, i32* %4, align 4, !dbg !38
  %20 = mul nsw i32 %19, 10, !dbg !40
  %21 = load i32*, i32** %2, align 8, !dbg !41
  %22 = load i32, i32* %4, align 4, !dbg !42
  %23 = sext i32 %22 to i64, !dbg !41
  %24 = getelementptr inbounds i32, i32* %21, i64 %23, !dbg !41
  store i32 %20, i32* %24, align 4, !dbg !43
  %25 = load i32*, i32** %2, align 8, !dbg !44
  %26 = load i32, i32* %4, align 4, !dbg !45
  %27 = sext i32 %26 to i64, !dbg !44
  %28 = getelementptr inbounds i32, i32* %25, i64 %27, !dbg !44
  %29 = bitcast i32* %28 to i8*, !dbg !46
  %30 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([13 x i8], [13 x i8]* @.str.3, i64 0, i64 0), i8* noundef %29), !dbg !47
  br label %31, !dbg !48

31:                                               ; preds = %18
  %32 = load i32, i32* %4, align 4, !dbg !49
  %33 = add nsw i32 %32, 1, !dbg !49
  store i32 %33, i32* %4, align 4, !dbg !49
  br label %15, !dbg !50, !llvm.loop !51

34:                                               ; preds = %15
  %35 = load i32*, i32** %2, align 8, !dbg !54
  %36 = getelementptr inbounds i32, i32* %35, i64 5, !dbg !54
  %37 = load i32, i32* %36, align 4, !dbg !54
  %38 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([22 x i8], [22 x i8]* @.str.4, i64 0, i64 0), i32 noundef %37), !dbg !55
  %39 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([28 x i8], [28 x i8]* @.str.5, i64 0, i64 0)), !dbg !56
  %40 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([15 x i8], [15 x i8]* @.str.6, i64 0, i64 0)), !dbg !57
  call void @llvm.dbg.declare(metadata i32* %5, metadata !58, metadata !DIExpression()), !dbg !59
  store i32 68, i32* %5, align 4, !dbg !59
  %41 = call i32 (i8*, ...) @__isoc99_scanf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.7, i64 0, i64 0), i32* noundef %5), !dbg !60
  %42 = load i32*, i32** %2, align 8, !dbg !61
  %43 = load i32, i32* %5, align 4, !dbg !62
  %44 = sext i32 %43 to i64, !dbg !61
  %45 = getelementptr inbounds i32, i32* %42, i64 %44, !dbg !61
  store i32 100, i32* %45, align 4, !dbg !63
  %46 = load i32*, i32** %2, align 8, !dbg !64
  %47 = bitcast i32* %46 to i8*, !dbg !64
  call void @free(i8* noundef %47) #4, !dbg !65
  %48 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.str.8, i64 0, i64 0)), !dbg !66
  call void @llvm.dbg.declare(metadata i32** %6, metadata !67, metadata !DIExpression()), !dbg !68
  %49 = call noalias i8* @malloc(i64 noundef 4) #4, !dbg !69
  %50 = bitcast i8* %49 to i32*, !dbg !70
  store i32* %50, i32** %6, align 8, !dbg !68
  %51 = load i32*, i32** %6, align 8, !dbg !71
  %52 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([13 x i8], [13 x i8]* @.str.3, i64 0, i64 0), i32* noundef %51), !dbg !72
  %53 = load i32*, i32** %6, align 8, !dbg !73
  store i32 20, i32* %53, align 4, !dbg !74
  %54 = load i32*, i32** %6, align 8, !dbg !75
  %55 = getelementptr inbounds i32, i32* %54, i64 1, !dbg !76
  store i32 10, i32* %55, align 4, !dbg !77
  ret i32 0, !dbg !78
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

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8, !9, !10, !11, !12}
!llvm.ident = !{!13}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "f6282595e1841b939889525198bdb6ff")
!2 = !{!3, !5}
!3 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!4 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!5 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!6 = !{i32 7, !"Dwarf Version", i32 5}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{i32 7, !"PIC Level", i32 2}
!10 = !{i32 7, !"PIE Level", i32 2}
!11 = !{i32 7, !"uwtable", i32 1}
!12 = !{i32 7, !"frame-pointer", i32 2}
!13 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!14 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 5, type: !15, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !17)
!15 = !DISubroutineType(types: !16)
!16 = !{!4}
!17 = !{}
!18 = !DILocation(line: 7, column: 3, scope: !14)
!19 = !DILocalVariable(name: "arr", scope: !14, file: !1, line: 8, type: !3)
!20 = !DILocation(line: 8, column: 8, scope: !14)
!21 = !DILocation(line: 8, column: 21, scope: !14)
!22 = !DILocation(line: 8, column: 14, scope: !14)
!23 = !DILocalVariable(name: "arr2", scope: !14, file: !1, line: 9, type: !3)
!24 = !DILocation(line: 9, column: 8, scope: !14)
!25 = !DILocation(line: 9, column: 22, scope: !14)
!26 = !DILocation(line: 9, column: 15, scope: !14)
!27 = !DILocation(line: 10, column: 40, scope: !14)
!28 = !DILocation(line: 10, column: 3, scope: !14)
!29 = !DILocation(line: 12, column: 3, scope: !14)
!30 = !DILocalVariable(name: "i", scope: !31, file: !1, line: 13, type: !4)
!31 = distinct !DILexicalBlock(scope: !14, file: !1, line: 13, column: 3)
!32 = !DILocation(line: 13, column: 12, scope: !31)
!33 = !DILocation(line: 13, column: 8, scope: !31)
!34 = !DILocation(line: 13, column: 19, scope: !35)
!35 = distinct !DILexicalBlock(scope: !31, file: !1, line: 13, column: 3)
!36 = !DILocation(line: 13, column: 21, scope: !35)
!37 = !DILocation(line: 13, column: 3, scope: !31)
!38 = !DILocation(line: 15, column: 14, scope: !39)
!39 = distinct !DILexicalBlock(scope: !35, file: !1, line: 14, column: 3)
!40 = !DILocation(line: 15, column: 16, scope: !39)
!41 = !DILocation(line: 15, column: 5, scope: !39)
!42 = !DILocation(line: 15, column: 9, scope: !39)
!43 = !DILocation(line: 15, column: 12, scope: !39)
!44 = !DILocation(line: 16, column: 38, scope: !39)
!45 = !DILocation(line: 16, column: 42, scope: !39)
!46 = !DILocation(line: 16, column: 29, scope: !39)
!47 = !DILocation(line: 16, column: 5, scope: !39)
!48 = !DILocation(line: 17, column: 3, scope: !39)
!49 = !DILocation(line: 13, column: 28, scope: !35)
!50 = !DILocation(line: 13, column: 3, scope: !35)
!51 = distinct !{!51, !37, !52, !53}
!52 = !DILocation(line: 17, column: 3, scope: !31)
!53 = !{!"llvm.loop.mustprogress"}
!54 = !DILocation(line: 19, column: 36, scope: !14)
!55 = !DILocation(line: 19, column: 3, scope: !14)
!56 = !DILocation(line: 21, column: 3, scope: !14)
!57 = !DILocation(line: 23, column: 3, scope: !14)
!58 = !DILocalVariable(name: "index", scope: !14, file: !1, line: 24, type: !4)
!59 = !DILocation(line: 24, column: 7, scope: !14)
!60 = !DILocation(line: 25, column: 3, scope: !14)
!61 = !DILocation(line: 26, column: 3, scope: !14)
!62 = !DILocation(line: 26, column: 7, scope: !14)
!63 = !DILocation(line: 26, column: 14, scope: !14)
!64 = !DILocation(line: 28, column: 8, scope: !14)
!65 = !DILocation(line: 28, column: 3, scope: !14)
!66 = !DILocation(line: 30, column: 3, scope: !14)
!67 = !DILocalVariable(name: "pi", scope: !14, file: !1, line: 31, type: !3)
!68 = !DILocation(line: 31, column: 8, scope: !14)
!69 = !DILocation(line: 31, column: 20, scope: !14)
!70 = !DILocation(line: 31, column: 13, scope: !14)
!71 = !DILocation(line: 32, column: 27, scope: !14)
!72 = !DILocation(line: 32, column: 3, scope: !14)
!73 = !DILocation(line: 33, column: 4, scope: !14)
!74 = !DILocation(line: 33, column: 7, scope: !14)
!75 = !DILocation(line: 34, column: 5, scope: !14)
!76 = !DILocation(line: 34, column: 8, scope: !14)
!77 = !DILocation(line: 34, column: 13, scope: !14)
!78 = !DILocation(line: 36, column: 3, scope: !14)
