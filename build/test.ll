; ModuleID = '/home/test/metadata-temp/test.c'
source_filename = "/home/test/metadata-temp/test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.teststruct = type { i32, i32 }

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !18 {
  %1 = alloca [10 x i32], align 16
  %2 = alloca %struct.teststruct*, align 8
  call void @llvm.dbg.declare(metadata [10 x i32]* %1, metadata !22, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.declare(metadata %struct.teststruct** %2, metadata !27, metadata !DIExpression()), !dbg !28
  %3 = call i8* @malloc(i64 noundef 8), !dbg !29
  %4 = bitcast i8* %3 to %struct.teststruct*, !dbg !30
  store %struct.teststruct* %4, %struct.teststruct** %2, align 8, !dbg !28
  %5 = load %struct.teststruct*, %struct.teststruct** %2, align 8, !dbg !31
  %6 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %5, i32 0, i32 0, !dbg !32
  store i32 1, i32* %6, align 4, !dbg !33
  ret i32 0, !dbg !34
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare i8* @malloc(i64 noundef) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!10, !11, !12, !13, !14, !15, !16}
!llvm.ident = !{!17}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "/home/test/metadata-temp/test.c", directory: "/home/test/metadata-temp/build", checksumkind: CSK_MD5, checksum: "13147ac18e4dc0133b43e63ee87a624c")
!2 = !{!3}
!3 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!4 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !5, line: 8, size: 64, elements: !6)
!5 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "13147ac18e4dc0133b43e63ee87a624c")
!6 = !{!7, !9}
!7 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !4, file: !5, line: 9, baseType: !8, size: 32)
!8 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!9 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !4, file: !5, line: 10, baseType: !8, size: 32, offset: 32)
!10 = !{i32 7, !"Dwarf Version", i32 5}
!11 = !{i32 2, !"Debug Info Version", i32 3}
!12 = !{i32 1, !"wchar_size", i32 4}
!13 = !{i32 7, !"PIC Level", i32 2}
!14 = !{i32 7, !"PIE Level", i32 2}
!15 = !{i32 7, !"uwtable", i32 1}
!16 = !{i32 7, !"frame-pointer", i32 2}
!17 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!18 = distinct !DISubprogram(name: "main", scope: !5, file: !5, line: 13, type: !19, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !21)
!19 = !DISubroutineType(types: !20)
!20 = !{!8}
!21 = !{}
!22 = !DILocalVariable(name: "arr", scope: !18, file: !5, line: 14, type: !23)
!23 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 320, elements: !24)
!24 = !{!25}
!25 = !DISubrange(count: 10)
!26 = !DILocation(line: 14, column: 6, scope: !18)
!27 = !DILocalVariable(name: "node", scope: !18, file: !5, line: 17, type: !3)
!28 = !DILocation(line: 17, column: 21, scope: !18)
!29 = !DILocation(line: 17, column: 49, scope: !18)
!30 = !DILocation(line: 17, column: 28, scope: !18)
!31 = !DILocation(line: 18, column: 2, scope: !18)
!32 = !DILocation(line: 18, column: 8, scope: !18)
!33 = !DILocation(line: 18, column: 15, scope: !18)
!34 = !DILocation(line: 22, column: 1, scope: !18)
