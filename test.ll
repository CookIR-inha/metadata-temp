; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !13 {
  %1 = alloca [10 x i32], align 16
  %2 = alloca i8*, align 8
  %3 = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata [10 x i32]* %1, metadata !17, metadata !DIExpression()), !dbg !21
  %4 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 11, !dbg !22
  store i32 1, i32* %4, align 4, !dbg !23
  call void @llvm.dbg.declare(metadata i8** %2, metadata !24, metadata !DIExpression()), !dbg !26
  %5 = bitcast [10 x i32]* %1 to i8*, !dbg !27
  store i8* %5, i8** %2, align 8, !dbg !26
  call void @llvm.dbg.declare(metadata i8** %3, metadata !28, metadata !DIExpression()), !dbg !29
  %6 = load i8*, i8** %2, align 8, !dbg !30
  store i8* %6, i8** %3, align 8, !dbg !29
  %7 = load i8*, i8** %3, align 8, !dbg !31
  %8 = bitcast i8* %7 to i32*, !dbg !32
  %9 = getelementptr inbounds i32, i32* %8, i64 12, !dbg !33
  store i32 1, i32* %9, align 4, !dbg !34
  ret i32 0, !dbg !35
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!5, !6, !7, !8, !9, !10, !11}
!llvm.ident = !{!12}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "31d38a3357455413ad983d4702445d77")
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
!13 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 8, type: !14, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !16)
!14 = !DISubroutineType(types: !15)
!15 = !{!4}
!16 = !{}
!17 = !DILocalVariable(name: "arr", scope: !13, file: !1, line: 9, type: !18)
!18 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 320, elements: !19)
!19 = !{!20}
!20 = !DISubrange(count: 10)
!21 = !DILocation(line: 9, column: 6, scope: !13)
!22 = !DILocation(line: 10, column: 2, scope: !13)
!23 = !DILocation(line: 10, column: 10, scope: !13)
!24 = !DILocalVariable(name: "ptr", scope: !13, file: !1, line: 12, type: !25)
!25 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!26 = !DILocation(line: 12, column: 8, scope: !13)
!27 = !DILocation(line: 12, column: 14, scope: !13)
!28 = !DILocalVariable(name: "ptr2", scope: !13, file: !1, line: 13, type: !25)
!29 = !DILocation(line: 13, column: 8, scope: !13)
!30 = !DILocation(line: 13, column: 15, scope: !13)
!31 = !DILocation(line: 14, column: 11, scope: !13)
!32 = !DILocation(line: 14, column: 4, scope: !13)
!33 = !DILocation(line: 14, column: 16, scope: !13)
!34 = !DILocation(line: 14, column: 22, scope: !13)
!35 = !DILocation(line: 15, column: 1, scope: !13)
