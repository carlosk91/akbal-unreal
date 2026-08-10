// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "Audio/AkbalMusicalPosition.h"

class AKBAL_API FAkbalRhythmJudgmentEvaluator
{
public:
	static EAkbalRhythmJudgment EvaluateDeltaMs(float AbsDeltaMs, const FAkbalRhythmTimingWindows& Windows);

	static FAkbalRhythmJudgmentResult JudgeInputAgainstSeconds(
		float InputSeconds,
		float TargetSeconds,
		const FAkbalRhythmTimingWindows& Windows);

	static FAkbalRhythmJudgmentResult JudgeInputAgainstNearestBeat(
		float InputSeconds,
		float BeatsPerMinute,
		const FAkbalRhythmTimingWindows& Windows);

	static float GetNearestBeatTargetSeconds(float InputSeconds, float BeatsPerMinute);

	static float GetSecondsPerBeat(float BeatsPerMinute);

	/**
	 * Simulates capturing an input on the next frame after the intended timestamp.
	 * Used to verify judgment is frame-rate independent when timestamps are correct.
	 */
	static float SimulateFrameDelayedTimestamp(float IntendedSeconds, float FrameDurationSeconds);
};
