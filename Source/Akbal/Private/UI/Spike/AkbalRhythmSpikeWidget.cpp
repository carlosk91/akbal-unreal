// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/Spike/AkbalRhythmSpikeWidget.h"

#include "Audio/AkbalMusicConductorSubsystem.h"
#include "Audio/AkbalMusicalPosition.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

namespace AkbalRhythmSpikeWidgetStyle
{
	static const FLinearColor PanelColor(0.02f, 0.03f, 0.06f, 0.92f);
	static const FLinearColor BeatIdleColor(0.15f, 0.18f, 0.24f, 1.f);
	static const FLinearColor BeatActiveColor(0.95f, 0.55f, 0.12f, 1.f);
	static const FLinearColor BeatCurrentColor(1.f, 0.82f, 0.2f, 1.f);
	static const FLinearColor TapIdleColor(0.12f, 0.45f, 0.72f, 1.f);
	static const FLinearColor TapPressedColor(0.2f, 0.65f, 0.95f, 1.f);
}

void UAkbalRhythmSpikeWidget::ConfigureSpike(float InBeatsPerMinute, int32 InBeatsPerBar)
{
	SpikeBeatsPerMinute = InBeatsPerMinute;
	SpikeBeatsPerBar = FMath::Max(1, InBeatsPerBar);

	if (bWidgetBuilt)
	{
		RebuildBeatIndicators();
	}
}

void UAkbalRhythmSpikeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Conductor = UAkbalMusicConductorSubsystem::Get(this);
	if (!bWidgetBuilt)
	{
		BuildWidgetTree();
	}
}

void UAkbalRhythmSpikeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!Conductor)
	{
		return;
	}

	RefreshVisuals(Conductor->GetDebugSnapshot());

	if (JudgmentFlashTimeRemaining > 0.f)
	{
		JudgmentFlashTimeRemaining = FMath::Max(0.f, JudgmentFlashTimeRemaining - InDeltaTime);
		if (JudgmentFlashOverlay)
		{
			const float Alpha = FMath::Clamp(JudgmentFlashTimeRemaining / 0.35f, 0.f, 1.f) * 0.45f;
			FLinearColor FlashColor = JudgmentFlashOverlay->GetBrushColor();
			FlashColor.A = Alpha;
			JudgmentFlashOverlay->SetBrushColor(FlashColor);
			JudgmentFlashOverlay->SetVisibility(Alpha > 0.01f ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

void UAkbalRhythmSpikeWidget::BuildWidgetTree()
{
	if (!WidgetTree || bWidgetBuilt)
	{
		return;
	}

	UBorder* RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("RootBorder"));
	RootBorder->SetBrushColor(AkbalRhythmSpikeWidgetStyle::PanelColor);
	RootBorder->SetPadding(FMargin(24.f));

	UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootCanvas;

	if (UCanvasPanelSlot* BorderSlot = RootCanvas->AddChildToCanvas(RootBorder))
	{
		BorderSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		BorderSlot->SetOffsets(FMargin(0.f));
	}

	RootBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootBox"));
	RootBorder->SetContent(RootBox);

	auto AddTextBlock = [this](const FName& Name, int32 FontSize, ETextJustify::Type Justify) -> UTextBlock*
	{
		UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), Name);
		FSlateFontInfo Font = Text->GetFont();
		Font.Size = FontSize;
		Text->SetFont(Font);
		Text->SetJustification(Justify);
		Text->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		return Text;
	};

	StateText = AddTextBlock(TEXT("StateText"), 20, ETextJustify::Center);
	if (UVerticalBoxSlot* StateSlot = RootBox->AddChildToVerticalBox(StateText))
	{
		StateSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
		StateSlot->SetHorizontalAlignment(HAlign_Center);
	}

	PositionText = AddTextBlock(TEXT("PositionText"), 28, ETextJustify::Center);
	if (UVerticalBoxSlot* PositionSlot = RootBox->AddChildToVerticalBox(PositionText))
	{
		PositionSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
		PositionSlot->SetHorizontalAlignment(HAlign_Center);
	}

	BeatIndicatorRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("BeatIndicatorRow"));
	if (UVerticalBoxSlot* BeatRowSlot = RootBox->AddChildToVerticalBox(BeatIndicatorRow))
	{
		BeatRowSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 12.f));
		BeatRowSlot->SetHorizontalAlignment(HAlign_Center);
	}
	RebuildBeatIndicators();

	BeatProgressBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("BeatProgressBar"));
	BeatProgressBar->SetPercent(0.f);
	BeatProgressBar->SetFillColorAndOpacity(AkbalRhythmSpikeWidgetStyle::BeatCurrentColor);
	if (UVerticalBoxSlot* ProgressSlot = RootBox->AddChildToVerticalBox(BeatProgressBar))
	{
		ProgressSlot->SetPadding(FMargin(40.f, 0.f, 40.f, 24.f));
		ProgressSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	UButton* TapButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("TapButton"));
	TapButton->OnClicked.AddDynamic(this, &UAkbalRhythmSpikeWidget::HandleTapClicked);

	TapButtonBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("TapButtonBorder"));
	TapButtonBorder->SetBrushColor(AkbalRhythmSpikeWidgetStyle::TapIdleColor);
	TapButtonBorder->SetPadding(FMargin(0.f));
	TapButton->SetContent(TapButtonBorder);

	UTextBlock* TapLabel = AddTextBlock(TEXT("TapLabel"), 42, ETextJustify::Center);
	TapLabel->SetText(FText::FromString(TEXT("TAP")));
	TapLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	TapButtonBorder->SetContent(TapLabel);

	if (UVerticalBoxSlot* TapSlot = RootBox->AddChildToVerticalBox(TapButton))
	{
		TapSlot->SetPadding(FMargin(80.f, 8.f, 80.f, 24.f));
		TapSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		TapSlot->SetHorizontalAlignment(HAlign_Center);
		TapSlot->SetVerticalAlignment(VAlign_Center);
	}

	JudgmentText = AddTextBlock(TEXT("JudgmentText"), 36, ETextJustify::Center);
	JudgmentText->SetText(FText::FromString(TEXT("Tap on the beat")));
	if (UVerticalBoxSlot* JudgmentSlot = RootBox->AddChildToVerticalBox(JudgmentText))
	{
		JudgmentSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));
		JudgmentSlot->SetHorizontalAlignment(HAlign_Center);
	}

	DeltaText = AddTextBlock(TEXT("DeltaText"), 18, ETextJustify::Center);
	DeltaText->SetColorAndOpacity(FSlateColor(FLinearColor(0.8f, 0.85f, 0.9f, 1.f)));
	if (UVerticalBoxSlot* DeltaSlot = RootBox->AddChildToVerticalBox(DeltaText))
	{
		DeltaSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
		DeltaSlot->SetHorizontalAlignment(HAlign_Center);
	}

	LatencyText = AddTextBlock(TEXT("LatencyText"), 14, ETextJustify::Center);
	LatencyText->SetColorAndOpacity(FSlateColor(FLinearColor(0.55f, 0.62f, 0.7f, 1.f)));
	if (UVerticalBoxSlot* LatencySlot = RootBox->AddChildToVerticalBox(LatencyText))
	{
		LatencySlot->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
		LatencySlot->SetHorizontalAlignment(HAlign_Center);
	}

	UHorizontalBox* ControlRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ControlRow"));
	if (UVerticalBoxSlot* ControlSlot = RootBox->AddChildToVerticalBox(ControlRow))
	{
		ControlSlot->SetHorizontalAlignment(HAlign_Center);
	}

	auto AddControlButton = [this, ControlRow](const FName& Name, const FString& Label) -> UButton*
	{
		UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);

		UBorder* ButtonBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
		ButtonBorder->SetBrushColor(FLinearColor(0.18f, 0.22f, 0.28f, 1.f));
		ButtonBorder->SetPadding(FMargin(18.f, 10.f));

		UTextBlock* ButtonLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		FSlateFontInfo Font = ButtonLabel->GetFont();
		Font.Size = 16;
		ButtonLabel->SetFont(Font);
		ButtonLabel->SetText(FText::FromString(Label));
		ButtonLabel->SetJustification(ETextJustify::Center);
		ButtonBorder->SetContent(ButtonLabel);
		Button->SetContent(ButtonBorder);

		if (UHorizontalBoxSlot* ButtonSlot = ControlRow->AddChildToHorizontalBox(Button))
		{
			ButtonSlot->SetPadding(FMargin(8.f, 0.f));
		}

		return Button;
	};

	if (UButton* PauseButton = AddControlButton(TEXT("PauseButton"), TEXT("Pause")))
	{
		PauseButton->OnClicked.AddDynamic(this, &UAkbalRhythmSpikeWidget::HandlePauseClicked);
	}

	if (UButton* RestartButton = AddControlButton(TEXT("RestartButton"), TEXT("Restart")))
	{
		RestartButton->OnClicked.AddDynamic(this, &UAkbalRhythmSpikeWidget::HandleRestartClicked);
	}

	JudgmentFlashOverlay = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("JudgmentFlashOverlay"));
	JudgmentFlashOverlay->SetVisibility(ESlateVisibility::Collapsed);
	JudgmentFlashOverlay->SetBrushColor(FLinearColor::Transparent);
	if (UCanvasPanelSlot* FlashSlot = RootCanvas->AddChildToCanvas(JudgmentFlashOverlay))
	{
		FlashSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		FlashSlot->SetOffsets(FMargin(0.f));
		FlashSlot->SetZOrder(10);
	}

	bWidgetBuilt = true;
}

