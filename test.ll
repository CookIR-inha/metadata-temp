; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.teststruct = type { [4 x i8], i32, i32 }

@.str = private unnamed_addr constant [8 x i8] c"abcdefg\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"field1: %s\0A\00", align 1
@.str.2 = private unnamed_addr constant [12 x i8] c"field2: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [12 x i8] c"field3: %d\0A\00", align 1
@test = dso_local global i32 0, align 4, !dbg !0
@test2 = dso_local global i32 0, align 4, !dbg !5

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !16 {
  %1 = alloca %struct.teststruct, align 4
  call void @llvm.dbg.declare(metadata %struct.teststruct* %1, metadata !20, metadata !DIExpression()), !dbg !30
  %2 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !31
  store i32 1234, i32* %2, align 4, !dbg !32
  %3 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !33
  store i32 5678, i32* %3, align 4, !dbg !34
  %4 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !35
  %5 = getelementptr inbounds [4 x i8], [4 x i8]* %4, i64 0, i64 0, !dbg !36
  %6 = call i8* @strcpy(i8* noundef %5, i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i64 0, i64 0)) #4, !dbg !37
  %7 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !38
  %8 = getelementptr inbounds [4 x i8], [4 x i8]* %7, i64 0, i64 0, !dbg !39
  %9 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i64 0, i64 0), i8* noundef %8), !dbg !40
  %10 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !41
  %11 = load i32, i32* %10, align 4, !dbg !41
  %12 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i64 0, i64 0), i32 noundef %11), !dbg !42
  %13 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !43
  %14 = load i32, i32* %13, align 4, !dbg !43
  %15 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i64 0, i64 0), i32 noundef %14), !dbg !44
  ret i32 0, !dbg !45
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare i8* @strcpy(i8* noundef, i8* noundef) #2

declare i32 @printf(i8* noundef, ...) #3

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
!20 = !DILocalVariable(name: "node", scope: !16, file: !3, line: 34, type: !21)
!21 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !3, line: 23, size: 96, elements: !22)
!22 = !{!23, !28, !29}
!23 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !21, file: !3, line: 25, baseType: !24, size: 32)
!24 = !DICompositeType(tag: DW_TAG_array_type, baseType: !25, size: 32, elements: !26)
!25 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!26 = !{!27}
!27 = !DISubrange(count: 4)
!28 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !21, file: !3, line: 26, baseType: !7, size: 32, offset: 32)
!29 = !DIDerivedType(tag: DW_TAG_member, name: "field3", scope: !21, file: !3, line: 27, baseType: !7, size: 32, offset: 64)
!30 = !DILocation(line: 34, column: 20, scope: !16)
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
