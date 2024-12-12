; ModuleID = 'test.ll'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.teststruct = type { i32, i32, i32 }
%struct.Metadata = type { i8*, i8* }

@.str = private unnamed_addr constant [12 x i8] c"field1: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"field2: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [12 x i8] c"field3: %d\0A\00", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 0, void ()* @__global_init, i8* null }]

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !10 {
  %1 = alloca %struct.teststruct, align 4
  %.voidptr = bitcast %struct.teststruct* %1 to i8*, !dbg !15
  %mtmp = getelementptr %struct.teststruct, %struct.teststruct* %1, i32 1, !dbg !15
  %mtmp.voidptr = bitcast %struct.teststruct* %mtmp to i8*, !dbg !15
  call void @llvm.dbg.declare(metadata %struct.teststruct* %1, metadata !16, metadata !DIExpression()), !dbg !15
  %2 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !22
  %.voidptr1 = bitcast i32* %2 to i8*, !dbg !23
  %3 = icmp ugt i8* %.voidptr, %.voidptr1, !dbg !23
  %4 = select i1 %3, i8* %.voidptr, i8* %.voidptr1, !dbg !23
  %5 = bitcast i32* %2 to %struct.teststruct*, !dbg !23
  %6 = getelementptr %struct.teststruct, %struct.teststruct* %5, i32 1, !dbg !23
  %.voidptr2 = bitcast %struct.teststruct* %6 to i8*, !dbg !23
  %7 = icmp ult i8* %mtmp.voidptr, %.voidptr2, !dbg !23
  %8 = select i1 %7, i8* %mtmp.voidptr, i8* %.voidptr2, !dbg !23
  store i32 1234, i32* %2, align 4, !dbg !23
  %.voidptr16 = bitcast i32* %2 to i8*, !dbg !24
  call void @_softboundcets_bound_check(i8* %4, i8* %8, i8* %.voidptr16), !dbg !24
  %.voidptr3 = bitcast i32* %2 to i8*, !dbg !24
  %9 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !24
  %.voidptr4 = bitcast i32* %9 to i8*, !dbg !25
  %10 = icmp ugt i8* %.voidptr, %.voidptr4, !dbg !25
  %11 = select i1 %10, i8* %.voidptr, i8* %.voidptr4, !dbg !25
  %12 = bitcast i32* %9 to %struct.teststruct*, !dbg !25
  %13 = getelementptr %struct.teststruct, %struct.teststruct* %12, i32 1, !dbg !25
  %.voidptr5 = bitcast %struct.teststruct* %13 to i8*, !dbg !25
  %14 = icmp ult i8* %mtmp.voidptr, %.voidptr5, !dbg !25
  %15 = select i1 %14, i8* %mtmp.voidptr, i8* %.voidptr5, !dbg !25
  store i32 5678, i32* %9, align 4, !dbg !25
  %.voidptr17 = bitcast i32* %9 to i8*, !dbg !26
  call void @_softboundcets_bound_check(i8* %11, i8* %15, i8* %.voidptr17), !dbg !26
  %.voidptr6 = bitcast i32* %9 to i8*, !dbg !26
  %16 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !26
  %.voidptr7 = bitcast i32* %16 to i8*, !dbg !26
  %17 = icmp ugt i8* %.voidptr, %.voidptr7, !dbg !26
  %18 = select i1 %17, i8* %.voidptr, i8* %.voidptr7, !dbg !26
  %19 = bitcast i32* %16 to %struct.teststruct*, !dbg !26
  %20 = getelementptr %struct.teststruct, %struct.teststruct* %19, i32 1, !dbg !26
  %.voidptr8 = bitcast %struct.teststruct* %20 to i8*, !dbg !26
  %21 = icmp ult i8* %mtmp.voidptr, %.voidptr8, !dbg !26
  %22 = select i1 %21, i8* %mtmp.voidptr, i8* %.voidptr8, !dbg !26
  %.voidptr9 = bitcast i32* %16 to i8*, !dbg !26
  %23 = load i32, i32* %16, align 4, !dbg !26
  %.voidptr18 = bitcast i32* %16 to i8*, !dbg !27
  call void @_softboundcets_bound_check(i8* %18, i8* %22, i8* %.voidptr18), !dbg !27
  %24 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0), i32 noundef %23), !dbg !27
  %25 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !28
  %.voidptr10 = bitcast i32* %25 to i8*, !dbg !28
  %26 = icmp ugt i8* %.voidptr, %.voidptr10, !dbg !28
  %27 = select i1 %26, i8* %.voidptr, i8* %.voidptr10, !dbg !28
  %28 = bitcast i32* %25 to %struct.teststruct*, !dbg !28
  %29 = getelementptr %struct.teststruct, %struct.teststruct* %28, i32 1, !dbg !28
  %.voidptr11 = bitcast %struct.teststruct* %29 to i8*, !dbg !28
  %30 = icmp ult i8* %mtmp.voidptr, %.voidptr11, !dbg !28
  %31 = select i1 %30, i8* %mtmp.voidptr, i8* %.voidptr11, !dbg !28
  %.voidptr12 = bitcast i32* %25 to i8*, !dbg !28
  %32 = load i32, i32* %25, align 4, !dbg !28
  %.voidptr19 = bitcast i32* %25 to i8*, !dbg !29
  call void @_softboundcets_bound_check(i8* %27, i8* %31, i8* %.voidptr19), !dbg !29
  %33 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i64 0, i64 0), i32 noundef %32), !dbg !29
  %34 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !30
  %.voidptr13 = bitcast i32* %34 to i8*, !dbg !30
  %35 = icmp ugt i8* %.voidptr, %.voidptr13, !dbg !30
  %36 = select i1 %35, i8* %.voidptr, i8* %.voidptr13, !dbg !30
  %37 = bitcast i32* %34 to %struct.teststruct*, !dbg !30
  %38 = getelementptr %struct.teststruct, %struct.teststruct* %37, i32 1, !dbg !30
  %.voidptr14 = bitcast %struct.teststruct* %38 to i8*, !dbg !30
  %39 = icmp ult i8* %mtmp.voidptr, %.voidptr14, !dbg !30
  %40 = select i1 %39, i8* %mtmp.voidptr, i8* %.voidptr14, !dbg !30
  %.voidptr15 = bitcast i32* %34 to i8*, !dbg !30
  %41 = load i32, i32* %34, align 4, !dbg !30
  %.voidptr20 = bitcast i32* %34 to i8*, !dbg !31
  call void @_softboundcets_bound_check(i8* %36, i8* %40, i8* %.voidptr20), !dbg !31
  %42 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i64 0, i64 0), i32 noundef %41), !dbg !31
  ret i32 0, !dbg !32
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare i32 @printf(i8* noundef, ...) #2

