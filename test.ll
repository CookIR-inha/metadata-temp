; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @func(i32 noundef %0) #0 !dbg !10 {
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 8
  %4 = alloca i64, align 8
  store i32 %0, i32* %2, align 4
  call void @llvm.dbg.declare(metadata i32* %2, metadata !15, metadata !DIExpression()), !dbg !16
  %5 = load i32, i32* %2, align 4, !dbg !17
  %6 = zext i32 %5 to i64, !dbg !18
  %7 = call i8* @llvm.stacksave(), !dbg !18
  store i8* %7, i8** %3, align 8, !dbg !18
  %8 = alloca i32, i64 %6, align 16, !dbg !18
  store i64 %6, i64* %4, align 8, !dbg !18
  call void @llvm.dbg.declare(metadata i64* %4, metadata !19, metadata !DIExpression()), !dbg !21
  call void @llvm.dbg.declare(metadata i32* %8, metadata !22, metadata !DIExpression()), !dbg !26
  %9 = load i8*, i8** %3, align 8, !dbg !27
  call void @llvm.stackrestore(i8* %9), !dbg !27
  ret void, !dbg !27
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nofree nosync nounwind willreturn
declare i8* @llvm.stacksave() #2

; Function Attrs: nofree nosync nounwind willreturn
declare void @llvm.stackrestore(i8*) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !28 {
  %1 = alloca [256 x i32], align 16
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata [256 x i32]* %1, metadata !31, metadata !DIExpression()), !dbg !35
  call void @llvm.dbg.declare(metadata i32* %2, metadata !36, metadata !DIExpression()), !dbg !37
  store i32 16, i32* %2, align 4, !dbg !37
  %3 = load i32, i32* %2, align 4, !dbg !38
  call void @func(i32 noundef %3), !dbg !39
  ret i32 0, !dbg !40
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { nofree nosync nounwind willreturn }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "f4fa76b064c60f15c0f870eac4745540")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 1}
!8 = !{i32 7, !"frame-pointer", i32 2}
!9 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!10 = distinct !DISubprogram(name: "func", scope: !1, file: !1, line: 3, type: !11, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !14)
!11 = !DISubroutineType(types: !12)
!12 = !{null, !13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !DILocalVariable(name: "n", arg: 1, scope: !10, file: !1, line: 3, type: !13)
!16 = !DILocation(line: 3, column: 15, scope: !10)
!17 = !DILocation(line: 4, column: 10, scope: !10)
!18 = !DILocation(line: 4, column: 2, scope: !10)
!19 = !DILocalVariable(name: "__vla_expr0", scope: !10, type: !20, flags: DIFlagArtificial)
!20 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!21 = !DILocation(line: 0, scope: !10)
!22 = !DILocalVariable(name: "arr", scope: !10, file: !1, line: 4, type: !23)
!23 = !DICompositeType(tag: DW_TAG_array_type, baseType: !13, elements: !24)
!24 = !{!25}
!25 = !DISubrange(count: !19)
!26 = !DILocation(line: 4, column: 6, scope: !10)
!27 = !DILocation(line: 5, column: 1, scope: !10)
!28 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 7, type: !29, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !14)
!29 = !DISubroutineType(types: !30)
!30 = !{!13}
!31 = !DILocalVariable(name: "arr2", scope: !28, file: !1, line: 8, type: !32)
!32 = !DICompositeType(tag: DW_TAG_array_type, baseType: !13, size: 8192, elements: !33)
!33 = !{!34}
!34 = !DISubrange(count: 256)
!35 = !DILocation(line: 8, column: 6, scope: !28)
!36 = !DILocalVariable(name: "num", scope: !28, file: !1, line: 9, type: !13)
!37 = !DILocation(line: 9, column: 6, scope: !28)
!38 = !DILocation(line: 10, column: 7, scope: !28)
!39 = !DILocation(line: 10, column: 2, scope: !28)
!40 = !DILocation(line: 12, column: 1, scope: !28)