void UAkbalRhythmSpikeWidget::RebuildBeatIndicators()
{
	if (!BeatIndicatorRow)
	{
		return;
	}

	BeatIndicatorRow->ClearChildren();
	BeatIndicators.Reset();

	for (int32 BeatIndex = 0; BeatIndex < SpikeBeatsPerBar; ++BeatIndex)
	{
		const FName BorderName = *FString::Printf(TEXT("BeatIndicator_%d"), BeatIndex);
		UBorder* Indicator = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), BorderName);
		Indicator->SetBrushColor(AkbalRhythmSpikeWidgetStyle::BeatIdleColor);
		Indicator->SetPadding(FMargin(0.f));

		USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		SizeBox->SetWidthOverride(48.f);
		SizeBox->SetHeightOverride(48.f);
		SizeBox->SetContent(Indicator);

		if (UHorizontalBoxSlot* IndicatorSlot = BeatIndicatorRow->AddChildToHorizontalBox(SizeBox))
		{
			IndicatorSlot->SetPadding(FMargin(8.f, 0.f));
			IndicatorSlot->SetVerticalAlignment(VAlign_Center);
		}

		BeatIndicators.Add(Indicator);
	}
}

void UAkbalRhythmSpikeWidget::RefreshVisuals(const FAkbalConductorDebugSnapshot& Snapshot)
{
	if (StateText)
	{
		const FString StateLabel = Snapshot.bPaused ? TEXT("Paused") : (Snapshot.bClockRunning ? TEXT("Running") : TEXT("Stopped"));
		StateText->SetText(FText::FromString(FString::Printf(TEXT("%s  |  %.0f BPM"), *StateLabel, Snapshot.BeatsPerMinute)));
	}

	if (PositionText)
	{
		PositionText->SetText(FText::FromString(FString::Printf(
			TEXT("Bar %d  |  Beat %d  |  %.2fs"),
			Snapshot.Position.Bar,
			Snapshot.Position.Beat,
			Snapshot.Position.SecondsSinceTransportStart)));
	}

	for (int32 BeatIndex = 0; BeatIndex < BeatIndicators.Num(); ++BeatIndex)
	{
		if (!BeatIndicators[BeatIndex])
		{
			continue;
		}

		const int32 BeatNumber = BeatIndex + 1;
		FLinearColor Color = AkbalRhythmSpikeWidgetStyle::BeatIdleColor;
		if (BeatNumber == Snapshot.Position.Beat)
		{
			Color = FMath::Lerp(
				AkbalRhythmSpikeWidgetStyle::BeatActiveColor,
				AkbalRhythmSpikeWidgetStyle::BeatCurrentColor,
				Snapshot.Position.BeatFraction);
		}
		else if (BeatNumber < Snapshot.Position.Beat)
		{
			Color = AkbalRhythmSpikeWidgetStyle::BeatActiveColor;
		}

		BeatIndicators[BeatIndex]->SetBrushColor(Color);
	}

	if (BeatProgressBar)
	{
		BeatProgressBar->SetPercent(FMath::Clamp(Snapshot.Position.BeatFraction, 0.f, 1.f));
	}

	if (LatencyText)
	{
		LatencyText->SetText(FText::FromString(FString::Printf(
			TEXT("Round-trip %.1f ms  |  Game->Audio %.1f ms  |  Beat callbacks %d"),
			Snapshot.RoundTripLatencyMs,
			Snapshot.GameToAudioLatencyMs,
			Snapshot.BeatCallbackCount)));
	}

	if (Snapshot.LastJudgment.Judgment != EAkbalRhythmJudgment::Miss || Snapshot.LastJudgment.InputSeconds > 0.f)
	{
		if (JudgmentText)
		{
			JudgmentText->SetText(FormatJudgmentText(Snapshot.LastJudgment.Judgment));
			JudgmentText->SetColorAndOpacity(FSlateColor(GetJudgmentColor(Snapshot.LastJudgment.Judgment)));
		}

		if (DeltaText)
		{
			const FString EarlyLate = Snapshot.LastJudgment.DeltaMs < 0.f ? TEXT("early") : TEXT("late");
			DeltaText->SetText(FText::FromString(FString::Printf(
				TEXT("%.1f ms %s"),
				FMath::Abs(Snapshot.LastJudgment.DeltaMs),
				*EarlyLate)));
		}
	}
}

