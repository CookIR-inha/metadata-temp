; ModuleID = 'test.ll'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.Metadata = type { i8*, i8* }
%struct.teststruct = type { i32, i32, i32 }

@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 0, void ()* @__global_init, i8* null }]

; Function Attrs: noinline nounwind uwtable
define dso_local void @func(i32* noundef %0, i32* noundef %1) #0 !dbg !19 {
  %shadow_stack_metadata_ptr1 = call %struct.Metadata* @__softboundcets_shadow_stack_metadata_ptr(i64 1)
  %baseptr2 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %shadow_stack_metadata_ptr1, i32 0, i32 0
  %baseptr2.voidptr = bitcast i8** %baseptr2 to i8*
  %3 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %baseptr2.voidptr)
  %baseptr6 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %3, i32 0, i32 0
  %base7 = load i8*, i8** %baseptr6, align 8
  %baseptr6.voidptr = bitcast i8** %baseptr6 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr6.voidptr)
  %boundptr8 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %3, i32 0, i32 1
  %base9 = load i8*, i8** %boundptr8, align 8
  %boundptr8.voidptr = bitcast i8** %boundptr8 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr8.voidptr)
  %base3 = load i8*, i8** %baseptr2, align 8
  %baseptr2.voidptr40 = bitcast i8** %baseptr2 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr2.voidptr40)
  %boundptr4 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %shadow_stack_metadata_ptr1, i32 0, i32 1
  %boundptr4.voidptr = bitcast i8** %boundptr4 to i8*
  %4 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %boundptr4.voidptr)
  %baseptr10 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %4, i32 0, i32 0
  %base11 = load i8*, i8** %baseptr10, align 8
  %baseptr10.voidptr = bitcast i8** %baseptr10 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr10.voidptr)
  %boundptr12 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %4, i32 0, i32 1
  %base13 = load i8*, i8** %boundptr12, align 8
  %boundptr12.voidptr = bitcast i8** %boundptr12 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr12.voidptr)
  %bound5 = load i8*, i8** %boundptr4, align 8
  %boundptr4.voidptr41 = bitcast i8** %boundptr4 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr4.voidptr41)
  %shadow_stack_metadata_ptr = call %struct.Metadata* @__softboundcets_shadow_stack_metadata_ptr(i64 0)
  %baseptr = getelementptr inbounds %struct.Metadata, %struct.Metadata* %shadow_stack_metadata_ptr, i32 0, i32 0
  %baseptr.voidptr = bitcast i8** %baseptr to i8*
  %5 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %baseptr.voidptr)
  %baseptr14 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %5, i32 0, i32 0
  %base15 = load i8*, i8** %baseptr14, align 8
  %baseptr14.voidptr = bitcast i8** %baseptr14 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr14.voidptr)
  %boundptr16 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %5, i32 0, i32 1
  %base17 = load i8*, i8** %boundptr16, align 8
  %boundptr16.voidptr = bitcast i8** %boundptr16 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr16.voidptr)
  %base = load i8*, i8** %baseptr, align 8
  %baseptr.voidptr42 = bitcast i8** %baseptr to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr.voidptr42)
  %boundptr = getelementptr inbounds %struct.Metadata, %struct.Metadata* %shadow_stack_metadata_ptr, i32 0, i32 1
  %boundptr.voidptr = bitcast i8** %boundptr to i8*
  %6 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %boundptr.voidptr)
  %baseptr18 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %6, i32 0, i32 0
  %base19 = load i8*, i8** %baseptr18, align 8
  %baseptr18.voidptr = bitcast i8** %baseptr18 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr18.voidptr)
  %boundptr20 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %6, i32 0, i32 1
  %base21 = load i8*, i8** %boundptr20, align 8
  %boundptr20.voidptr = bitcast i8** %boundptr20 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr20.voidptr)
  %bound = load i8*, i8** %boundptr, align 8
  %boundptr.voidptr43 = bitcast i8** %boundptr to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr.voidptr43)
  %7 = alloca i32*, align 8
  %.voidptr = bitcast i32** %7 to i8*
  %mtmp = getelementptr i32*, i32** %7, i32 1
  %mtmp.voidptr = bitcast i32** %mtmp to i8*
  %8 = alloca i32*, align 8
  %.voidptr22 = bitcast i32** %8 to i8*
  %mtmp23 = getelementptr i32*, i32** %8, i32 1
  %mtmp23.voidptr = bitcast i32** %mtmp23 to i8*
  %9 = alloca i8*, align 8
  %.voidptr24 = bitcast i8** %9 to i8*
  %mtmp25 = getelementptr i8*, i8** %9, i32 1
  %mtmp25.voidptr = bitcast i8** %mtmp25 to i8*
  store i32* %0, i32** %7, align 8
  %.voidptr44 = bitcast i32** %7 to i8*, !dbg !23
  %.voidptr26 = bitcast i32** %7 to i8*, !dbg !23
  call void @_softboundcets_set_metadata(i8* %.voidptr26, i8* %base, i8* %bound), !dbg !23
  call void @llvm.dbg.declare(metadata i32** %7, metadata !24, metadata !DIExpression()), !dbg !23
  store i32* %1, i32** %8, align 8
  %.voidptr45 = bitcast i32** %8 to i8*, !dbg !25
  %.voidptr27 = bitcast i32** %8 to i8*, !dbg !25
  call void @_softboundcets_set_metadata(i8* %.voidptr27, i8* %base3, i8* %bound5), !dbg !25
  call void @llvm.dbg.declare(metadata i32** %8, metadata !26, metadata !DIExpression()), !dbg !25
  call void @llvm.dbg.declare(metadata i8** %9, metadata !27, metadata !DIExpression()), !dbg !29
  %.voidptr28 = bitcast i32** %8 to i8*, !dbg !30
  %10 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %.voidptr28), !dbg !30
  %baseptr29 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %10, i32 0, i32 0, !dbg !30
  %base30 = load i8*, i8** %baseptr29, align 8, !dbg !30
  %baseptr29.voidptr = bitcast i8** %baseptr29 to i8*, !dbg !30
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr29.voidptr), !dbg !30
  %boundptr31 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %10, i32 0, i32 1, !dbg !30
  %base32 = load i8*, i8** %boundptr31, align 8, !dbg !30
  %boundptr31.voidptr = bitcast i8** %boundptr31 to i8*, !dbg !30
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr31.voidptr), !dbg !30
  %11 = load i32*, i32** %8, align 8, !dbg !30
  %.voidptr46 = bitcast i32** %8 to i8*, !dbg !30
  call void @_softboundcets_bound_check(i8* %.voidptr22, i8* %mtmp23.voidptr, i8* %.voidptr46), !dbg !30
  %12 = bitcast i32* %11 to i8*, !dbg !30
  store i8* %12, i8** %9, align 8, !dbg !29
  %.voidptr47 = bitcast i8** %9 to i8*, !dbg !31
  %.voidptr33 = bitcast i8** %9 to i8*, !dbg !31
  call void @_softboundcets_set_metadata(i8* %.voidptr33, i8* %base30, i8* %base32), !dbg !31
  %.voidptr34 = bitcast i8** %9 to i8*, !dbg !31
  %13 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %.voidptr34), !dbg !31
  %baseptr35 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %13, i32 0, i32 0, !dbg !31
  %base36 = load i8*, i8** %baseptr35, align 8, !dbg !31
  %baseptr35.voidptr = bitcast i8** %baseptr35 to i8*, !dbg !31
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr35.voidptr), !dbg !31
  %boundptr37 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %13, i32 0, i32 1, !dbg !31
  %base38 = load i8*, i8** %boundptr37, align 8, !dbg !31
  %boundptr37.voidptr = bitcast i8** %boundptr37 to i8*, !dbg !31
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr37.voidptr), !dbg !31
  %14 = load i8*, i8** %9, align 8, !dbg !31
  %.voidptr48 = bitcast i8** %9 to i8*, !dbg !32
  call void @_softboundcets_bound_check(i8* %.voidptr24, i8* %mtmp25.voidptr, i8* %.voidptr48), !dbg !32
  %15 = bitcast i8* %14 to i32*, !dbg !32
  %16 = getelementptr inbounds i32, i32* %15, i64 11, !dbg !33
  store i32 255, i32* %16, align 4, !dbg !34
  %.voidptr49 = bitcast i32* %16 to i8*, !dbg !35
  call void @_softboundcets_bound_check(i8* %base36, i8* %base38, i8* %.voidptr49), !dbg !35
  %.voidptr39 = bitcast i32* %16 to i8*, !dbg !35
  ret void, !dbg !35
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !36 {
  %1 = alloca [10 x i32], align 16
  %.voidptr = bitcast [10 x i32]* %1 to i8*
  %mtmp = getelementptr [10 x i32], [10 x i32]* %1, i32 1
  %mtmp.voidptr = bitcast [10 x i32]* %mtmp to i8*
  %2 = alloca [8 x i32], align 16
  %.voidptr1 = bitcast [8 x i32]* %2 to i8*
  %mtmp2 = getelementptr [8 x i32], [8 x i32]* %2, i32 1
  %mtmp2.voidptr = bitcast [8 x i32]* %mtmp2 to i8*
  %3 = alloca i8*, align 8
  %.voidptr3 = bitcast i8** %3 to i8*
  %mtmp4 = getelementptr i8*, i8** %3, i32 1
  %mtmp4.voidptr = bitcast i8** %mtmp4 to i8*
  %4 = alloca %struct.teststruct*, align 8
  %.voidptr5 = bitcast %struct.teststruct** %4 to i8*, !dbg !39
  %mtmp6 = getelementptr %struct.teststruct*, %struct.teststruct** %4, i32 1, !dbg !39
  %mtmp6.voidptr = bitcast %struct.teststruct** %mtmp6 to i8*, !dbg !39
  call void @llvm.dbg.declare(metadata [10 x i32]* %1, metadata !40, metadata !DIExpression()), !dbg !39
  call void @llvm.dbg.declare(metadata [8 x i32]* %2, metadata !44, metadata !DIExpression()), !dbg !48
  call void @llvm.dbg.declare(metadata i8** %3, metadata !49, metadata !DIExpression()), !dbg !50
  %5 = getelementptr inbounds [10 x i32], [10 x i32]* %1, i64 0, i64 0, !dbg !51
  %6 = bitcast i32* %5 to i8*, !dbg !51
  store i8* %6, i8** %3, align 8, !dbg !50
  %.voidptr34 = bitcast i8** %3 to i8*, !dbg !52
  %.voidptr7 = bitcast i8** %3 to i8*, !dbg !52
  call void @_softboundcets_set_metadata(i8* %.voidptr7, i8* %.voidptr, i8* %mtmp.voidptr), !dbg !52
  %.voidptr8 = bitcast i8** %3 to i8*, !dbg !52
  %7 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %.voidptr8), !dbg !52
  %baseptr = getelementptr inbounds %struct.Metadata, %struct.Metadata* %7, i32 0, i32 0, !dbg !52
  %base = load i8*, i8** %baseptr, align 8, !dbg !52
  %baseptr.voidptr = bitcast i8** %baseptr to i8*, !dbg !52
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr.voidptr), !dbg !52
  %boundptr = getelementptr inbounds %struct.Metadata, %struct.Metadata* %7, i32 0, i32 1, !dbg !52
  %base9 = load i8*, i8** %boundptr, align 8, !dbg !52
  %boundptr.voidptr = bitcast i8** %boundptr to i8*, !dbg !52
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr.voidptr), !dbg !52
  %8 = load i8*, i8** %3, align 8, !dbg !52
  %.voidptr35 = bitcast i8** %3 to i8*, !dbg !53
  call void @_softboundcets_bound_check(i8* %.voidptr3, i8* %mtmp4.voidptr, i8* %.voidptr35), !dbg !53
  %9 = bitcast i8* %8 to i32*, !dbg !53
  %10 = getelementptr inbounds i32, i32* %9, i64 11, !dbg !54
  store i32 1, i32* %10, align 4, !dbg !55
  %.voidptr36 = bitcast i32* %10 to i8*, !dbg !56
  call void @_softboundcets_bound_check(i8* %base, i8* %base9, i8* %.voidptr36), !dbg !56
  %.voidptr10 = bitcast i32* %10 to i8*, !dbg !56
  call void @llvm.dbg.declare(metadata %struct.teststruct** %4, metadata !57, metadata !DIExpression()), !dbg !56
  call void @__softboundcets_allocate_shadow_stack_space(i64 1)
  %11 = call noalias i8* @_softboundcets_malloc(i64 noundef 12) #3, !dbg !58
  %shadow_stack_metadata_ptr = call %struct.Metadata* @__softboundcets_shadow_stack_metadata_ptr(i64 0), !dbg !59
  %baseptr11 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %shadow_stack_metadata_ptr, i32 0, i32 0, !dbg !59
  %baseptr11.voidptr = bitcast i8** %baseptr11 to i8*, !dbg !59
  %12 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %baseptr11.voidptr), !dbg !59
  %baseptr14 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %12, i32 0, i32 0, !dbg !59
  %base15 = load i8*, i8** %baseptr14, align 8, !dbg !59
  %baseptr14.voidptr = bitcast i8** %baseptr14 to i8*, !dbg !59
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr14.voidptr), !dbg !59
  %boundptr16 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %12, i32 0, i32 1, !dbg !59
  %base17 = load i8*, i8** %boundptr16, align 8, !dbg !59
  %boundptr16.voidptr = bitcast i8** %boundptr16 to i8*, !dbg !59
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr16.voidptr), !dbg !59
  %base12 = load i8*, i8** %baseptr11, align 8, !dbg !59
  %baseptr11.voidptr37 = bitcast i8** %baseptr11 to i8*, !dbg !59
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr11.voidptr37), !dbg !59
  %boundptr13 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %shadow_stack_metadata_ptr, i32 0, i32 1, !dbg !59
  %boundptr13.voidptr = bitcast i8** %boundptr13 to i8*, !dbg !59
  %13 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %boundptr13.voidptr), !dbg !59
  %baseptr18 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %13, i32 0, i32 0, !dbg !59
  %base19 = load i8*, i8** %baseptr18, align 8, !dbg !59
  %baseptr18.voidptr = bitcast i8** %baseptr18 to i8*, !dbg !59
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr18.voidptr), !dbg !59
  %boundptr20 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %13, i32 0, i32 1, !dbg !59
  %base21 = load i8*, i8** %boundptr20, align 8, !dbg !59
  %boundptr20.voidptr = bitcast i8** %boundptr20 to i8*, !dbg !59
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr20.voidptr), !dbg !59
  %bound = load i8*, i8** %boundptr13, align 8, !dbg !59
  %boundptr13.voidptr38 = bitcast i8** %boundptr13 to i8*
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr13.voidptr38)
  call void @__softboundcets_deallocate_shadow_stack_space()
  %14 = bitcast i8* %11 to %struct.teststruct*, !dbg !59
  store %struct.teststruct* %14, %struct.teststruct** %4, align 8, !dbg !56
  %.voidptr39 = bitcast %struct.teststruct** %4 to i8*, !dbg !60
  %.voidptr22 = bitcast %struct.teststruct** %4 to i8*, !dbg !60
  call void @_softboundcets_set_metadata(i8* %.voidptr22, i8* %base12, i8* %bound), !dbg !60
  %.voidptr23 = bitcast %struct.teststruct** %4 to i8*, !dbg !60
  %15 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %.voidptr23), !dbg !60
  %baseptr24 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %15, i32 0, i32 0, !dbg !60
  %base25 = load i8*, i8** %baseptr24, align 8, !dbg !60
  %baseptr24.voidptr = bitcast i8** %baseptr24 to i8*, !dbg !60
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr24.voidptr), !dbg !60
  %boundptr26 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %15, i32 0, i32 1, !dbg !60
  %base27 = load i8*, i8** %boundptr26, align 8, !dbg !60
  %boundptr26.voidptr = bitcast i8** %boundptr26 to i8*, !dbg !60
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr26.voidptr), !dbg !60
  %16 = load %struct.teststruct*, %struct.teststruct** %4, align 8, !dbg !60
  %.voidptr40 = bitcast %struct.teststruct** %4 to i8*, !dbg !60
  call void @_softboundcets_bound_check(i8* %.voidptr5, i8* %mtmp6.voidptr, i8* %.voidptr40), !dbg !60
  %17 = bitcast %struct.teststruct* %16 to i8*, !dbg !60
  call void @__softboundcets_allocate_shadow_stack_space(i64 1)
  call void @__softboundcets_store_metadata_shadow_stack(i8* %base25, i8* %base27, i64 0), !dbg !61
  call void @_softboundcets_free(i8* noundef %17) #3, !dbg !61
  call void @__softboundcets_deallocate_shadow_stack_space()
  %.voidptr28 = bitcast %struct.teststruct** %4 to i8*, !dbg !62
  %18 = call %struct.Metadata* @__softboundcets_shadowspace_metadata_ptr(i8* %.voidptr28), !dbg !62
  %baseptr29 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %18, i32 0, i32 0, !dbg !62
  %base30 = load i8*, i8** %baseptr29, align 8, !dbg !62
  %baseptr29.voidptr = bitcast i8** %baseptr29 to i8*, !dbg !62
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %baseptr29.voidptr), !dbg !62
  %boundptr31 = getelementptr inbounds %struct.Metadata, %struct.Metadata* %18, i32 0, i32 1, !dbg !62
  %base32 = load i8*, i8** %boundptr31, align 8, !dbg !62
  %boundptr31.voidptr = bitcast i8** %boundptr31 to i8*, !dbg !62
  call void @_softboundcets_bound_check(i8* null, i8* inttoptr (i64 1302123111085380114 to i8*), i8* %boundptr31.voidptr), !dbg !62
  %19 = load %struct.teststruct*, %struct.teststruct** %4, align 8, !dbg !62
  %.voidptr41 = bitcast %struct.teststruct** %4 to i8*, !dbg !63
  call void @_softboundcets_bound_check(i8* %.voidptr5, i8* %mtmp6.voidptr, i8* %.voidptr41), !dbg !63
  %20 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %19, i32 0, i32 0, !dbg !63
  store i32 2, i32* %20, align 4, !dbg !64
  %.voidptr42 = bitcast i32* %20 to i8*, !dbg !65
  call void @_softboundcets_bound_check(i8* %base30, i8* %base32, i8* %.voidptr42), !dbg !65
  %.voidptr33 = bitcast i32* %20 to i8*, !dbg !65
  ret i32 0, !dbg !65
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64 noundef) #2

