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

#include "Components/TextBlock.h"

#include "Components/VerticalBox.h"

#include "Components/VerticalBoxSlot.h"

#include "Engine/Engine.h"

#include "GameFramework/PlayerController.h"

#include "Input/AkbalRhythmChartEvaluator.h"

#include "Input/AkbalRhythmChartTypes.h"

#include "Kismet/GameplayStatics.h"

#include "Rendering/DrawElements.h"

#include "Styling/CoreStyle.h"



namespace AkbalRhythmGameStyle

{

	static FSlateBrush MakeBoxBrush(const FLinearColor& Color)

	{

		FSlateBrush Brush;

		Brush.DrawAs = ESlateBrushDrawType::Box;

		Brush.TintColor = FSlateColor(Color);

		Brush.ImageSize = FVector2D(1.f, 1.f);

		return Brush;

	}

}



UAkbalRhythmSpikeWidget::UAkbalRhythmSpikeWidget(const FObjectInitializer& ObjectInitializer)

	: Super(ObjectInitializer)

{

}



TSharedRef<SWidget> UAkbalRhythmSpikeWidget::RebuildWidget()

{

	if (WidgetTree && !bWidgetBuilt)

	{

		BuildWidgetTree();

	}



	if (WidgetTree && WidgetTree->RootWidget)

	{

		return WidgetTree->RootWidget->TakeWidget();

	}



	return Super::RebuildWidget();

}



void UAkbalRhythmSpikeWidget::NativeConstruct()

{

	Super::NativeConstruct();



	Conductor = UAkbalMusicConductorSubsystem::Get(this);

	BuildChart();

	RefreshLatencyLabel();

	SetIsFocusable(false);

}



void UAkbalRhythmSpikeWidget::ConfigureSpike(float InBeatsPerMinute, int32 InBeatsPerBar)

{

	SpikeBeatsPerMinute = InBeatsPerMinute;

	SpikeBeatsPerBar = FMath::Max(1, InBeatsPerBar);

	BuildChart();

}



void UAkbalRhythmSpikeWidget::RestartChart()

{

	BuildChart();

	if (JudgmentText)

	{

		JudgmentText->SetText(FText::FromString(TEXT("Hit arrow keys when notes reach the center")));

		JudgmentText->SetColorAndOpacity(FSlateColor(FLinearColor::White));

	}

}



void UAkbalRhythmSpikeWidget::SetActiveInstrument(EAkbalRhythmInstrument Instrument)
{
	ActiveInstrument = Instrument;
}



void UAkbalRhythmSpikeWidget::CycleActiveInstrument(int32 Delta)

{

	const int32 CurrentIndex = FAkbalRhythmInstrumentLibrary::IndexFromInstrument(ActiveInstrument);

	const int32 NextIndex = (CurrentIndex + Delta + FAkbalRhythmInstrumentLibrary::InstrumentCount)

		% FAkbalRhythmInstrumentLibrary::InstrumentCount;

	SetActiveInstrument(FAkbalRhythmInstrumentLibrary::InstrumentFromIndex(NextIndex));

}



void UAkbalRhythmSpikeWidget::BuildChart()

{

	ChartStates.Reset();

	const TArray<FAkbalRhythmChartNote> Notes = FAkbalRhythmChartBuilder::BuildSpikeTestChart(SpikeBeatsPerMinute);



	for (const FAkbalRhythmChartNote& Note : Notes)

	{

		FAkbalRhythmChartNoteState State;

		State.Note = Note;

		ChartStates.Add(State);

	}

}



void UAkbalRhythmSpikeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)

{

	Super::NativeTick(MyGeometry, InDeltaTime);



	if (!Conductor)

	{

		return;

	}



	const float CurrentSeconds = Conductor->GetSecondsSinceTransportStart();

	UpdateChartState(CurrentSeconds);



	if (StatusText)

	{

		const FAkbalMusicalPosition Position = Conductor->GetMusicalPosition();

		StatusText->SetText(FText::FromString(FString::Printf(

			TEXT("Bar %d | Beat %d/%d | %.1f BPM | Latency %+.0f ms"),

			Position.Bar,

			Position.Beat,

			SpikeBeatsPerBar,

			Conductor->GetBeatsPerMinute(),

			Conductor->GetInputLatencyOffsetMs())));
	}

	RefreshGameplayLabels(CurrentSeconds);

	if (JudgmentFlashTimeRemaining > 0.f)

	{

		JudgmentFlashTimeRemaining = FMath::Max(0.f, JudgmentFlashTimeRemaining - InDeltaTime);

	}

	Invalidate(EInvalidateWidget::Paint);

}



