; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.teststruct = type { [4 x i8], i32, i32 }

@.str = private unnamed_addr constant [8 x i8] c"abcdefg\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"field1: %s\0A\00", align 1
@.str.2 = private unnamed_addr constant [12 x i8] c"field2: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [12 x i8] c"field3: %d\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !10 {
  %1 = alloca %struct.teststruct, align 4
  call void @llvm.dbg.declare(metadata %struct.teststruct* %1, metadata !15, metadata !DIExpression()), !dbg !25
  %2 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !26
  store i32 1234, i32* %2, align 4, !dbg !27
  %3 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !28
  store i32 5678, i32* %3, align 4, !dbg !29
  %4 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !30
  %5 = getelementptr inbounds [4 x i8], [4 x i8]* %4, i64 0, i64 0, !dbg !31
  %6 = call i8* @strcpy(i8* noundef %5, i8* noundef getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i64 0, i64 0)) #4, !dbg !32
  %7 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !33
  %8 = getelementptr inbounds [4 x i8], [4 x i8]* %7, i64 0, i64 0, !dbg !34
  %9 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i64 0, i64 0), i8* noundef %8), !dbg !35
  %10 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !36
  %11 = load i32, i32* %10, align 4, !dbg !36
  %12 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i64 0, i64 0), i32 noundef %11), !dbg !37
  %13 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !38
  %14 = load i32, i32* %13, align 4, !dbg !38
  %15 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i64 0, i64 0), i32 noundef %14), !dbg !39
  ret i32 0, !dbg !40
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

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 14.0.0-1ubuntu1.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/test/metadata-temp", checksumkind: CSK_MD5, checksum: "c68c6c9f6fcc28f2174e676e5a5d80ae")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 1}
!8 = !{i32 7, !"frame-pointer", i32 2}
!9 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 28, type: !11, scopeLine: 29, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !14)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !DILocalVariable(name: "node", scope: !10, file: !1, line: 33, type: !16)
!16 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !1, line: 22, size: 96, elements: !17)
!17 = !{!18, !23, !24}
!18 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !16, file: !1, line: 24, baseType: !19, size: 32)
!19 = !DICompositeType(tag: DW_TAG_array_type, baseType: !20, size: 32, elements: !21)
!20 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!21 = !{!22}
!22 = !DISubrange(count: 4)
!23 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !16, file: !1, line: 25, baseType: !13, size: 32, offset: 32)
!24 = !DIDerivedType(tag: DW_TAG_member, name: "field3", scope: !16, file: !1, line: 26, baseType: !13, size: 32, offset: 64)
!25 = !DILocation(line: 33, column: 20, scope: !10)
!26 = !DILocation(line: 34, column: 7, scope: !10)
!27 = !DILocation(line: 34, column: 14, scope: !10)
!28 = !DILocation(line: 35, column: 7, scope: !10)
!29 = !DILocation(line: 35, column: 14, scope: !10)
!30 = !DILocation(line: 36, column: 14, scope: !10)
!31 = !DILocation(line: 36, column: 9, scope: !10)
!32 = !DILocation(line: 36, column: 2, scope: !10)
!33 = !DILocation(line: 37, column: 30, scope: !10)
!34 = !DILocation(line: 37, column: 25, scope: !10)
!35 = !DILocation(line: 37, column: 2, scope: !10)
!36 = !DILocation(line: 38, column: 33, scope: !10)
!37 = !DILocation(line: 38, column: 5, scope: !10)
!38 = !DILocation(line: 39, column: 33, scope: !10)
!39 = !DILocation(line: 39, column: 5, scope: !10)
!40 = !DILocation(line: 40, column: 1, scope: !10)
