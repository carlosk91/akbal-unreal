// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/AkbalRhythmJudgmentEvaluator.h"

EAkbalRhythmJudgment FAkbalRhythmJudgmentEvaluator::EvaluateDeltaMs(float AbsDeltaMs, const FAkbalRhythmTimingWindows& Windows)
{
	if (AbsDeltaMs <= Windows.PerfectMs)
	{
		return EAkbalRhythmJudgment::Perfect;
	}
	if (AbsDeltaMs <= Windows.GoodMs)
	{
		return EAkbalRhythmJudgment::Good;
	}
	if (AbsDeltaMs <= Windows.PassMs)
	{
		return EAkbalRhythmJudgment::Pass;
	}
	return EAkbalRhythmJudgment::Miss;
}

FAkbalRhythmJudgmentResult FAkbalRhythmJudgmentEvaluator::JudgeInputAgainstSeconds(
	float InputSeconds,
	float TargetSeconds,
	const FAkbalRhythmTimingWindows& Windows)
{
	FAkbalRhythmJudgmentResult Result;
	Result.InputSeconds = InputSeconds;
	Result.TargetSeconds = TargetSeconds;
	Result.DeltaMs = (InputSeconds - TargetSeconds) * 1000.f;
	Result.Judgment = EvaluateDeltaMs(FMath::Abs(Result.DeltaMs), Windows);
	return Result;
}

FAkbalRhythmJudgmentResult FAkbalRhythmJudgmentEvaluator::JudgeInputAgainstNearestBeat(
	float InputSeconds,
	float BeatsPerMinute,
	const FAkbalRhythmTimingWindows& Windows)
{
	const float TargetSeconds = GetNearestBeatTargetSeconds(InputSeconds, BeatsPerMinute);
	return JudgeInputAgainstSeconds(InputSeconds, TargetSeconds, Windows);
}

float FAkbalRhythmJudgmentEvaluator::GetNearestBeatTargetSeconds(float InputSeconds, float BeatsPerMinute)
{
	const float SecondsPerBeat = GetSecondsPerBeat(BeatsPerMinute);
	if (SecondsPerBeat <= KINDA_SMALL_NUMBER)
	{
		return 0.f;
	}

	const float BeatIndex = FMath::RoundToFloat(InputSeconds / SecondsPerBeat);
	return BeatIndex * SecondsPerBeat;
}

float FAkbalRhythmJudgmentEvaluator::GetSecondsPerBeat(float BeatsPerMinute)
{
	if (BeatsPerMinute <= KINDA_SMALL_NUMBER)
	{
		return 0.f;
	}
	return 60.f / BeatsPerMinute;
}

float FAkbalRhythmJudgmentEvaluator::SimulateFrameDelayedTimestamp(float IntendedSeconds, float FrameDurationSeconds)
{
	return IntendedSeconds + (FrameDurationSeconds * 0.5f);
}
