// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "AkbalRhythmSpikeWidget.generated.h"

class UAkbalMusicConductorSubsystem;
class UBorder;
class UButton;
class UHorizontalBox;
class UProgressBar;
class UTextBlock;
class UVerticalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAkbalRhythmSpikeTap, const FAkbalRhythmJudgmentResult&, Result);

UCLASS()
class AKBAL_API UAkbalRhythmSpikeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureSpike(float InBeatsPerMinute, int32 InBeatsPerBar);

	UPROPERTY(BlueprintAssignable, Category = "Akbal|Spike")
	FOnAkbalRhythmSpikeTap OnTapSubmitted;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void BuildWidgetTree();
	void RebuildBeatIndicators();
	void RefreshVisuals(const struct FAkbalConductorDebugSnapshot& Snapshot);
	void ApplyJudgmentFeedback(EAkbalRhythmJudgment Judgment);
	FLinearColor GetJudgmentColor(EAkbalRhythmJudgment Judgment) const;
	FText FormatJudgmentText(EAkbalRhythmJudgment Judgment) const;

	UFUNCTION()
	void HandleTapClicked();

	UFUNCTION()
	void HandlePauseClicked();

	UFUNCTION()
	void HandleRestartClicked();

	UPROPERTY()
	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;

	UPROPERTY()
	TObjectPtr<UVerticalBox> RootBox;

	UPROPERTY()
	TObjectPtr<UTextBlock> StateText;

	UPROPERTY()
	TObjectPtr<UTextBlock> PositionText;

	UPROPERTY()
	TObjectPtr<UHorizontalBox> BeatIndicatorRow;

	UPROPERTY()
	TArray<TObjectPtr<UBorder>> BeatIndicators;

	UPROPERTY()
	TObjectPtr<UProgressBar> BeatProgressBar;

	UPROPERTY()
	TObjectPtr<UBorder> TapButtonBorder;

	UPROPERTY()
	TObjectPtr<UTextBlock> JudgmentText;

	UPROPERTY()
	TObjectPtr<UTextBlock> DeltaText;

	UPROPERTY()
	TObjectPtr<UTextBlock> LatencyText;

	UPROPERTY()
	TObjectPtr<UBorder> JudgmentFlashOverlay;

	float SpikeBeatsPerMinute = 120.f;
	int32 SpikeBeatsPerBar = 4;
	float JudgmentFlashTimeRemaining = 0.f;
	bool bWidgetBuilt = false;
};
