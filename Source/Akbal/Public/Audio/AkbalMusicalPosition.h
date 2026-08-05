// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "AkbalMusicalPosition.generated.h"

USTRUCT(BlueprintType)
struct FAkbalMusicalPosition
{
	GENERATED_BODY()

	/** 1-based bar index. */
	UPROPERTY(BlueprintReadOnly, Category = "Music")
	int32 Bar = 1;

	/** 1-based beat index within the current bar. */
	UPROPERTY(BlueprintReadOnly, Category = "Music")
	int32 Beat = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	float BeatFraction = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	float SecondsSinceTransportStart = 0.f;

	float GetTotalBeats(int32 BeatsPerBar) const;

	static FAkbalMusicalPosition FromTotalBeats(float TotalBeats, int32 BeatsPerBar, float SecondsSinceTransportStart);

	static FAkbalMusicalPosition FromSeconds(float Seconds, float BeatsPerMinute, int32 BeatsPerBar);
};

USTRUCT(BlueprintType)
struct FAkbalConductorDebugSnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	bool bClockRunning = false;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	bool bPaused = false;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	float BeatsPerMinute = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	int32 BeatsPerBar = 4;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	FAkbalMusicalPosition Position;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	int32 BeatCallbackCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	float RoundTripLatencyMs = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	float GameToAudioLatencyMs = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	FAkbalRhythmJudgmentResult LastJudgment;
};
