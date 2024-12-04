; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @func(i32* noundef %0) #0 !dbg !13 {
  %2 = alloca i32*, align 8
  %3 = alloca i8*, align 8
  store i32* %0, i32** %2, align 8
  call void @llvm.dbg.declare(metadata i32** %2, metadata !17, metadata !DIExpression()), !dbg !18
  call void @llvm.dbg.declare(metadata i8** %3, metadata !19, metadata !DIExpression()), !dbg !21
  %4 = load i32*, i32** %2, align 8, !dbg !22
  %5 = bitcast i32* %4 to i8*, !dbg !22
  store i8* %5, i8** %3, align 8, !dbg !21
  %6 = load i8*, i8** %3, align 8, !dbg !23
  %7 = bitcast i8* %6 to i32*, !dbg !24
  %8 = getelementptr inbounds i32, i32* %7, i64 12, !dbg !25
  store i32 255, i32* %8, align 4, !dbg !26
  ret void, !dbg !27
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !28 {
  %1 = alloca [10 x i32], align 16
  %2 = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata [10 x i32]* %1, metadata !31, metadata !DIExpression()), !dbg !35
  call void @llvm.dbg.declare(metadata i8** %2, metadata !36, metadata !DIExpression()), !dbg !37
  %3 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 0, !dbg !38
  %4 = bitcast i32* %3 to i8*, !dbg !38
  store i8* %4, i8** %2, align 8, !dbg !37
  %5 = load i8*, i8** %2, align 8, !dbg !39
  %6 = bitcast i8* %5 to i32*, !dbg !40
  %7 = getelementptr inbounds i32, i32* %6, i64 11, !dbg !41
  store i32 1, i32* %7, align 4, !dbg !42
  %8 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 0, !dbg !43
  call void @func(i32* noundef %8), !dbg !44
  ret i32 0, !dbg !45
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!5, !6, !7, !8, !9, !10, !11}
!llvm.ident = !{!12}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "cb5aeb822bf897fd46e1742c89f9dfa7")
!2 = !{!3}
!3 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!4 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!5 = !{i32 7, !"Dwarf Version", i32 5}
!6 = !{i32 2, !"Debug Info Version", i32 3}
!7 = !{i32 1, !"wchar_size", i32 4}
!8 = !{i32 7, !"PIC Level", i32 2}
!9 = !{i32 7, !"PIE Level", i32 2}
!10 = !{i32 7, !"uwtable", i32 1}
!11 = !{i32 7, !"frame-pointer", i32 2}
!12 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!13 = distinct !DISubprogram(name: "func", scope: !1, file: !1, line: 3, type: !14, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !16)
!14 = !DISubroutineType(types: !15)
!15 = !{null, !3}
!16 = !{}
!17 = !DILocalVariable(name: "arr", arg: 1, scope: !13, file: !1, line: 3, type: !3)
!18 = !DILocation(line: 3, column: 16, scope: !13)
!19 = !DILocalVariable(name: "test", scope: !13, file: !1, line: 4, type: !20)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!21 = !DILocation(line: 4, column: 8, scope: !13)
!22 = !DILocation(line: 4, column: 15, scope: !13)
!23 = !DILocation(line: 5, column: 11, scope: !13)
!24 = !DILocation(line: 5, column: 4, scope: !13)
!25 = !DILocation(line: 5, column: 16, scope: !13)
!26 = !DILocation(line: 5, column: 21, scope: !13)
!27 = !DILocation(line: 6, column: 1, scope: !13)
!28 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 8, type: !29, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !16)
!29 = !DISubroutineType(types: !30)
!30 = !{!4}
!31 = !DILocalVariable(name: "arr", scope: !28, file: !1, line: 9, type: !32)
!32 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 320, elements: !33)
!33 = !{!34}
!34 = !DISubrange(count: 10)
!35 = !DILocation(line: 9, column: 6, scope: !28)
!36 = !DILocalVariable(name: "arr_copy", scope: !28, file: !1, line: 10, type: !20)
!37 = !DILocation(line: 10, column: 8, scope: !28)
!38 = !DILocation(line: 10, column: 19, scope: !28)
!39 = !DILocation(line: 11, column: 11, scope: !28)
!40 = !DILocation(line: 11, column: 4, scope: !28)
!41 = !DILocation(line: 11, column: 20, scope: !28)
!42 = !DILocation(line: 11, column: 26, scope: !28)
!43 = !DILocation(line: 13, column: 7, scope: !28)
!44 = !DILocation(line: 13, column: 2, scope: !28)
!45 = !DILocation(line: 14, column: 1, scope: !28)
