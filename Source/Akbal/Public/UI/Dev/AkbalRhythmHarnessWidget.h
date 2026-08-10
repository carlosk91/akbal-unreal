// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "AkbalRhythmHarnessWidget.generated.h"



class AAkbalRhythmSpikePlayerController;

class UAkbalMusicConductorSubsystem;

class UAkbalRitualInputSession;

class UBorder;

class UButton;

class UCanvasPanel;

class UTextBlock;



/** Developer harness for F001: pause, restart, latency, BPM, instruments, note speed. */

UCLASS()

class AKBAL_API UAkbalRhythmHarnessWidget : public UUserWidget

{

	GENERATED_BODY()



public:

	void BindHarness(
		UAkbalRitualInputSession* InSession,
		UAkbalMusicConductorSubsystem* InConductor,
		AAkbalRhythmSpikePlayerController* InPlayerController);

	/** Build widget tree if it has not been built yet (safe to call before AddToViewport). */
	void EnsureHarnessBuilt();

	bool HasBuiltContent() const;

	FVector2D MeasureHarnessSize() const;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;



private:

	void BuildWidgetTree();

	void EnsureWidgetTree();

	void RefreshLabels() const;

	static constexpr float DefaultHarnessWidth = 380.f;
	static constexpr float DefaultHarnessHeight = 560.f;



	UFUNCTION()

	void HandlePauseClicked();



	UFUNCTION()

	void HandleRestartClicked();



	UFUNCTION()

	void HandleLatencyDecrease();



	UFUNCTION()

	void HandleLatencyIncrease();



	UFUNCTION()

	void HandleBpmDecrease();



	UFUNCTION()

	void HandleBpmIncrease();



	UFUNCTION()

	void HandleApproachDecrease();



	UFUNCTION()

	void HandleApproachIncrease();



	UFUNCTION()

	void HandlePreviewDecrease();



	UFUNCTION()

	void HandlePreviewIncrease();



	UFUNCTION()

	void SetInstrumentCount1();

	UFUNCTION()

	void SetInstrumentCount2();

	UFUNCTION()

	void SetInstrumentCount3();

	UFUNCTION()

	void SetInstrumentCount4();



	UFUNCTION()

	void HandleSessionChanged();



	void SetInstrumentCount(int32 Count);



	UPROPERTY()

	TObjectPtr<UAkbalRitualInputSession> Session;



	UPROPERTY()

	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;



	UPROPERTY()

	TObjectPtr<AAkbalRhythmSpikePlayerController> PlayerController;



	UPROPERTY()

	TObjectPtr<UTextBlock> StatusText;



	UPROPERTY()

	TObjectPtr<UTextBlock> JudgmentText;



	UPROPERTY()

	TObjectPtr<UTextBlock> BpmValueText;



	UPROPERTY()

	TObjectPtr<UTextBlock> InstrumentCountText;



	UPROPERTY()

	TObjectPtr<UTextBlock> ApproachValueText;



	UPROPERTY()

	TObjectPtr<UTextBlock> PreviewValueText;



	UPROPERTY()

	TObjectPtr<UTextBlock> LatencyValueText;



	bool bWidgetBuilt = false;

};