; Function Attrs: nounwind
declare void @free(i8* noundef) #2

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
declare noalias i8* @_softboundcets_malloc(i64 noundef) #2

; Function Attrs: nounwind
declare void @_softboundcets_free(i8* noundef) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!11, !12, !13, !14, !15, !16, !17}
!llvm.ident = !{!18}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "9e83ae95a9ca5bee6d7a720815694abb")
!2 = !{!3, !5}
!3 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!4 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!5 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !6, size: 64)
!6 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !1, line: 10, size: 96, elements: !7)
!7 = !{!8, !9, !10}
!8 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !6, file: !1, line: 12, baseType: !4, size: 32)
!9 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !6, file: !1, line: 13, baseType: !4, size: 32, offset: 32)
!10 = !DIDerivedType(tag: DW_TAG_member, name: "field3", scope: !6, file: !1, line: 14, baseType: !4, size: 32, offset: 64)
!11 = !{i32 7, !"Dwarf Version", i32 5}
!12 = !{i32 2, !"Debug Info Version", i32 3}
!13 = !{i32 1, !"wchar_size", i32 4}
!14 = !{i32 7, !"PIC Level", i32 2}
!15 = !{i32 7, !"PIE Level", i32 2}
!16 = !{i32 7, !"uwtable", i32 1}
!17 = !{i32 7, !"frame-pointer", i32 2}
!18 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!19 = distinct !DISubprogram(name: "func", scope: !1, file: !1, line: 4, type: !20, scopeLine: 5, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !22)
!20 = !DISubroutineType(types: !21)
!21 = !{null, !3, !3}
!22 = !{}
!23 = !DILocation(line: 4, column: 16, scope: !19)
!24 = !DILocalVariable(name: "arr", arg: 1, scope: !19, file: !1, line: 4, type: !3)
!25 = !DILocation(line: 4, column: 26, scope: !19)
!26 = !DILocalVariable(name: "arr2", arg: 2, scope: !19, file: !1, line: 4, type: !3)
!27 = !DILocalVariable(name: "test", scope: !19, file: !1, line: 6, type: !28)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!29 = !DILocation(line: 6, column: 8, scope: !19)
!30 = !DILocation(line: 6, column: 15, scope: !19)
!31 = !DILocation(line: 7, column: 11, scope: !19)
!32 = !DILocation(line: 7, column: 4, scope: !19)
!33 = !DILocation(line: 7, column: 16, scope: !19)
!34 = !DILocation(line: 7, column: 22, scope: !19)
!35 = !DILocation(line: 8, column: 1, scope: !19)
!36 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 17, type: !37, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !22)
!37 = !DISubroutineType(types: !38)
!38 = !{!4}
!39 = !DILocation(line: 19, column: 6, scope: !36)
!40 = !DILocalVariable(name: "arr", scope: !36, file: !1, line: 19, type: !41)
!41 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 320, elements: !42)
!42 = !{!43}
!43 = !DISubrange(count: 10)
!44 = !DILocalVariable(name: "arr2", scope: !36, file: !1, line: 20, type: !45)
!45 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 256, elements: !46)
!46 = !{!47}
!47 = !DISubrange(count: 8)
!48 = !DILocation(line: 20, column: 6, scope: !36)
!49 = !DILocalVariable(name: "arr_copy", scope: !36, file: !1, line: 21, type: !28)
!50 = !DILocation(line: 21, column: 8, scope: !36)
!51 = !DILocation(line: 21, column: 19, scope: !36)
!52 = !DILocation(line: 22, column: 11, scope: !36)
!53 = !DILocation(line: 22, column: 4, scope: !36)
!54 = !DILocation(line: 22, column: 20, scope: !36)
!55 = !DILocation(line: 22, column: 26, scope: !36)
!56 = !DILocation(line: 23, column: 21, scope: !36)
!57 = !DILocalVariable(name: "node", scope: !36, file: !1, line: 23, type: !5)
!58 = !DILocation(line: 23, column: 49, scope: !36)
!59 = !DILocation(line: 23, column: 28, scope: !36)
!60 = !DILocation(line: 26, column: 7, scope: !36)
!61 = !DILocation(line: 26, column: 2, scope: !36)
!62 = !DILocation(line: 27, column: 2, scope: !36)
!63 = !DILocation(line: 27, column: 8, scope: !36)
!64 = !DILocation(line: 27, column: 15, scope: !36)
!65 = !DILocation(line: 31, column: 1, scope: !36)
