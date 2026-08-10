// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AkbalRhythmJudgmentTypes.generated.h"

UENUM(BlueprintType)
enum class EAkbalRhythmJudgment : uint8
{
	Perfect UMETA(DisplayName = "Perfect"),
	Good UMETA(DisplayName = "Good"),
	Pass UMETA(DisplayName = "Pass"),
	Miss UMETA(DisplayName = "Miss")
};

USTRUCT(BlueprintType)
struct FAkbalRhythmTimingWindows
{
	GENERATED_BODY()

	/** Half-width of the Perfect window in milliseconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
	float PerfectMs = 25.f;

	/** Half-width of the Good window in milliseconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
	float GoodMs = 50.f;

	/** Half-width of the Pass window in milliseconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
	float PassMs = 80.f;
};

USTRUCT(BlueprintType)
struct FAkbalRhythmJudgmentResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")
	EAkbalRhythmJudgment Judgment = EAkbalRhythmJudgment::Miss;

	/** Signed delta from target beat time in milliseconds (negative = early). */
	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")
	float DeltaMs = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")
	float TargetSeconds = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")
	float InputSeconds = 0.f;
};