declare void @_softboundcets_set_metadata(i8*, i8*, i8*)

declare void @_softboundcets_print_metadata(i8*, i8*)

declare void @_softboundcets_bound_check(i8*, i8*, i8*)

declare void @_softboundcets_print_metadata_table()

declare i8* @__softboundcets_get_base_addr(i8*)

declare i8* @__softboundcets_get_bound_addr(i8*)

declare void @_softboundcets_init_metadata_table()

declare void @_ASan_init()

declare void @__softboundcets_store_metadata_shadow_stack(i8*, i8*, i64)

declare void @__softboundcets_allocate_shadow_stack_space(i64)

declare void @__softboundcets_deallocate_shadow_stack_space()

declare %struct.Metadata* @__softboundcets_shadow_stack_metadata_ptr(i64)

declare %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8*)

define internal void @__global_init() {
entry:
  call void @_softboundcets_init_metadata_table()
  call void @_ASan_init()
  ret void
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "938b96ec1cf9ccdff45dcb2d0b1eea27")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 1}
!8 = !{i32 7, !"frame-pointer", i32 2}
!9 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 24, type: !11, scopeLine: 25, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !14)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !DILocation(line: 30, column: 20, scope: !10)
!16 = !DILocalVariable(name: "node", scope: !10, file: !1, line: 30, type: !17)
!17 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !1, line: 16, size: 96, elements: !18)
!18 = !{!19, !20, !21}
!19 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !17, file: !1, line: 19, baseType: !13, size: 32)
!20 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !17, file: !1, line: 20, baseType: !13, size: 32, offset: 32)
!21 = !DIDerivedType(tag: DW_TAG_member, name: "field3", scope: !17, file: !1, line: 21, baseType: !13, size: 32, offset: 64)
!22 = !DILocation(line: 31, column: 7, scope: !10)
!23 = !DILocation(line: 31, column: 14, scope: !10)
!24 = !DILocation(line: 32, column: 7, scope: !10)
!25 = !DILocation(line: 32, column: 14, scope: !10)
!26 = !DILocation(line: 36, column: 30, scope: !10)
!27 = !DILocation(line: 36, column: 2, scope: !10)
!28 = !DILocation(line: 37, column: 30, scope: !10)
!29 = !DILocation(line: 37, column: 2, scope: !10)
!30 = !DILocation(line: 38, column: 30, scope: !10)
!31 = !DILocation(line: 38, column: 2, scope: !10)
!32 = !DILocation(line: 39, column: 1, scope: !10)
