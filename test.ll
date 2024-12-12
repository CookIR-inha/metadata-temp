; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.teststruct = type { i32, i32, i32 }

@.str = private unnamed_addr constant [12 x i8] c"field1: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"field2: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [12 x i8] c"field3: %d\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !10 {
  %1 = alloca %struct.teststruct, align 4
  call void @llvm.dbg.declare(metadata %struct.teststruct* %1, metadata !15, metadata !DIExpression()), !dbg !21
  %2 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !22
  store i32 1234, i32* %2, align 4, !dbg !23
  %3 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !24
  store i32 5678, i32* %3, align 4, !dbg !25
  %4 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 0, !dbg !26
  %5 = load i32, i32* %4, align 4, !dbg !26
  %6 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0), i32 noundef %5), !dbg !27
  %7 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 1, !dbg !28
  %8 = load i32, i32* %7, align 4, !dbg !28
  %9 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i64 0, i64 0), i32 noundef %8), !dbg !29
  %10 = getelementptr inbounds %struct.teststruct, %struct.teststruct* %1, i32 0, i32 2, !dbg !30
  %11 = load i32, i32* %10, align 4, !dbg !30
  %12 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i64 0, i64 0), i32 noundef %11), !dbg !31
  ret i32 0, !dbg !32
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare i32 @printf(i8* noundef, ...) #2

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
!15 = !DILocalVariable(name: "node", scope: !10, file: !1, line: 30, type: !16)
!16 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "teststruct", file: !1, line: 16, size: 96, elements: !17)
!17 = !{!18, !19, !20}
!18 = !DIDerivedType(tag: DW_TAG_member, name: "field1", scope: !16, file: !1, line: 19, baseType: !13, size: 32)
!19 = !DIDerivedType(tag: DW_TAG_member, name: "field2", scope: !16, file: !1, line: 20, baseType: !13, size: 32, offset: 32)
!20 = !DIDerivedType(tag: DW_TAG_member, name: "field3", scope: !16, file: !1, line: 21, baseType: !13, size: 32, offset: 64)
!21 = !DILocation(line: 30, column: 20, scope: !10)
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