void UAkbalRhythmSpikeWidget::UpdateChartState(float CurrentSeconds)

{

	if (!Conductor)

	{

		return;

	}



	const FAkbalRhythmTimingWindows& Windows = Conductor->TimingWindows;

	for (FAkbalRhythmChartNoteState& State : ChartStates)

	{

		if (State.bConsumed || State.bMissed || !State.Note.bRequired)

		{

			continue;

		}



		if (FAkbalRhythmChartEvaluator::ShouldAutoMissNote(CurrentSeconds, State.Note.TargetSeconds, Windows))

		{

			State.bMissed = true;

		}

	}

}



bool UAkbalRhythmSpikeWidget::ProcessLaneInput(EAkbalRhythmLane Lane)

{

	if (!Conductor)

	{

		return false;

	}



	const float RawInputSeconds = Conductor->GetSecondsSinceTransportStart();

	const float AdjustedInputSeconds = Conductor->ApplyLatencyCompensation(RawInputSeconds);



	int32 MatchedNoteIndex = INDEX_NONE;

	LastJudgment = FAkbalRhythmChartEvaluator::JudgeInputAgainstChart(

		AdjustedInputSeconds,

		Lane,

		ActiveInstrument,

		ChartStates,

		Conductor->TimingWindows,

		MatchedNoteIndex);

	LastJudgment.InputSeconds = RawInputSeconds;



	if (MatchedNoteIndex != INDEX_NONE)

	{

		ChartStates[MatchedNoteIndex].bConsumed = true;

	}



	ApplyJudgmentFeedback(LastJudgment.Judgment);

	OnTapSubmitted.Broadcast(LastJudgment);

	return MatchedNoteIndex != INDEX_NONE;

}



void UAkbalRhythmSpikeWidget::BuildWidgetTree()

