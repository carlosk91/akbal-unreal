// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "Input/AkbalRhythmChartTypes.h"

#include "Input/AkbalRhythmJudgmentTypes.h"

#include "AkbalRhythmSpikePlayerController.generated.h"



class UAkbalMusicConductorSubsystem;

class UAkbalRhythmSpikeWidget;

struct FAkbalMusicalPosition;



UCLASS()

class AKBAL_API AAkbalRhythmSpikePlayerController : public APlayerController

{

	GENERATED_BODY()



public:

	AAkbalRhythmSpikePlayerController();



	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual bool InputKey(const FInputKeyEventArgs& Params) override;



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



	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")

	bool bMetronomeEnabled = true;



	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")

	float MetronomeVolume = 0.75f;



	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")

	float MetronomeTickFrequencyHz = 880.f;



	UPROPERTY(EditAnywhere, Category = "Akbal|Spike")

	float MetronomeAccentFrequencyHz = 440.f;



private:

	void InitializeSpike();

	bool CreateRhythmWidget();

	void DrawDebugOverlay() const;

	void OnLanePressed(EAkbalRhythmLane Lane);

	void OnLeftLanePressed();

	void OnRightLanePressed();

	void OnUpLanePressed();

	void OnDownLanePressed();

	void CycleInstrument(int32 Delta);

	bool TryHandleRhythmKey(const FKey& Key, EInputEvent Event);



	UFUNCTION()

	void OnRestartPressed();



	UFUNCTION()

	void OnTogglePausePressed();



	UFUNCTION()

	void OnInstrumentPreviousPressed();



	UFUNCTION()

	void OnInstrumentNextPressed();



	UFUNCTION()

	void HandleWidgetTap(const FAkbalRhythmJudgmentResult& Result);



	UFUNCTION()

	void HandleConductorBeat(const FAkbalMusicalPosition& Position);



	UPROPERTY()

	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;



	UPROPERTY()

	TObjectPtr<UAkbalRhythmSpikeWidget> RhythmWidget;



	bool bSpikeInitialized = false;

};



