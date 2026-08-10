// Copyright Epic Games, Inc. All Rights Reserved.



#include "UI/Dev/AkbalRhythmHarnessWidget.h"



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

#include "Input/AkbalRhythmChartEvaluator.h"

#include "Input/AkbalRhythmChartTypes.h"

#include "Spike/AkbalRhythmSpikePlayerController.h"

#include "UI/Ritual/AkbalRitualInputSession.h"



namespace AkbalHarnessStyle

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



void UAkbalRhythmHarnessWidget::BindHarness(

	UAkbalRitualInputSession* InSession,

	UAkbalMusicConductorSubsystem* InConductor,

	AAkbalRhythmSpikePlayerController* InPlayerController)

{

	Session = InSession;

	Conductor = InConductor;

	PlayerController = InPlayerController;



	if (Session)

	{

		Session->OnSessionChanged.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandleSessionChanged);

	}

}



void UAkbalRhythmHarnessWidget::EnsureWidgetTree()
{
	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"));
	}
}

void UAkbalRhythmHarnessWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	EnsureWidgetTree();
	if (!bWidgetBuilt)
	{
		BuildWidgetTree();
	}
}

void UAkbalRhythmHarnessWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EnsureWidgetTree();
	if (!bWidgetBuilt)
	{
		BuildWidgetTree();
	}

	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(false);
	RefreshLabels();
}



void UAkbalRhythmHarnessWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)

{

	Super::NativeTick(MyGeometry, InDeltaTime);

	RefreshLabels();

}



void UAkbalRhythmHarnessWidget::BuildWidgetTree()