{

	if (!WidgetTree || bWidgetBuilt)

	{

		return;

	}



	RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));

	WidgetTree->RootWidget = RootCanvas;



	auto MakeText = [this](const FName& Name, int32 FontSize) -> UTextBlock*

	{

		UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), Name);

		FSlateFontInfo Font = Text->GetFont();

		Font.Size = FontSize;

		Text->SetFont(Font);

		Text->SetColorAndOpacity(FSlateColor(FLinearColor::White));

		return Text;

	};



	auto AddButton = [this](const FName& Name, const FString& Label) -> UButton*

	{

		UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);

		UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());

		Border->SetBrush(AkbalRhythmGameStyle::MakeBoxBrush(FLinearColor(0.12f, 0.18f, 0.28f, 0.9f)));

		Border->SetPadding(FMargin(12.f, 6.f));

		UTextBlock* LabelText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

		FSlateFontInfo Font = LabelText->GetFont();

		Font.Size = 14;

		LabelText->SetFont(Font);

		LabelText->SetText(FText::FromString(Label));

		LabelText->SetJustification(ETextJustify::Center);

		Border->SetContent(LabelText);

		Button->SetContent(Border);

		return Button;

	};



	UBorder* MenuPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("MenuPanel"));

	MenuPanel->SetBrush(AkbalRhythmGameStyle::MakeBoxBrush(FLinearColor(0.02f, 0.04f, 0.08f, 0.85f)));

	MenuPanel->SetPadding(FMargin(12.f));



	UVerticalBox* MenuBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MenuBox"));

	MenuPanel->SetContent(MenuBox);



	StatusText = MakeText(TEXT("StatusText"), 14);

	if (UVerticalBoxSlot* StatusSlot = MenuBox->AddChildToVerticalBox(StatusText))

	{

		StatusSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));

	}



	InstrumentText = MakeText(TEXT("InstrumentText"), 16);

	if (UVerticalBoxSlot* InstrumentSlot = MenuBox->AddChildToVerticalBox(InstrumentText))

	{

		InstrumentSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));

	}



	ControlsText = MakeText(TEXT("ControlsText"), 12);

	ControlsText->SetText(FText::FromString(

		TEXT("Arrows: lane input | Q/E: switch instrument (chart keeps progress)\nGhost notes = other instruments | P: pause | R: restart")));

	if (UVerticalBoxSlot* ControlsSlot = MenuBox->AddChildToVerticalBox(ControlsText))

	{

		ControlsSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));

	}



	JudgmentText = MakeText(TEXT("JudgmentText"), 18);

	JudgmentText->SetText(FText::FromString(TEXT("Hit arrow keys when notes reach the center")));

	if (UVerticalBoxSlot* JudgmentSlot = MenuBox->AddChildToVerticalBox(JudgmentText))

	{

		JudgmentSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));

	}



	UTextBlock* LatencyLabel = MakeText(TEXT("LatencyLabel"), 13);

	LatencyLabel->SetText(FText::FromString(TEXT("Input Latency Compensation")));

	if (UVerticalBoxSlot* LatencyLabelSlot = MenuBox->AddChildToVerticalBox(LatencyLabel))

	{

		LatencyLabelSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));

	}



	UHorizontalBox* LatencyRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("LatencyRow"));

	if (UVerticalBoxSlot* LatencyRowSlot = MenuBox->AddChildToVerticalBox(LatencyRow))

	{

		LatencyRowSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));

	}



	if (UButton* DecreaseButton = AddButton(TEXT("LatencyDecrease"), TEXT("-10ms")))

	{

		DecreaseButton->OnClicked.AddDynamic(this, &UAkbalRhythmSpikeWidget::HandleLatencyDecrease);

		LatencyRow->AddChildToHorizontalBox(DecreaseButton);

	}



	LatencyValueText = MakeText(TEXT("LatencyValueText"), 14);

	LatencyValueText->SetJustification(ETextJustify::Center);

	if (UHorizontalBoxSlot* ValueSlot = LatencyRow->AddChildToHorizontalBox(LatencyValueText))

	{

		ValueSlot->SetPadding(FMargin(8.f, 0.f));

	}



	if (UButton* IncreaseButton = AddButton(TEXT("LatencyIncrease"), TEXT("+10ms")))

	{

		IncreaseButton->OnClicked.AddDynamic(this, &UAkbalRhythmSpikeWidget::HandleLatencyIncrease);

		LatencyRow->AddChildToHorizontalBox(IncreaseButton);

	}



	if (UButton* PauseButton = AddButton(TEXT("PauseButton"), TEXT("Pause")))

	{

		PauseButton->OnClicked.AddDynamic(this, &UAkbalRhythmSpikeWidget::HandlePauseClicked);

		MenuBox->AddChildToVerticalBox(PauseButton);

	}



	if (UButton* RestartButton = AddButton(TEXT("RestartButton"), TEXT("Restart")))

	{

		RestartButton->OnClicked.AddDynamic(this, &UAkbalRhythmSpikeWidget::HandleRestartClicked);

		if (UVerticalBoxSlot* RestartSlot = MenuBox->AddChildToVerticalBox(RestartButton))

		{

			RestartSlot->SetPadding(FMargin(0.f, 6.f, 0.f, 0.f));

		}

	}



	if (UCanvasPanelSlot* MenuSlot = RootCanvas->AddChildToCanvas(MenuPanel))

	{

		MenuSlot->SetAnchors(FAnchors(1.f, 0.f, 1.f, 0.f));

		MenuSlot->SetAlignment(FVector2D(1.f, 0.f));

		MenuSlot->SetPosition(FVector2D(-16.f, 16.f));

		MenuSlot->SetAutoSize(true);

		MenuSlot->SetZOrder(20);

	}



	bWidgetBuilt = true;

}



void UAkbalRhythmSpikeWidget::RefreshLatencyLabel() const

{

	if (LatencyValueText && Conductor)

	{

		LatencyValueText->SetText(FText::FromString(FString::Printf(

			TEXT("%+.0f ms"),

			Conductor->GetInputLatencyOffsetMs())));

	}

}



