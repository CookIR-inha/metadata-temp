; ModuleID = 'test.ll'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.teststruct = type { [4 x i8], i32, i32 }
%struct.Metadata = type { i8*, i8* }

@.str = private unnamed_addr constant [8 x i8] c"abcdefg\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"field1: %s\0A\00", align 1
@.str.2 = private unnamed_addr constant [12 x i8] c"field2: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [12 x i8] c"field3: %d\0A\00", align 1
@test = dso_local global i32 0, align 4, !dbg !0
@test2 = dso_local global i32 0, align 4, !dbg !5
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 0, void ()* @__global_init, i8* null }]

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !16 {
  %1 = alloca %struct.teststruct, align 4
  %.voidptr = bitcast %struct.teststruct* %1 to i8*, !dbg !20
  %mtmp = getelementptr %struct.teststruct, %struct.teststruct* %1, i32 1, !dbg !20
  %mtmp.voidptr = bitcast %struct.teststruct* %mtmp to i8*, !dbg !20
  call void @llvm.dbg.declare(metadata %struct.teststruct* %1, metadata !21, metadata !DIExpression()), !dbg !20
  %2 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !31
  store i32 1234, i32* %2, align 4, !dbg !32
  %.voidptr13 = bitcast i32* %2 to i8*, !dbg !33
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %.voidptr13), !dbg !33
  %.voidptr1 = bitcast i32* %2 to i8*, !dbg !33
  %3 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !33
  store i32 5678, i32* %3, align 4, !dbg !34
  %.voidptr14 = bitcast i32* %3 to i8*, !dbg !35
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %.voidptr14), !dbg !35
  %.voidptr2 = bitcast i32* %3 to i8*, !dbg !35
  %4 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !35
  %5 = getelementptr inbounds [4 x i8], [4 x i8]* %4, i64 0, i64 0, !dbg !36
  call void @__softboundcets_allocate_shadow_stack_space(i64 3)
  call void @__softboundcets_store_metadata_shadow_stack(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i64 1), !dbg !37
  call void @__softboundcets_store_metadata_shadow_stack(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i32 1, i32 0), i64 2), !dbg !37
  %6 = call i8* @_softboundcets_strcpy(i8* noundef %5, i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i64 0, i64 0)) #4, !dbg !37
  %shadow_stack_metadata_ptr = call %struct.Metadata* @__softboundcets_shadow_stack_metadata_ptr(i64 0), !dbg !38
  %baseptr = getelementptr inbounds %struct.Metadata, %struct.Metadata* %shadow_stack_metadata_ptr, i32 0, i32 0, !dbg !38
  %baseptr.voidptr = bitcast i8** %baseptr to i8*, !dbg !38
  %7 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %baseptr.voidptr), !dbg !38
  %baseptr3 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %7, i32 0, i32 0, !dbg !38
  %base4 = load i8*, i8** %baseptr3, align 8, !dbg !38
  %baseptr3.voidptr = bitcast i8** %baseptr3 to i8*, !dbg !38
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr3.voidptr), !dbg !38
  %boundptr5 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %7, i32 0, i32 1, !dbg !38
  %base6 = load i8*, i8** %boundptr5, align 8, !dbg !38
  %boundptr5.voidptr = bitcast i8** %boundptr5 to i8*, !dbg !38
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr5.voidptr), !dbg !38
  %base = load i8*, i8** %baseptr, align 8, !dbg !38
  %baseptr.voidptr15 = bitcast i8** %baseptr to i8*, !dbg !38
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr.voidptr15), !dbg !38
  %boundptr = getelementptr inbounds %struct.Metadata, %struct.Metadata* %shadow_stack_metadata_ptr, i32 0, i32 1, !dbg !38
  %boundptr.voidptr = bitcast i8** %boundptr to i8*, !dbg !38
  %8 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %boundptr.voidptr), !dbg !38
  %baseptr7 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %8, i32 0, i32 0, !dbg !38
  %base8 = load i8*, i8** %baseptr7, align 8, !dbg !38
  %baseptr7.voidptr = bitcast i8** %baseptr7 to i8*, !dbg !38
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr7.voidptr), !dbg !38
  %boundptr9 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %8, i32 0, i32 1, !dbg !38
  %base10 = load i8*, i8** %boundptr9, align 8, !dbg !38
  %boundptr9.voidptr = bitcast i8** %boundptr9 to i8*, !dbg !38
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr9.voidptr), !dbg !38
  %bound = load i8*, i8** %boundptr, align 8, !dbg !38
  %boundptr.voidptr16 = bitcast i8** %boundptr to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr.voidptr16)
  call void @__softboundcets_deallocate_shadow_stack_space()
  %9 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !38
  %10 = getelementptr inbounds [4 x i8], [4 x i8]* %9, i64 0, i64 0, !dbg !39
  %11 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i64 0, i64 0), i8* noundef %10), !dbg !40
  %12 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !41
  %.voidptr11 = bitcast i32* %12 to i8*, !dbg !41
  %13 = load i32, i32* %12, align 4, !dbg !41
  %.voidptr17 = bitcast i32* %12 to i8*, !dbg !42
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %.voidptr17), !dbg !42
  %14 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i64 0, i64 0), i32 noundef %13), !dbg !42
  %15 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !43
  %.voidptr12 = bitcast i32* %15 to i8*, !dbg !43
  %16 = load i32, i32* %15, align 4, !dbg !43
  %.voidptr18 = bitcast i32* %15 to i8*, !dbg !44
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %.voidptr18), !dbg !44
  %17 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i64 0, i64 0), i32 noundef %16), !dbg !44
  ret i32 0, !dbg !45
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare i8* @strcpy(i8* noundef, i8* noundef) #2

