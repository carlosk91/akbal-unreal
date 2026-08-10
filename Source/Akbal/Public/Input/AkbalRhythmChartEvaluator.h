// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/AkbalRhythmChartTypes.h"
#include "Input/AkbalRhythmJudgmentTypes.h"

class AKBAL_API FAkbalRhythmChartEvaluator
{
public:
	static FAkbalRhythmJudgmentResult JudgeInputAgainstChart(
		float AdjustedInputSeconds,
		EAkbalRhythmLane Lane,
		EAkbalRhythmInstrument Instrument,
		const TArray<FAkbalRhythmChartNoteState>& ChartStates,
		const FAkbalRhythmTimingWindows& Windows,
		int32& OutNoteIndex);

	static float GetNoteApproachProgress(float CurrentSeconds, float TargetSeconds, float ApproachSeconds);

	static bool ShouldAutoMissNote(float CurrentSeconds, float TargetSeconds, const FAkbalRhythmTimingWindows& Windows);

	static bool IsNoteWithinPreviewWindow(float CurrentSeconds, float TargetSeconds, float PreviewSeconds);

	static bool IsNoteVisible(
		float CurrentSeconds,
		float TargetSeconds,
		float PreviewSeconds,
		float ApproachSeconds);

	/** Earliest required note that has not been consumed or missed and is still hittable. */
	static bool FindNextRequiredNote(
		const TArray<FAkbalRhythmChartNoteState>& ChartStates,
		float CurrentSeconds,
		const FAkbalRhythmTimingWindows& Windows,
		FAkbalRhythmChartNote& OutNote);
};
