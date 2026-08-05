// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "AkbalRhythmSpikePlayerController.generated.h"

class UAkbalMusicConductorSubsystem;
class UAkbalRhythmSpikeWidget;

UCLASS()
class AKBAL_API AAkbalRhythmSpikePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAkbalRhythmSpikePlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	float SpikeBeatsPerMinute = 120.f;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	int32 SpikeBeatsPerBar = 4;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	bool bAutoStartEncounter = true;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	bool bUseRhythmWidget = true;

	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")
	bool bShowDebugOverlay = true;

private:
	void InitializeSpike();
	bool CreateRhythmWidget();
	void DrawDebugOverlay() const;

	UFUNCTION()
	void OnTapPressed();

	UFUNCTION()
	void OnRestartPressed();

	UFUNCTION()
	void OnTogglePausePressed();

	UFUNCTION()
	void HandleWidgetTap(const FAkbalRhythmJudgmentResult& Result);

	UPROPERTY()
	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;

	UPROPERTY()
	TObjectPtr<UAkbalRhythmSpikeWidget> RhythmWidget;

	bool bSpikeInitialized = false;
};