void UAkbalRhythmSpikeWidget::RefreshGameplayLabels(float CurrentSeconds)
{
	if (!InstrumentText)
	{
		return;
	}

	const FLinearColor ActiveColor = FAkbalRhythmInstrumentLibrary::GetPrimaryColor(ActiveInstrument);
	FAkbalRhythmChartNote NextNote;
	if (Conductor && FAkbalRhythmChartEvaluator::FindNextRequiredNote(
		ChartStates, CurrentSeconds, Conductor->TimingWindows, NextNote))
	{
		const FLinearColor NextColor = FAkbalRhythmInstrumentLibrary::GetPrimaryColor(NextNote.Instrument);
		const bool bNextMatchesActive = NextNote.Instrument == ActiveInstrument;
		InstrumentText->SetText(FText::FromString(FString::Printf(
			TEXT("Playing: %s | Next: %s + %s arrow%s"),
			*FAkbalRhythmInstrumentLibrary::GetDisplayName(ActiveInstrument),
			*FAkbalRhythmInstrumentLibrary::GetDisplayName(NextNote.Instrument),
			*FAkbalRhythmLaneInput::GetArrowKeyLabel(NextNote.Lane),
			bNextMatchesActive ? TEXT("") : TEXT(" (switch Q/E)"))));
		InstrumentText->SetColorAndOpacity(FSlateColor(bNextMatchesActive ? ActiveColor : NextColor));
	}
	else
	{
		InstrumentText->SetText(FText::FromString(FString::Printf(
			TEXT("Playing: %s  (Q/E to switch)"),
			*FAkbalRhythmInstrumentLibrary::GetDisplayName(ActiveInstrument))));
		InstrumentText->SetColorAndOpacity(FSlateColor(ActiveColor));
	}
}



FVector2D UAkbalRhythmSpikeWidget::GetLanePosition(

	const FVector2D& Center,

	const FVector2D& Size,

	EAkbalRhythmLane Lane,

	float Progress) const

{

	FVector2D Start = Center;

	const float EdgePadding = 48.f;



	switch (Lane)

	{

	case EAkbalRhythmLane::LeftToRight:

		Start = FVector2D(EdgePadding, Center.Y);

		break;

	case EAkbalRhythmLane::RightToLeft:

		Start = FVector2D(Size.X - EdgePadding, Center.Y);

		break;

	case EAkbalRhythmLane::TopToBottom:

		Start = FVector2D(Center.X, EdgePadding);

		break;

	case EAkbalRhythmLane::BottomToTop:

		Start = FVector2D(Center.X, Size.Y - EdgePadding);

		break;

	default:

		break;

	}



	return FMath::Lerp(Start, Center, Progress);

}



void UAkbalRhythmSpikeWidget::DrawInstrumentNote(
	const FGeometry& AllottedGeometry,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FVector2D& Position,
	float Radius,
	EAkbalRhythmInstrument Instrument,
	const FLinearColor& Color,
	bool bGhostNote) const
{
	const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush("WhiteBrush");

	auto ScaleAlpha = [](const FLinearColor& Source, float Multiplier)
	{
		return FLinearColor(Source.R, Source.G, Source.B, Source.A * Multiplier);
	};

	auto DrawBox = [&](const FVector2D& TopLeft, const FVector2D& BoxSize, const FLinearColor& BoxColor, int32 LocalLayer)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LocalLayer,
			AllottedGeometry.ToPaintGeometry(FVector2f(BoxSize), FSlateLayoutTransform(FVector2f(TopLeft))),
			WhiteBrush,
			ESlateDrawEffect::None,
			BoxColor);
	};

	auto DrawPolyline = [&](const TArray<FVector2D>& Points, const FLinearColor& LineColor, float Thickness, int32 LocalLayer)
	{
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LocalLayer,
			AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None,
			LineColor,
			true,
			Thickness);
	};

	if (bGhostNote)
	{
		const FLinearColor OutlineColor = FLinearColor::White.CopyWithNewOpacity(GhostOutlineAlpha);
		DrawBox(
			Position - FVector2D(Radius + 2.f, Radius + 2.f),
			FVector2D((Radius + 2.f) * 2.f, (Radius + 2.f) * 2.f),
			OutlineColor,
			LayerId);
	}

	switch (Instrument)
	{
	case EAkbalRhythmInstrument::Drum:
		DrawBox(
			Position - FVector2D(Radius, Radius),
			FVector2D(Radius * 2.f, Radius * 2.f),
			Color,
			LayerId + 1);
		break;

	case EAkbalRhythmInstrument::Chime:
	{
		TArray<FVector2D> Diamond;
		Diamond.Add(Position + FVector2D(0.f, -Radius));
		Diamond.Add(Position + FVector2D(Radius, 0.f));
		Diamond.Add(Position + FVector2D(0.f, Radius));
		Diamond.Add(Position + FVector2D(-Radius, 0.f));
		Diamond.Add(Position + FVector2D(0.f, -Radius));
		DrawPolyline(Diamond, Color, bGhostNote ? 3.f : 4.f, LayerId + 1);
		DrawBox(
			Position - FVector2D(Radius * 0.45f, Radius * 0.45f),
			FVector2D(Radius * 0.9f, Radius * 0.9f),
			ScaleAlpha(Color, 0.5f),
			LayerId + 2);
		break;
	}

	case EAkbalRhythmInstrument::String:
		DrawBox(
			Position - FVector2D(Radius * 1.35f, Radius * 0.65f),
			FVector2D(Radius * 2.7f, Radius * 1.3f),
			Color,
			LayerId + 1);
		break;

	case EAkbalRhythmInstrument::Wind:
	default:
	{
		TArray<FVector2D> Triangle;
		Triangle.Add(Position + FVector2D(0.f, -Radius));
		Triangle.Add(Position + FVector2D(Radius * 0.95f, Radius * 0.85f));
		Triangle.Add(Position + FVector2D(-Radius * 0.95f, Radius * 0.85f));
		Triangle.Add(Position + FVector2D(0.f, -Radius));
		DrawPolyline(Triangle, Color, bGhostNote ? 3.f : 4.f, LayerId + 1);
		DrawBox(
			Position - FVector2D(Radius * 0.35f, Radius * 0.35f),
			FVector2D(Radius * 0.7f, Radius * 0.7f),
			ScaleAlpha(Color, 0.45f),
			LayerId + 2);
		break;
	}
	}
}