declare i32 @printf(i8* noundef, ...) #3

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

; Function Attrs: nounwind
declare i8* @_softboundcets_strcpy(i8* noundef, i8* noundef) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!8, !9, !10, !11, !12, !13, !14}
!llvm.ident = !{!15}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "test", scope: !2, file: !3, line: 19, type: !7, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "42a993073abeaf09f20ac502c71bc245")
!4 = !{!0, !5}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "test2", scope: !2, file: !3, line: 20, type: !7, isLocal: false, isDefinition: true)
!7 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!8 = !{i32 7, !"Dwarf Version", i32 5}
!9 = !{i32 2, !"Debug Info Version", i32 3}
!10 = !{i32 1, !"wchar_size", i32 4}
!11 = !{i32 7, !"PIC Level", i32 2}
!12 = !{i32 7, !"PIE Level", i32 2}
!13 = !{i32 7, !"uwtable", i32 1}
!14 = !{i32 7, !"frame-pointer", i32 2}
!15 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!16 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 29, type: !17, scopeLine: 30, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !19)
!17 = !DISubroutineType(types: !18)
!18 = !{!7}
!19 = !{}
!20 = !DILocation(line: 34, column: 20, scope: !16)
!21 = !DILocalVariable(name: "node", scope: !16, file: !3, line: 34, type: !22)
!22 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !3, line: 23, size: 96, elements: !23)
!23 = !{!24, !29, !30}
!24 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !22, file: !3, line: 25, baseType: !25, size: 32)
!25 = !DICompositeType(tag: DW_TAG_array_type, baseType: !26, size: 32, elements: !27)
!26 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!27 = !{!28}
!28 = !DISubrange(count: 4)
!29 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !22, file: !3, line: 26, baseType: !7, size: 32, offset: 32)
!30 = !DIDerivedType(tag: DW_TAG_member, name: "field3", scope: !22, file: !3, line: 27, baseType: !7, size: 32, offset: 64)
!31 = !DILocation(line: 35, column: 7, scope: !16)
!32 = !DILocation(line: 35, column: 14, scope: !16)
!33 = !DILocation(line: 36, column: 7, scope: !16)
!34 = !DILocation(line: 36, column: 14, scope: !16)
!35 = !DILocation(line: 37, column: 14, scope: !16)
!36 = !DILocation(line: 37, column: 9, scope: !16)
!37 = !DILocation(line: 37, column: 2, scope: !16)
!38 = !DILocation(line: 38, column: 30, scope: !16)
!39 = !DILocation(line: 38, column: 25, scope: !16)
!40 = !DILocation(line: 38, column: 2, scope: !16)
!41 = !DILocation(line: 39, column: 33, scope: !16)
!42 = !DILocation(line: 39, column: 5, scope: !16)
!43 = !DILocation(line: 40, column: 33, scope: !16)
!44 = !DILocation(line: 40, column: 5, scope: !16)
!45 = !DILocation(line: 41, column: 1, scope: !16)
