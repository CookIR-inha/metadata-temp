; ModuleID = '/home/test/metadata-temp/test.c'
source_filename = "/home/test/metadata-temp/test.c"
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
define dso_local i32 @main() #0 !dbg !17 {
  %1 = alloca %struct.teststruct, align 4
  call void @llvm.dbg.declare(metadata %struct.teststruct* %1, metadata !21, metadata !DIExpression()), !dbg !31
  %2 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !32
  store i32 1234, i32* %2, align 4, !dbg !33
  %3 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !34
  store i32 5678, i32* %3, align 4, !dbg !35
  %4 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !36
  %5 = getelementptr inbounds [4 x i8], [4 x i8]* %4, i64 0, i64 0, !dbg !37
  %6 = call i8* @strcpy(i8* noundef %5, i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i64 0, i64 0)) #4, !dbg !38
  %7 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !39
  %8 = getelementptr inbounds [4 x i8], [4 x i8]* %7, i64 0, i64 0, !dbg !40
  %9 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i64 0, i64 0), i8* noundef %8), !dbg !41
  %10 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !42
  %11 = load i32, i32* %10, align 4, !dbg !42
  %12 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i64 0, i64 0), i32 noundef %11), !dbg !43
  %13 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !44
  %14 = load i32, i32* %13, align 4, !dbg !44
  %15 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i64 0, i64 0), i32 noundef %14), !dbg !45
  ret i32 0, !dbg !46
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
!llvm.module.flags = !{!9, !10, !11, !12, !13, !14, !15}
!llvm.ident = !{!16}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "test", scope: !2, file: !7, line: 19, type: !8, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/test/metadata-temp/test.c", directory: "/home/test/metadata-temp/build", checksumkind: CSK_MD5, checksum: "42a993073abeaf09f20ac502c71bc245")
!4 = !{!0, !5}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "test2", scope: !2, file: !7, line: 20, type: !8, isLocal: false, isDefinition: true)
!7 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "42a993073abeaf09f20ac502c71bc245")
!8 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!9 = !{i32 7, !"Dwarf Version", i32 5}
!10 = !{i32 2, !"Debug Info Version", i32 3}
!11 = !{i32 1, !"wchar_size", i32 4}
!12 = !{i32 7, !"PIC Level", i32 2}
!13 = !{i32 7, !"PIE Level", i32 2}
!14 = !{i32 7, !"uwtable", i32 1}
!15 = !{i32 7, !"frame-pointer", i32 2}
!16 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!17 = distinct !DISubprogram(name: "main", scope: !7, file: !7, line: 29, type: !18, scopeLine: 30, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !20)
!18 = !DISubroutineType(types: !19)
!19 = !{!8}
!20 = !{}
!21 = !DILocalVariable(name: "node", scope: !17, file: !7, line: 34, type: !22)
!22 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !7, line: 23, size: 96, elements: !23)
!23 = !{!24, !29, !30}
!24 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !22, file: !7, line: 25, baseType: !25, size: 32)
!25 = !DICompositeType(tag: DW_TAG_array_type, baseType: !26, size: 32, elements: !27)
!26 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!27 = !{!28}
!28 = !DISubrange(count: 4)
!29 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !22, file: !7, line: 26, baseType: !8, size: 32, offset: 32)
!30 = !DIDerivedType(tag: DW_TAG_member, name: "field3", scope: !22, file: !7, line: 27, baseType: !8, size: 32, offset: 64)
!31 = !DILocation(line: 34, column: 20, scope: !17)
!32 = !DILocation(line: 35, column: 7, scope: !17)
!33 = !DILocation(line: 35, column: 14, scope: !17)
!34 = !DILocation(line: 36, column: 7, scope: !17)
!35 = !DILocation(line: 36, column: 14, scope: !17)
!36 = !DILocation(line: 37, column: 14, scope: !17)
!37 = !DILocation(line: 37, column: 9, scope: !17)
!38 = !DILocation(line: 37, column: 2, scope: !17)
!39 = !DILocation(line: 38, column: 30, scope: !17)
!40 = !DILocation(line: 38, column: 25, scope: !17)
!41 = !DILocation(line: 38, column: 2, scope: !17)
!42 = !DILocation(line: 39, column: 33, scope: !17)
!43 = !DILocation(line: 39, column: 5, scope: !17)
!44 = !DILocation(line: 40, column: 33, scope: !17)
!45 = !DILocation(line: 40, column: 5, scope: !17)
!46 = !DILocation(line: 41, column: 1, scope: !17)