int32 UAkbalRhythmSpikeWidget::NativePaint(

	const FPaintArgs& Args,

	const FGeometry& AllottedGeometry,

	const FSlateRect& MyCullingRect,

	FSlateWindowElementList& OutDrawElements,

	int32 LayerId,

	const FWidgetStyle& InWidgetStyle,

	bool bParentEnabled) const

{

	const FVector2D Size = AllottedGeometry.GetLocalSize();

	const FVector2D Center(Size.X * 0.5f, Size.Y * 0.5f);

	const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush("WhiteBrush");



	auto DrawLine = [&](const FVector2D& Start, const FVector2D& End, const FLinearColor& Color, float Thickness)

	{

		TArray<FVector2D> Points;

		Points.Add(Start);

		Points.Add(End);

		FSlateDrawElement::MakeLines(

			OutDrawElements,

			LayerId + 1,

			AllottedGeometry.ToPaintGeometry(),

			Points,

			ESlateDrawEffect::None,

			Color,

			true,

			Thickness);

	};



	auto DrawCircle = [&](const FVector2D& Position, float Radius, const FLinearColor& Color, int32 LocalLayer)

	{

		const FVector2f TopLeft(Position.X - Radius, Position.Y - Radius);

		const FVector2f BoxSize(Radius * 2.f, Radius * 2.f);

		FSlateDrawElement::MakeBox(

			OutDrawElements,

			LocalLayer,

			AllottedGeometry.ToPaintGeometry(BoxSize, FSlateLayoutTransform(TopLeft)),

			WhiteBrush,

			ESlateDrawEffect::None,

			Color);

	};



	const FLinearColor ActiveLaneColor = FAkbalRhythmInstrumentLibrary::GetPrimaryColor(ActiveInstrument).CopyWithNewOpacity(0.65f);

	DrawLine(FVector2D(48.f, Center.Y), Center, ActiveLaneColor, 3.f);

	DrawLine(FVector2D(Size.X - 48.f, Center.Y), Center, ActiveLaneColor, 3.f);

	DrawLine(FVector2D(Center.X, 48.f), Center, ActiveLaneColor, 3.f);

	DrawLine(FVector2D(Center.X, Size.Y - 48.f), Center, ActiveLaneColor, 3.f);



	FLinearColor CenterColor = FAkbalRhythmInstrumentLibrary::GetPrimaryColor(ActiveInstrument).CopyWithNewOpacity(0.22f);

	if (JudgmentFlashTimeRemaining > 0.f)

	{

		const float FlashAlpha = FMath::Clamp(JudgmentFlashTimeRemaining / 0.35f, 0.f, 1.f);

		CenterColor = FMath::Lerp(CenterColor, GetJudgmentColor(LastJudgment.Judgment).CopyWithNewOpacity(0.55f), FlashAlpha);

	}

	DrawCircle(Center, CenterZoneRadius, CenterColor, LayerId + 2);
	DrawCircle(Center, CenterZoneRadius, FAkbalRhythmInstrumentLibrary::GetPrimaryColor(ActiveInstrument).CopyWithNewOpacity(0.35f), LayerId + 3);

	const float CurrentSeconds = Conductor ? Conductor->GetSecondsSinceTransportStart() : 0.f;
	const int32 GhostNoteLayer = LayerId + 5;
	const int32 ActiveNoteLayer = LayerId + 8;

	auto DrawVisibleNote = [&](const FAkbalRhythmChartNoteState& State, bool bActiveInstrument, int32 LocalLayer)
	{
		const float Progress = FAkbalRhythmChartEvaluator::GetNoteApproachProgress(
			CurrentSeconds, State.Note.TargetSeconds, NoteApproachSeconds);

		if (Progress >= 1.f)
		{
			return;
		}

		const FVector2D NotePosition = GetLanePosition(Center, Size, State.Note.Lane, Progress);
		const FLinearColor BaseColor = FAkbalRhythmInstrumentLibrary::GetPrimaryColor(State.Note.Instrument);
		const float Radius = bActiveInstrument ? NoteRadius : NoteRadius * GhostNoteScale;
		const FLinearColor NoteColor = bActiveInstrument
			? BaseColor
			: BaseColor.CopyWithNewOpacity(GhostNoteAlpha);
		DrawInstrumentNote(
			AllottedGeometry,
			OutDrawElements,
			LocalLayer,
			NotePosition,
			Radius,
			State.Note.Instrument,
			NoteColor,
			!bActiveInstrument);
	};

	for (const FAkbalRhythmChartNoteState& State : ChartStates)
	{
		if (State.bConsumed || State.bMissed || !State.Note.bRequired)
		{
			continue;
		}

		if (State.Note.Instrument == ActiveInstrument)
		{
			continue;
		}

		DrawVisibleNote(State, false, GhostNoteLayer);
	}

	for (const FAkbalRhythmChartNoteState& State : ChartStates)
	{
		if (State.bConsumed || State.bMissed || !State.Note.bRequired)
		{
			continue;
		}

		if (State.Note.Instrument != ActiveInstrument)
		{
			continue;
		}

		DrawVisibleNote(State, true, ActiveNoteLayer);
	}

	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId + 10, InWidgetStyle, bParentEnabled);
}



