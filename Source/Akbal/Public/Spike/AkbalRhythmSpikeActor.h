// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Audio/AkbalMusicalPosition.h"
#include "Input/AkbalRhythmChartTypes.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "AkbalRhythmSpikeActor.generated.h"

class UAkbalMusicConductorSubsystem;
class UMetaSoundSource;
class UAudioComponent;

UCLASS()
class AKBAL_API AAkbalRhythmSpikeActor : public AActor
{
	GENERATED_BODY()

public:
	AAkbalRhythmSpikeActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	float SpikeBeatsPerMinute = 80.f;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	int32 SpikeBeatsPerBar = 4;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	bool bAutoStartEncounter = false;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	bool bShowDebugOverlay = true;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	TObjectPtr<UMetaSoundSource> SpikeMetaSound;

private:
	void BindInput();
	void OnLanePressed(EAkbalRhythmLane Lane);
	void OnLeftLanePressed();
	void OnRightLanePressed();
	void OnUpLanePressed();
	void OnDownLanePressed();
	void OnRestartPressed();
	void OnTogglePausePressed();
	UFUNCTION()
	void HandleWidgetTap(const FAkbalRhythmJudgmentResult& Result);
	void TryInitializeSpikeUI();
	void DrawDebugOverlay(const FAkbalConductorDebugSnapshot& Snapshot) const;
	void TryPlayMetaSoundClick();

	bool bSpikeUIInitialized = false;
	FTimerHandle SpikeInitRetryHandle;

	UPROPERTY()
	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;

	UPROPERTY()
	TObjectPtr<UAudioComponent> MetaSoundComponent;
};