void UAkbalRhythmSpikeWidget::ApplyJudgmentFeedback(EAkbalRhythmJudgment Judgment)
{
	if (TapButtonBorder)
	{
		TapButtonBorder->SetBrushColor(AkbalRhythmSpikeWidgetStyle::TapPressedColor);
	}

	if (JudgmentFlashOverlay)
	{
		FLinearColor FlashColor = GetJudgmentColor(Judgment);
		FlashColor.A = 0.45f;
		JudgmentFlashOverlay->SetBrushColor(FlashColor);
		JudgmentFlashOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	JudgmentFlashTimeRemaining = 0.35f;

	if (JudgmentText)
	{
		JudgmentText->SetText(FormatJudgmentText(Judgment));
		JudgmentText->SetColorAndOpacity(FSlateColor(GetJudgmentColor(Judgment)));
	}
}

FLinearColor UAkbalRhythmSpikeWidget::GetJudgmentColor(EAkbalRhythmJudgment Judgment) const
{
	switch (Judgment)
	{
	case EAkbalRhythmJudgment::Perfect:
		return FLinearColor(1.f, 0.84f, 0.2f, 1.f);
	case EAkbalRhythmJudgment::Good:
		return FLinearColor(0.35f, 0.9f, 0.45f, 1.f);
	case EAkbalRhythmJudgment::Pass:
		return FLinearColor(0.95f, 0.75f, 0.2f, 1.f);
	default:
		return FLinearColor(0.95f, 0.3f, 0.3f, 1.f);
	}
}

FText UAkbalRhythmSpikeWidget::FormatJudgmentText(EAkbalRhythmJudgment Judgment) const
{
	switch (Judgment)
	{
	case EAkbalRhythmJudgment::Perfect:
		return FText::FromString(TEXT("PERFECT"));
	case EAkbalRhythmJudgment::Good:
		return FText::FromString(TEXT("GOOD"));
	case EAkbalRhythmJudgment::Pass:
		return FText::FromString(TEXT("PASS"));
	default:
		return FText::FromString(TEXT("MISS"));
	}
}

void UAkbalRhythmSpikeWidget::HandleTapClicked()
{
	if (!Conductor)
	{
		return;
	}

	const float InputSeconds = Conductor->GetSecondsSinceTransportStart();
	const FAkbalRhythmJudgmentResult Result = Conductor->JudgeInputAtSeconds(InputSeconds);
	ApplyJudgmentFeedback(Result.Judgment);

	if (DeltaText)
	{
		const FString EarlyLate = Result.DeltaMs < 0.f ? TEXT("early") : TEXT("late");
		DeltaText->SetText(FText::FromString(FString::Printf(TEXT("%.1f ms %s"), FMath::Abs(Result.DeltaMs), *EarlyLate)));
	}

	OnTapSubmitted.Broadcast(Result);

	if (TapButtonBorder)
	{
		FTimerHandle ResetColorHandle;
		GetWorld()->GetTimerManager().SetTimer(
			ResetColorHandle,
			FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				if (TapButtonBorder)
				{
					TapButtonBorder->SetBrushColor(AkbalRhythmSpikeWidgetStyle::TapIdleColor);
				}
			}),
			0.12f,
			false);
	}
}

void UAkbalRhythmSpikeWidget::HandlePauseClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController || !Conductor)
	{
		return;
	}

	const bool bShouldPause = !PlayerController->IsPaused();
	PlayerController->SetPause(bShouldPause);

	if (bShouldPause)
	{
		Conductor->PauseEncounter();
	}
	else
	{
		Conductor->ResumeEncounter();
	}
}

void UAkbalRhythmSpikeWidget::HandleRestartClicked()
{
	if (!Conductor)
	{
		return;
	}

	Conductor->StopEncounter(true);
	Conductor->StartEncounter(SpikeBeatsPerMinute, SpikeBeatsPerBar);

	if (JudgmentText)
	{
		JudgmentText->SetText(FText::FromString(TEXT("Tap on the beat")));
		JudgmentText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}

	if (DeltaText)
	{
		DeltaText->SetText(FText::GetEmpty());
	}
}
