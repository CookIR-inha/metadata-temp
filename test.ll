; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !10 {
  %1 = alloca [10 x i32], align 16
  %2 = alloca i32*, align 8
  call void @llvm.dbg.declare(metadata [10 x i32]* %1, metadata !15, metadata !DIExpression()), !dbg !19
  %3 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 33, !dbg !20
  store i32 1, i32* %3, align 4, !dbg !21
  call void @llvm.dbg.declare(metadata i32** %2, metadata !22, metadata !DIExpression()), !dbg !24
  %4 = call i8* @malloc(i64 noundef 16), !dbg !25
  %5 = bitcast i8* %4 to i32*, !dbg !25
  store i32* %5, i32** %2, align 8, !dbg !24
  %6 = load i32*, i32** %2, align 8, !dbg !26
  %7 = getelementptr inbounds i32, i32* %6, i64 4, !dbg !26
  store i32 1, i32* %7, align 4, !dbg !27
  ret i32 0, !dbg !28
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare i8* @malloc(i64 noundef) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "73bf426d4f0ef1f432f279315214a62e")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 1}
!8 = !{i32 7, !"frame-pointer", i32 2}
!9 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 8, type: !11, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !14)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !DILocalVariable(name: "arr", scope: !10, file: !1, line: 9, type: !16)
!16 = !DICompositeType(tag: DW_TAG_array_type, baseType: !13, size: 320, elements: !17)
!17 = !{!18}
!18 = !DISubrange(count: 10)
!19 = !DILocation(line: 9, column: 6, scope: !10)
!20 = !DILocation(line: 10, column: 2, scope: !10)
!21 = !DILocation(line: 10, column: 10, scope: !10)
!22 = !DILocalVariable(name: "arr2", scope: !10, file: !1, line: 11, type: !23)
!23 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!24 = !DILocation(line: 11, column: 7, scope: !10)
!25 = !DILocation(line: 11, column: 14, scope: !10)
!26 = !DILocation(line: 12, column: 2, scope: !10)
!27 = !DILocation(line: 12, column: 10, scope: !10)
!28 = !DILocation(line: 14, column: 1, scope: !10)
