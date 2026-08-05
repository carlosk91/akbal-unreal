// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/AkbalRhythmChartEvaluator.h"

#include "Input/AkbalRhythmJudgmentEvaluator.h"

FAkbalRhythmJudgmentResult FAkbalRhythmChartEvaluator::JudgeInputAgainstChart(
	float AdjustedInputSeconds,
	EAkbalRhythmLane Lane,
	EAkbalRhythmInstrument Instrument,
	const TArray<FAkbalRhythmChartNoteState>& ChartStates,
	const FAkbalRhythmTimingWindows& Windows,
	int32& OutNoteIndex)
{
	OutNoteIndex = INDEX_NONE;

	int32 BestNoteIndex = INDEX_NONE;
	float BestAbsDeltaMs = TNumericLimits<float>::Max();

	for (int32 Index = 0; Index < ChartStates.Num(); ++Index)
	{
		const FAkbalRhythmChartNoteState& State = ChartStates[Index];
		if (!State.Note.bRequired || State.bConsumed || State.bMissed)
		{
			continue;
		}

		if (State.Note.Lane != Lane || State.Note.Instrument != Instrument)
		{
			continue;
		}

		const float DeltaMs = (AdjustedInputSeconds - State.Note.TargetSeconds) * 1000.f;
		const float AbsDeltaMs = FMath::Abs(DeltaMs);
		if (AbsDeltaMs <= Windows.PassMs && AbsDeltaMs < BestAbsDeltaMs)
		{
			BestAbsDeltaMs = AbsDeltaMs;
			BestNoteIndex = Index;
		}
	}

	FAkbalRhythmJudgmentResult Result;
	Result.InputSeconds = AdjustedInputSeconds;

	if (BestNoteIndex == INDEX_NONE)
	{
		Result.Judgment = EAkbalRhythmJudgment::Miss;
		return Result;
	}

	OutNoteIndex = BestNoteIndex;
	const FAkbalRhythmChartNote& MatchedNote = ChartStates[BestNoteIndex].Note;
	return FAkbalRhythmJudgmentEvaluator::JudgeInputAgainstSeconds(
		AdjustedInputSeconds,
		MatchedNote.TargetSeconds,
		Windows);
}

float FAkbalRhythmChartEvaluator::GetNoteApproachProgress(float CurrentSeconds, float TargetSeconds, float ApproachSeconds)
{
	if (ApproachSeconds <= KINDA_SMALL_NUMBER)
	{
		return 1.f;
	}

	const float TimeUntilHit = TargetSeconds - CurrentSeconds;
	return FMath::Clamp(1.f - (TimeUntilHit / ApproachSeconds), 0.f, 1.f);
}

bool FAkbalRhythmChartEvaluator::ShouldAutoMissNote(
	float CurrentSeconds,
	float TargetSeconds,
	const FAkbalRhythmTimingWindows& Windows)
{
	const float LateMs = (CurrentSeconds - TargetSeconds) * 1000.f;
	return LateMs > Windows.PassMs;
}

bool FAkbalRhythmChartEvaluator::FindNextRequiredNote(
	const TArray<FAkbalRhythmChartNoteState>& ChartStates,
	float CurrentSeconds,
	const FAkbalRhythmTimingWindows& Windows,
	FAkbalRhythmChartNote& OutNote)
{
	const float MissCutoffSeconds = CurrentSeconds - (Windows.PassMs * 0.001f);
	float BestTargetSeconds = TNumericLimits<float>::Max();
	int32 BestIndex = INDEX_NONE;

	for (int32 Index = 0; Index < ChartStates.Num(); ++Index)
	{
		const FAkbalRhythmChartNoteState& State = ChartStates[Index];
		if (!State.Note.bRequired || State.bConsumed || State.bMissed)
		{
			continue;
		}

		if (State.Note.TargetSeconds < MissCutoffSeconds)
		{
			continue;
		}

		if (State.Note.TargetSeconds < BestTargetSeconds)
		{
			BestTargetSeconds = State.Note.TargetSeconds;
			BestIndex = Index;
		}
	}

	if (BestIndex == INDEX_NONE)
	{
		return false;
	}

	OutNote = ChartStates[BestIndex].Note;
	return true;
}
