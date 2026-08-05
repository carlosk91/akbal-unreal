// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Input/AkbalRhythmChartTypes.h"

#include "Input/AkbalRhythmJudgmentTypes.h"

#include "AkbalRhythmSpikeWidget.generated.h"



class UAkbalMusicConductorSubsystem;

class UBorder;

class UButton;

class UCanvasPanel;

class UTextBlock;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAkbalRhythmSpikeTap, const FAkbalRhythmJudgmentResult&, Result);



UCLASS()

class AKBAL_API UAkbalRhythmSpikeWidget : public UUserWidget

{

	GENERATED_BODY()



public:

	UAkbalRhythmSpikeWidget(const FObjectInitializer& ObjectInitializer);



	void ConfigureSpike(float InBeatsPerMinute, int32 InBeatsPerBar);

	void RestartChart();

	bool ProcessLaneInput(EAkbalRhythmLane Lane);

	void SetActiveInstrument(EAkbalRhythmInstrument Instrument);

	void CycleActiveInstrument(int32 Delta);



	EAkbalRhythmInstrument GetActiveInstrument() const { return ActiveInstrument; }



	UPROPERTY(BlueprintAssignable, Category = "Akbal|Spike")

	FOnAkbalRhythmSpikeTap OnTapSubmitted;



protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual int32 NativePaint(

		const FPaintArgs& Args,

		const FGeometry& AllottedGeometry,

		const FSlateRect& MyCullingRect,

		FSlateWindowElementList& OutDrawElements,

		int32 LayerId,

		const FWidgetStyle& InWidgetStyle,

		bool bParentEnabled) const override;



private:

	void BuildWidgetTree();

	void BuildChart();

	void UpdateChartState(float CurrentSeconds);

	void RefreshLatencyLabel() const;
	void RefreshGameplayLabels(float CurrentSeconds);
	void ApplyJudgmentFeedback(EAkbalRhythmJudgment Judgment);

	FText FormatJudgmentText(EAkbalRhythmJudgment Judgment) const;

	FLinearColor GetJudgmentColor(EAkbalRhythmJudgment Judgment) const;

	FVector2D GetLanePosition(const FVector2D& Center, const FVector2D& Size, EAkbalRhythmLane Lane, float Progress) const;

	void DrawInstrumentNote(
		const FGeometry& AllottedGeometry,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FVector2D& Position,
		float Radius,
		EAkbalRhythmInstrument Instrument,
		const FLinearColor& Color,
		bool bGhostNote = false) const;



	UFUNCTION()

	void HandlePauseClicked();



	UFUNCTION()

	void HandleRestartClicked();



	UFUNCTION()

	void HandleLatencyDecrease();



	UFUNCTION()

	void HandleLatencyIncrease();



	UPROPERTY()

	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;



	UPROPERTY()

	TObjectPtr<UCanvasPanel> RootCanvas;



	UPROPERTY()

	TObjectPtr<UTextBlock> LatencyValueText;



	UPROPERTY()

	TObjectPtr<UTextBlock> StatusText;



	UPROPERTY()

	TObjectPtr<UTextBlock> InstrumentText;



	UPROPERTY()

	TObjectPtr<UTextBlock> ControlsText;



	UPROPERTY()

	TObjectPtr<UTextBlock> JudgmentText;



	TArray<FAkbalRhythmChartNoteState> ChartStates;

	FAkbalRhythmJudgmentResult LastJudgment;

	EAkbalRhythmInstrument ActiveInstrument = EAkbalRhythmInstrument::Drum;



	float SpikeBeatsPerMinute = 120.f;

	int32 SpikeBeatsPerBar = 4;

	float NoteApproachSeconds = 1.75f;

	float CenterZoneRadius = 52.f;

	float NoteRadius = 16.f;
	float GhostNoteAlpha = 0.42f;
	float GhostOutlineAlpha = 0.62f;
	float GhostNoteScale = 0.88f;
	float JudgmentFlashTimeRemaining = 0.f;

	bool bWidgetBuilt = false;

};



