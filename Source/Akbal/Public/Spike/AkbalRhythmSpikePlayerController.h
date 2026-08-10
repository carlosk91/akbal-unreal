// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "Input/AkbalRhythmChartTypes.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "Input/AkbalRitualInputTypes.h"

#include "AkbalRhythmSpikePlayerController.generated.h"



class UAkbalMusicConductorSubsystem;

class UAkbalRhythmHarnessWidget;

class UAkbalRitualInputHud;

class UAkbalRitualInputSession;

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



	UAkbalRitualInputSession* GetRitualInputSession() const { return RitualInputSession; }



protected:

	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	FAkbalRitualInputHudConfig DefaultHudConfig;



	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	bool bAutoStartEncounter = true;



	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	bool bUseRitualHud = true;



	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	bool bUseRhythmHarness = true;



	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	bool bShowDebugOverlay = false;



	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	bool bMetronomeEnabled = true;



	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	float MetronomeVolume = 0.75f;



	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	float MetronomeTickFrequencyHz = 880.f;



	UPROPERTY(EditAnywhere, Category = "Akbal|Ritual")

	float MetronomeAccentFrequencyHz = 440.f;



private:

	void InitializeSpike();

	bool CreateRitualWidgets();

	void RetryCreateRitualWidgets();

	void LogRitualUiState() const;

	void UpdateRitualHudLayout();

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

	void HandleSessionJudgment(const FAkbalRhythmJudgmentResult& Result);



	UFUNCTION()

	void HandleConductorBeat(const FAkbalMusicalPosition& Position);



	UPROPERTY()

	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;



	UPROPERTY()

	TObjectPtr<UAkbalRitualInputSession> RitualInputSession;



	UPROPERTY()

	TObjectPtr<UAkbalRitualInputHud> RitualInputHud;



	UPROPERTY()

	TObjectPtr<UAkbalRhythmHarnessWidget> RhythmHarness;



	bool bSpikeInitialized = false;

	FTimerHandle WidgetRetryHandle;
};