{

	if (!WidgetTree || bWidgetBuilt)

	{

		return;

	}



	UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("HarnessRoot"));

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

		Border->SetBrush(AkbalHarnessStyle::MakeBoxBrush(FLinearColor(0.12f, 0.18f, 0.28f, 0.9f)));

		Border->SetPadding(FMargin(10.f, 5.f));

		UTextBlock* LabelText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

		FSlateFontInfo Font = LabelText->GetFont();

		Font.Size = 13;

		LabelText->SetFont(Font);

		LabelText->SetText(FText::FromString(Label));

		LabelText->SetJustification(ETextJustify::Center);

		Border->SetContent(LabelText);

		Button->SetContent(Border);

		return Button;

	};



	auto AddLabelRow = [this](UVerticalBox* Parent, TObjectPtr<UTextBlock>& OutValueText, const FString& Label) -> UHorizontalBox*

	{

		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		UTextBlock* LabelText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

		FSlateFontInfo Font = LabelText->GetFont();

		Font.Size = 13;

		LabelText->SetFont(Font);

		LabelText->SetText(FText::FromString(Label));

		Row->AddChildToHorizontalBox(LabelText);

		OutValueText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

		OutValueText->SetFont(Font);

		if (UHorizontalBoxSlot* ValueSlot = Row->AddChildToHorizontalBox(OutValueText))

		{

			ValueSlot->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f));

		}

		if (UVerticalBoxSlot* RowSlot = Parent->AddChildToVerticalBox(Row))

		{

			RowSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));

		}

		return Row;

	};



	UBorder* MenuPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("HarnessPanel"));

	MenuPanel->SetBrush(AkbalHarnessStyle::MakeBoxBrush(FLinearColor(0.02f, 0.04f, 0.08f, 0.88f)));

	MenuPanel->SetPadding(FMargin(12.f));



	UVerticalBox* MenuBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("HarnessBox"));

	MenuPanel->SetContent(MenuBox);



	UTextBlock* Title = MakeText(TEXT("HarnessTitle"), 15);

	Title->SetText(FText::FromString(TEXT("F001 Rhythm Harness")));

	MenuBox->AddChildToVerticalBox(Title);



	StatusText = MakeText(TEXT("StatusText"), 13);

	MenuBox->AddChildToVerticalBox(StatusText);



	JudgmentText = MakeText(TEXT("JudgmentText"), 16);

	JudgmentText->SetText(FText::FromString(TEXT("Arrows: lanes | Q/E: instrument")));

	MenuBox->AddChildToVerticalBox(JudgmentText);



	UHorizontalBox* BpmRow = AddLabelRow(MenuBox, BpmValueText, TEXT("BPM:"));

	if (UButton* BpmDown = AddButton(TEXT("BpmDown"), TEXT("-5")))

	{

		BpmDown->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandleBpmDecrease);

		BpmRow->AddChildToHorizontalBox(BpmDown);

	}

	if (UButton* BpmUp = AddButton(TEXT("BpmUp"), TEXT("+5")))

	{

		BpmUp->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandleBpmIncrease);

		BpmRow->AddChildToHorizontalBox(BpmUp);

	}



	UHorizontalBox* InstrumentRow = AddLabelRow(MenuBox, InstrumentCountText, TEXT("Instruments:"));

	if (UButton* Inst1 = AddButton(TEXT("Inst1"), TEXT("1")))

	{

		Inst1->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::SetInstrumentCount1);

		InstrumentRow->AddChildToHorizontalBox(Inst1);

	}

	if (UButton* Inst2 = AddButton(TEXT("Inst2"), TEXT("2")))

	{

		Inst2->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::SetInstrumentCount2);

		InstrumentRow->AddChildToHorizontalBox(Inst2);

	}

	if (UButton* Inst3 = AddButton(TEXT("Inst3"), TEXT("3")))

	{

		Inst3->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::SetInstrumentCount3);

		InstrumentRow->AddChildToHorizontalBox(Inst3);

	}

	if (UButton* Inst4 = AddButton(TEXT("Inst4"), TEXT("4")))

	{

		Inst4->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::SetInstrumentCount4);

		InstrumentRow->AddChildToHorizontalBox(Inst4);

	}



	UHorizontalBox* ApproachRow = AddLabelRow(MenuBox, ApproachValueText, TEXT("Note speed (s):"));

	if (UButton* ApproachDown = AddButton(TEXT("ApproachDown"), TEXT("-0.25")))

	{

		ApproachDown->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandleApproachDecrease);

		ApproachRow->AddChildToHorizontalBox(ApproachDown);

	}

	if (UButton* ApproachUp = AddButton(TEXT("ApproachUp"), TEXT("+0.25")))

	{

		ApproachUp->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandleApproachIncrease);

		ApproachRow->AddChildToHorizontalBox(ApproachUp);

	}



	UHorizontalBox* PreviewRow = AddLabelRow(MenuBox, PreviewValueText, TEXT("Preview (s):"));

	if (UButton* PreviewDown = AddButton(TEXT("PreviewDown"), TEXT("-0.25")))

	{

		PreviewDown->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandlePreviewDecrease);

		PreviewRow->AddChildToHorizontalBox(PreviewDown);

	}

	if (UButton* PreviewUp = AddButton(TEXT("PreviewUp"), TEXT("+0.25")))

	{

		PreviewUp->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandlePreviewIncrease);

		PreviewRow->AddChildToHorizontalBox(PreviewUp);

	}



	UHorizontalBox* LatencyRow = AddLabelRow(MenuBox, LatencyValueText, TEXT("Latency:"));

	if (UButton* LatencyDown = AddButton(TEXT("LatencyDown"), TEXT("-10ms")))

	{

		LatencyDown->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandleLatencyDecrease);

		LatencyRow->AddChildToHorizontalBox(LatencyDown);

	}

	if (UButton* LatencyUp = AddButton(TEXT("LatencyUp"), TEXT("+10ms")))

	{

		LatencyUp->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandleLatencyIncrease);

		LatencyRow->AddChildToHorizontalBox(LatencyUp);

	}



	if (UButton* PauseButton = AddButton(TEXT("PauseButton"), TEXT("Pause")))

	{

		PauseButton->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandlePauseClicked);

		MenuBox->AddChildToVerticalBox(PauseButton);

	}

	if (UButton* RestartButton = AddButton(TEXT("RestartButton"), TEXT("Restart")))

	{

		RestartButton->OnClicked.AddDynamic(this, &UAkbalRhythmHarnessWidget::HandleRestartClicked);

		MenuBox->AddChildToVerticalBox(RestartButton);

	}



	if (UCanvasPanelSlot* MenuSlot = RootCanvas->AddChildToCanvas(MenuPanel))
	{
		MenuSlot->SetAnchors(FAnchors(0.f, 0.f, 0.f, 0.f));
		MenuSlot->SetAlignment(FVector2D(0.f, 0.f));
		MenuSlot->SetPosition(FVector2D::ZeroVector);
		MenuSlot->SetAutoSize(true);
		MenuSlot->SetZOrder(30);
	}



	bWidgetBuilt = true;

}



void UAkbalRhythmHarnessWidget::RefreshLabels() const