void UAkbalRhythmSpikeWidget::ApplyJudgmentFeedback(EAkbalRhythmJudgment Judgment)

{

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

	case EAkbalRhythmJudgment::Perfect: return FLinearColor(1.f, 0.84f, 0.2f, 1.f);

	case EAkbalRhythmJudgment::Good: return FLinearColor(0.35f, 0.9f, 0.45f, 1.f);

	case EAkbalRhythmJudgment::Pass: return FLinearColor(0.95f, 0.75f, 0.2f, 1.f);

	default: return FLinearColor(0.95f, 0.3f, 0.3f, 1.f);

	}

}



FText UAkbalRhythmSpikeWidget::FormatJudgmentText(EAkbalRhythmJudgment Judgment) const

{

	switch (Judgment)

	{

	case EAkbalRhythmJudgment::Perfect: return FText::FromString(TEXT("PERFECT"));

	case EAkbalRhythmJudgment::Good: return FText::FromString(TEXT("GOOD"));

	case EAkbalRhythmJudgment::Pass: return FText::FromString(TEXT("PASS"));

	default: return FText::FromString(TEXT("MISS"));

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

	RestartChart();

}



void UAkbalRhythmSpikeWidget::HandleLatencyDecrease()

{

	if (!Conductor)

	{

		return;

	}



	Conductor->SetInputLatencyOffsetMs(Conductor->GetInputLatencyOffsetMs() - 10.f);

	RefreshLatencyLabel();

}



void UAkbalRhythmSpikeWidget::HandleLatencyIncrease()

{

	if (!Conductor)

	{

		return;

	}



	Conductor->SetInputLatencyOffsetMs(Conductor->GetInputLatencyOffsetMs() + 10.f);

	RefreshLatencyLabel();

}



