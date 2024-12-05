; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.teststruct = type { i32, i32, i32 }

; Function Attrs: noinline nounwind uwtable
define dso_local void @func(i32* noundef %0) #0 !dbg !19 {
  %2 = alloca i32*, align 8
  %3 = alloca i8*, align 8
  store i32* %0, i32** %2, align 8
  call void @llvm.dbg.declare(metadata i32** %2, metadata !23, metadata !DIExpression()), !dbg !24
  call void @llvm.dbg.declare(metadata i8** %3, metadata !25, metadata !DIExpression()), !dbg !27
  %4 = load i32*, i32** %2, align 8, !dbg !28
  %5 = bitcast i32* %4 to i8*, !dbg !28
  store i8* %5, i8** %3, align 8, !dbg !27
  %6 = load i8*, i8** %3, align 8, !dbg !29
  %7 = bitcast i8* %6 to i32*, !dbg !30
  %8 = getelementptr inbounds i32, i32* %7, i64 10, !dbg !31
  store i32 255, i32* %8, align 4, !dbg !32
  ret void, !dbg !33
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !34 {
  %1 = alloca [10 x i32], align 16
  %2 = alloca %struct.teststruct*, align 8
  %3 = alloca i32*, align 8
  call void @llvm.dbg.declare(metadata [10 x i32]* %1, metadata !37, metadata !DIExpression()), !dbg !41
  call void @llvm.dbg.declare(metadata %struct.teststruct** %2, metadata !42, metadata !DIExpression()), !dbg !43
  %4 = call noalias i8* @malloc(i64 noundef 12) #3, !dbg !44
  %5 = bitcast i8* %4 to %struct.teststruct*, !dbg !45
  store %struct.teststruct* %5, %struct.teststruct** %2, align 8, !dbg !43
  %6 = load %struct.teststruct*, %struct.teststruct** %2, align 8, !dbg !46
  %7 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %6, i32 0, i32 0, !dbg !47
  store i32 1, i32* %7, align 4, !dbg !48
  call void @llvm.dbg.declare(metadata i32** %3, metadata !49, metadata !DIExpression()), !dbg !50
  %8 = load %struct.teststruct*, %struct.teststruct** %2, align 8, !dbg !51
  %9 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %8, i64 1, !dbg !52
  %10 = bitcast %struct.teststruct* %9 to i32*, !dbg !53
  store i32* %10, i32** %3, align 8, !dbg !50
  %11 = load i32*, i32** %3, align 8, !dbg !54
  store i32 -559038737, i32* %11, align 4, !dbg !55
  ret i32 0, !dbg !56
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64 noundef) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!11, !12, !13, !14, !15, !16, !17}
!llvm.ident = !{!18}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "7891d951f9f6e354edefa88e7f8d3370")
!2 = !{!3, !5}
!3 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!4 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!5 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !6, size: 64)
!6 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !1, line: 9, size: 96, elements: !7)
!7 = !{!8, !9, !10}
!8 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !6, file: !1, line: 11, baseType: !4, size: 32)
!9 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !6, file: !1, line: 12, baseType: !4, size: 32, offset: 32)
!10 = !DIDerivedType(tag: DW_TAG_member, name: "field3", scope: !6, file: !1, line: 13, baseType: !4, size: 32, offset: 64)
!11 = !{i32 7, !"Dwarf Version", i32 5}
!12 = !{i32 2, !"Debug Info Version", i32 3}
!13 = !{i32 1, !"wchar_size", i32 4}
!14 = !{i32 7, !"PIC Level", i32 2}
!15 = !{i32 7, !"PIE Level", i32 2}
!16 = !{i32 7, !"uwtable", i32 1}
!17 = !{i32 7, !"frame-pointer", i32 2}
!18 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!19 = distinct !DISubprogram(name: "func", scope: !1, file: !1, line: 3, type: !20, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !22)
!20 = !DISubroutineType(types: !21)
!21 = !{null, !3}
!22 = !{}
!23 = !DILocalVariable(name: "arr", arg: 1, scope: !19, file: !1, line: 3, type: !3)
!24 = !DILocation(line: 3, column: 16, scope: !19)
!25 = !DILocalVariable(name: "test", scope: !19, file: !1, line: 5, type: !26)
!26 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!27 = !DILocation(line: 5, column: 8, scope: !19)
!28 = !DILocation(line: 5, column: 15, scope: !19)
!29 = !DILocation(line: 6, column: 11, scope: !19)
!30 = !DILocation(line: 6, column: 4, scope: !19)
!31 = !DILocation(line: 6, column: 16, scope: !19)
!32 = !DILocation(line: 6, column: 22, scope: !19)
!33 = !DILocation(line: 7, column: 1, scope: !19)
!34 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 16, type: !35, scopeLine: 17, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !22)
!35 = !DISubroutineType(types: !36)
!36 = !{!4}
!37 = !DILocalVariable(name: "arr", scope: !34, file: !1, line: 18, type: !38)
!38 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 320, elements: !39)
!39 = !{!40}
!40 = !DISubrange(count: 10)
!41 = !DILocation(line: 18, column: 6, scope: !34)
!42 = !DILocalVariable(name: "node", scope: !34, file: !1, line: 21, type: !5)
!43 = !DILocation(line: 21, column: 21, scope: !34)
!44 = !DILocation(line: 21, column: 49, scope: !34)
!45 = !DILocation(line: 21, column: 28, scope: !34)
!46 = !DILocation(line: 22, column: 2, scope: !34)
!47 = !DILocation(line: 22, column: 8, scope: !34)
!48 = !DILocation(line: 22, column: 15, scope: !34)
!49 = !DILocalVariable(name: "out_of_bound_access", scope: !34, file: !1, line: 24, type: !3)
!50 = !DILocation(line: 24, column: 7, scope: !34)
!51 = !DILocation(line: 24, column: 37, scope: !34)
!52 = !DILocation(line: 24, column: 42, scope: !34)
!53 = !DILocation(line: 24, column: 29, scope: !34)
!54 = !DILocation(line: 25, column: 3, scope: !34)
!55 = !DILocation(line: 25, column: 23, scope: !34)
!56 = !DILocation(line: 26, column: 1, scope: !34)
