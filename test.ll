; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.teststruct = type { i32, i32, i32 }

; Function Attrs: noinline nounwind uwtable
define dso_local void @func(i32* noundef %0, i32* noundef %1) #0 !dbg !19 {
  %3 = alloca i32*, align 8
  %4 = alloca i32*, align 8
  %5 = alloca i8*, align 8
  store i32* %0, i32** %3, align 8
  call void @llvm.dbg.declare(metadata i32** %3, metadata !23, metadata !DIExpression()), !dbg !24
  store i32* %1, i32** %4, align 8
  call void @llvm.dbg.declare(metadata i32** %4, metadata !25, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.declare(metadata i8** %5, metadata !27, metadata !DIExpression()), !dbg !29
  %6 = load i32*, i32** %4, align 8, !dbg !30
  %7 = bitcast i32* %6 to i8*, !dbg !30
  store i8* %7, i8** %5, align 8, !dbg !29
  %8 = load i8*, i8** %5, align 8, !dbg !31
  %9 = bitcast i8* %8 to i32*, !dbg !32
  %10 = getelementptr inbounds i32, i32* %9, i64 10, !dbg !33
  store i32 255, i32* %10, align 4, !dbg !34
  ret void, !dbg !35
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !36 {
  %1 = alloca [10 x i32], align 16
  %2 = alloca [8 x i32], align 16
  %3 = alloca %struct.teststruct*, align 8
  %4 = alloca i32*, align 8
  call void @llvm.dbg.declare(metadata [10 x i32]* %1, metadata !39, metadata !DIExpression()), !dbg !43
  call void @llvm.dbg.declare(metadata [8 x i32]* %2, metadata !44, metadata !DIExpression()), !dbg !48
  call void @llvm.dbg.declare(metadata %struct.teststruct** %3, metadata !49, metadata !DIExpression()), !dbg !50
  %5 = call noalias i8* @malloc(i64 noundef 12) #3, !dbg !51
  %6 = bitcast i8* %5 to %struct.teststruct*, !dbg !52
  store %struct.teststruct* %6, %struct.teststruct** %3, align 8, !dbg !50
  %7 = load %struct.teststruct*, %struct.teststruct** %3, align 8, !dbg !53
  %8 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %7, i32 0, i32 0, !dbg !54
  store i32 1, i32* %8, align 4, !dbg !55
  %9 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 0, !dbg !56
  %10 = getelementptr inbounds [8 x i32], [8 x i32]* %2, i64 0, i64 0, !dbg !57
  call void @func(i32* noundef %9, i32* noundef %10), !dbg !58
  call void @llvm.dbg.declare(metadata i32** %4, metadata !59, metadata !DIExpression()), !dbg !60
  %11 = load %struct.teststruct*, %struct.teststruct** %3, align 8, !dbg !61
  %12 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %11, i64 1, !dbg !62
  %13 = bitcast %struct.teststruct* %12 to i32*, !dbg !63
  store i32* %13, i32** %4, align 8, !dbg !60
  %14 = load i32*, i32** %4, align 8, !dbg !64
  store i32 -559038737, i32* %14, align 4, !dbg !65
  ret i32 0, !dbg !66
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
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "9d9e54216a54c3f28afefbfc6451e0cc")
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
!21 = !{null, !3, !3}
!22 = !{}
!23 = !DILocalVariable(name: "arr", arg: 1, scope: !19, file: !1, line: 3, type: !3)
!24 = !DILocation(line: 3, column: 16, scope: !19)
!25 = !DILocalVariable(name: "arr2", arg: 2, scope: !19, file: !1, line: 3, type: !3)
!26 = !DILocation(line: 3, column: 26, scope: !19)
!27 = !DILocalVariable(name: "test", scope: !19, file: !1, line: 5, type: !28)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!29 = !DILocation(line: 5, column: 8, scope: !19)
!30 = !DILocation(line: 5, column: 15, scope: !19)
!31 = !DILocation(line: 6, column: 11, scope: !19)
!32 = !DILocation(line: 6, column: 4, scope: !19)
!33 = !DILocation(line: 6, column: 16, scope: !19)
!34 = !DILocation(line: 6, column: 22, scope: !19)
!35 = !DILocation(line: 7, column: 1, scope: !19)
!36 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 16, type: !37, scopeLine: 17, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !22)
!37 = !DISubroutineType(types: !38)
!38 = !{!4}
!39 = !DILocalVariable(name: "arr", scope: !36, file: !1, line: 18, type: !40)
!40 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 320, elements: !41)
!41 = !{!42}
!42 = !DISubrange(count: 10)
!43 = !DILocation(line: 18, column: 6, scope: !36)
!44 = !DILocalVariable(name: "arr2", scope: !36, file: !1, line: 19, type: !45)
!45 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 256, elements: !46)
!46 = !{!47}
!47 = !DISubrange(count: 8)
!48 = !DILocation(line: 19, column: 6, scope: !36)
!49 = !DILocalVariable(name: "node", scope: !36, file: !1, line: 22, type: !5)
!50 = !DILocation(line: 22, column: 21, scope: !36)
!51 = !DILocation(line: 22, column: 49, scope: !36)
!52 = !DILocation(line: 22, column: 28, scope: !36)
!53 = !DILocation(line: 23, column: 2, scope: !36)
!54 = !DILocation(line: 23, column: 8, scope: !36)
!55 = !DILocation(line: 23, column: 15, scope: !36)
!56 = !DILocation(line: 24, column: 7, scope: !36)
!57 = !DILocation(line: 24, column: 12, scope: !36)
!58 = !DILocation(line: 24, column: 2, scope: !36)
!59 = !DILocalVariable(name: "out_of_bound_access", scope: !36, file: !1, line: 25, type: !3)
!60 = !DILocation(line: 25, column: 7, scope: !36)
!61 = !DILocation(line: 25, column: 37, scope: !36)
!62 = !DILocation(line: 25, column: 42, scope: !36)
!63 = !DILocation(line: 25, column: 29, scope: !36)
!64 = !DILocation(line: 26, column: 3, scope: !36)
!65 = !DILocation(line: 26, column: 23, scope: !36)
!66 = !DILocation(line: 27, column: 1, scope: !36)
