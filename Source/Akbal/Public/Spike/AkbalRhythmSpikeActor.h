// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Audio/AkbalMusicalPosition.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "AkbalRhythmSpikeActor.generated.h"

class UAkbalMusicConductorSubsystem;
class UMetaSoundSource;
class UAudioComponent;
class UAkbalRhythmSpikeWidget;

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
	float SpikeBeatsPerMinute = 120.f;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	int32 SpikeBeatsPerBar = 4;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	bool bAutoStartEncounter = true;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	bool bShowDebugOverlay = false;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	bool bUseRhythmWidget = true;

	/** Optional MetaSound for manual Quartz/MetaSound alignment validation. */
	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	TObjectPtr<UMetaSoundSource> SpikeMetaSound;

private:
	void BindInput();
	void OnTapPressed();
	void OnRestartPressed();
	void OnTogglePausePressed();
	UFUNCTION()
	void HandleWidgetTap(const FAkbalRhythmJudgmentResult& Result);
	void CreateRhythmWidget();
	void DrawDebugOverlay(const FAkbalConductorDebugSnapshot& Snapshot) const;
	void TryPlayMetaSoundClick();

	UPROPERTY()
	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;

	UPROPERTY()
	TObjectPtr<UAudioComponent> MetaSoundComponent;

	UPROPERTY()
	TObjectPtr<UAkbalRhythmSpikeWidget> RhythmWidget;
};
