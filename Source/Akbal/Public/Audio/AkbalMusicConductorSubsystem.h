// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Sound/QuartzQuantizationUtilities.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "Audio/AkbalMusicalPosition.h"
#include "AkbalMusicConductorSubsystem.generated.h"

class UQuartzClockHandle;
class UQuartzSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAkbalConductorBeat, const FAkbalMusicalPosition&, Position);

UCLASS()
class AKBAL_API UAkbalMusicConductorSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UAkbalMusicConductorSubsystem* Get(const UObject* WorldContextObject);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	bool StartEncounter(float BeatsPerMinute, int32 BeatsPerBar = 4);

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	void StopEncounter(bool bCancelPendingEvents = true);

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	void PauseEncounter();

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	void ResumeEncounter();

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	bool IsEncounterRunning() const { return bEncounterRunning; }

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	bool IsEncounterPaused() const { return bEncounterPaused; }

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	float GetBeatsPerMinute() const { return ActiveBeatsPerMinute; }

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	int32 GetBeatsPerBar() const { return ActiveBeatsPerBar; }

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	FAkbalMusicalPosition GetMusicalPosition();

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	float GetSecondsSinceTransportStart();

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	FAkbalRhythmJudgmentResult JudgeInputAtSeconds(float InputSeconds);

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	float GetInputLatencyOffsetMs() const { return InputLatencyOffsetMs; }

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	void SetInputLatencyOffsetMs(float InOffsetMs);

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	float ApplyLatencyCompensation(float InputSeconds) const;

	UFUNCTION(BlueprintCallable, Category = "Akbal|Conductor")
	FAkbalConductorDebugSnapshot GetDebugSnapshot();

	UPROPERTY(BlueprintAssignable, Category = "Akbal|Conductor")
	FOnAkbalConductorBeat OnBeat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Akbal|Conductor")
	FAkbalRhythmTimingWindows TimingWindows;

private:
	static const FName EncounterClockName;

	void EnsureClockExists();
	void SubscribeToBeatEvents();
	void UnsubscribeFromBeatEvents();

	UFUNCTION()
	void HandleBeatEvent(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

	UQuartzSubsystem* GetQuartzSubsystem();
	UQuartzClockHandle* GetClockHandle();

	UPROPERTY()
	UQuartzClockHandle* ClockHandle = nullptr;

	bool bEncounterRunning = false;
	bool bEncounterPaused = false;
	float ActiveBeatsPerMinute = 120.f;
	int32 ActiveBeatsPerBar = 4;
	int32 BeatCallbackCount = 0;
	float InputLatencyOffsetMs = 0.f;
	FAkbalMusicalPosition LastBeatPosition;
	FAkbalRhythmJudgmentResult LastJudgment;
};