{

	if (!Session || !Conductor)

	{

		return;

	}



	const FAkbalRitualInputHudConfig& Config = Session->GetConfig();

	const FAkbalMusicalPosition Position = Conductor->GetMusicalPosition();



	if (StatusText)

	{

		StatusText->SetText(FText::FromString(FString::Printf(

			TEXT("Bar %d | Beat %d/%d | Playing %s"),

			Position.Bar,

			Position.Beat,

			Config.BeatsPerBar,

			*FAkbalRhythmInstrumentLibrary::GetDisplayName(Session->GetActiveInstrument()))));

	}



	if (BpmValueText)

	{

		BpmValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Config.BeatsPerMinute)));

	}

	if (InstrumentCountText)

	{

		InstrumentCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Config.AvailableInstrumentCount)));

	}

	if (ApproachValueText)

	{

		ApproachValueText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Config.NoteApproachSeconds)));

	}

	if (PreviewValueText)

	{

		PreviewValueText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Config.NotePreviewSeconds)));

	}

	if (LatencyValueText)

	{

		LatencyValueText->SetText(FText::FromString(FString::Printf(TEXT("%+.0f ms"), Conductor->GetInputLatencyOffsetMs())));

	}



	if (JudgmentText)

	{

		FAkbalRhythmChartNote NextNote;

		if (FAkbalRhythmChartEvaluator::FindNextRequiredNote(

			Session->GetChartStates(), Conductor->GetSecondsSinceTransportStart(), Conductor->TimingWindows, NextNote))

		{

			JudgmentText->SetText(FText::FromString(FString::Printf(

				TEXT("Next: %s + %s | Last: %s"),

				*FAkbalRhythmInstrumentLibrary::GetDisplayName(NextNote.Instrument),

				*FAkbalRhythmLaneInput::GetArrowKeyLabel(NextNote.Lane),

				*UEnum::GetValueAsString(Session->GetLastJudgment().Judgment))));

		}

	}

}



void UAkbalRhythmHarnessWidget::HandlePauseClicked()

{

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



void UAkbalRhythmHarnessWidget::HandleRestartClicked()

{

	if (Session)

	{

		Session->RestartChart();

	}

}



void UAkbalRhythmHarnessWidget::HandleLatencyDecrease()

{

	if (Conductor)

	{

		Conductor->SetInputLatencyOffsetMs(Conductor->GetInputLatencyOffsetMs() - 10.f);

	}

}



void UAkbalRhythmHarnessWidget::HandleLatencyIncrease()

{

	if (Conductor)

	{

		Conductor->SetInputLatencyOffsetMs(Conductor->GetInputLatencyOffsetMs() + 10.f);

	}

}



void UAkbalRhythmHarnessWidget::HandleBpmDecrease()

{

	if (!Session)

	{

		return;

	}

	FAkbalRitualInputHudConfig Config = Session->GetConfig();

	Config.BeatsPerMinute = FMath::Max(40.f, Config.BeatsPerMinute - 5.f);

	Session->ApplyConfig(Config);

}



void UAkbalRhythmHarnessWidget::HandleBpmIncrease()

{

	if (!Session)

	{

		return;

	}

	FAkbalRitualInputHudConfig Config = Session->GetConfig();

	Config.BeatsPerMinute = FMath::Min(200.f, Config.BeatsPerMinute + 5.f);

	Session->ApplyConfig(Config);

}



void UAkbalRhythmHarnessWidget::HandleApproachDecrease()

{

	if (!Session)

	{

		return;

	}

	FAkbalRitualInputHudConfig Config = Session->GetConfig();

	Config.NoteApproachSeconds = FMath::Max(0.75f, Config.NoteApproachSeconds - 0.25f);

	Session->ApplyConfig(Config, false);

}



void UAkbalRhythmHarnessWidget::HandleApproachIncrease()

{

	if (!Session)

	{

		return;

	}

	FAkbalRitualInputHudConfig Config = Session->GetConfig();

	Config.NoteApproachSeconds = FMath::Min(5.f, Config.NoteApproachSeconds + 0.25f);

	Session->ApplyConfig(Config, false);

}



void UAkbalRhythmHarnessWidget::HandlePreviewDecrease()

{

	if (!Session)

	{

		return;

	}

	FAkbalRitualInputHudConfig Config = Session->GetConfig();

	Config.NotePreviewSeconds = FMath::Max(0.25f, Config.NotePreviewSeconds - 0.25f);

	Session->ApplyConfig(Config, false);

}



void UAkbalRhythmHarnessWidget::HandlePreviewIncrease()

{

	if (!Session)

	{

		return;

	}

	FAkbalRitualInputHudConfig Config = Session->GetConfig();

	Config.NotePreviewSeconds = FMath::Min(4.f, Config.NotePreviewSeconds + 0.25f);

	Session->ApplyConfig(Config, false);

}



void UAkbalRhythmHarnessWidget::SetInstrumentCount1() { SetInstrumentCount(1); }

void UAkbalRhythmHarnessWidget::SetInstrumentCount2() { SetInstrumentCount(2); }

void UAkbalRhythmHarnessWidget::SetInstrumentCount3() { SetInstrumentCount(3); }

void UAkbalRhythmHarnessWidget::SetInstrumentCount4() { SetInstrumentCount(4); }



void UAkbalRhythmHarnessWidget::SetInstrumentCount(int32 Count)

{

	if (!Session)

	{

		return;

	}

	FAkbalRitualInputHudConfig Config = Session->GetConfig();

	Config.AvailableInstrumentCount = FMath::Clamp(Count, 1, FAkbalRhythmInstrumentLibrary::InstrumentCount);

	Session->ApplyConfig(Config);

}



void UAkbalRhythmHarnessWidget::HandleSessionChanged()

{

	RefreshLabels();

}


