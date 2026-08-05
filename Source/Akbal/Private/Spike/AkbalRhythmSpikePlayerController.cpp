// Copyright Epic Games, Inc. All Rights Reserved.



#include "Spike/AkbalRhythmSpikePlayerController.h"



#include "Audio/AkbalMetronomeSoundFactory.h"

#include "Audio/AkbalMusicConductorSubsystem.h"

#include "Audio/AkbalMusicalPosition.h"

#include "Engine/Engine.h"

#include "Kismet/GameplayStatics.h"

#include "UI/Spike/AkbalRhythmSpikeWidget.h"



AAkbalRhythmSpikePlayerController::AAkbalRhythmSpikePlayerController()

{

	bShowMouseCursor = true;

	bEnableClickEvents = true;

	bEnableMouseOverEvents = true;

	PrimaryActorTick.bCanEverTick = true;

}



void AAkbalRhythmSpikePlayerController::BeginPlay()

{

	Super::BeginPlay();



	FInputModeGameOnly InputMode;

	InputMode.SetConsumeCaptureMouseDown(false);

	SetInputMode(InputMode);

	SetShowMouseCursor(true);



	InitializeSpike();

}



void AAkbalRhythmSpikePlayerController::SetupInputComponent()

{

	Super::SetupInputComponent();



	if (!InputComponent)

	{

		return;

	}



	InputComponent->BindKey(EKeys::Left, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnLeftLanePressed);

	InputComponent->BindKey(EKeys::Right, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnRightLanePressed);

	InputComponent->BindKey(EKeys::Up, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnUpLanePressed);

	InputComponent->BindKey(EKeys::Down, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnDownLanePressed);

	InputComponent->BindKey(EKeys::Q, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnInstrumentPreviousPressed);

	InputComponent->BindKey(EKeys::E, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnInstrumentNextPressed);

	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnRestartPressed);

	InputComponent->BindKey(EKeys::P, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnTogglePausePressed);

}



bool AAkbalRhythmSpikePlayerController::InputKey(const FInputKeyEventArgs& Params)

{

	if (TryHandleRhythmKey(Params.Key, Params.Event))

	{

		return true;

	}



	return Super::InputKey(Params);

}



bool AAkbalRhythmSpikePlayerController::TryHandleRhythmKey(const FKey& Key, EInputEvent Event)

{

	if (Event != IE_Pressed)

	{

		return false;

	}



	if (Key == EKeys::Left)

	{

		OnLeftLanePressed();

		return true;

	}

	if (Key == EKeys::Right)

	{

		OnRightLanePressed();

		return true;

	}

	if (Key == EKeys::Up)

	{

		OnUpLanePressed();

		return true;

	}

	if (Key == EKeys::Down)

	{

		OnDownLanePressed();

		return true;

	}

	if (Key == EKeys::Q)

	{

		OnInstrumentPreviousPressed();

		return true;

	}

	if (Key == EKeys::E)

	{

		OnInstrumentNextPressed();

		return true;

	}



	return false;

}



void AAkbalRhythmSpikePlayerController::Tick(float DeltaSeconds)

{

	Super::Tick(DeltaSeconds);



	if (!bShowDebugOverlay || !Conductor)

	{

		return;

	}



	if (bUseRhythmWidget && RhythmWidget)

	{

		return;

	}



	DrawDebugOverlay();

}



void AAkbalRhythmSpikePlayerController::InitializeSpike()

{

	if (bSpikeInitialized)

	{

		return;

	}



	Conductor = UAkbalMusicConductorSubsystem::Get(this);



	if (Conductor)

	{

		Conductor->OnBeat.AddDynamic(this, &AAkbalRhythmSpikePlayerController::HandleConductorBeat);



		if (bAutoStartEncounter)

		{

			Conductor->StartEncounter(SpikeBeatsPerMinute, SpikeBeatsPerBar);

		}

	}



	bool bWidgetReady = false;

	if (bUseRhythmWidget)

	{

		bWidgetReady = CreateRhythmWidget();

		if (!bWidgetReady)

		{

			UE_LOG(LogTemp, Warning, TEXT("Akbal rhythm spike widget failed to create; using debug overlay fallback."));

		}

	}



	bSpikeInitialized = true;



	if (GEngine)

	{

		const FString Status = bWidgetReady

			? TEXT("Akbal rhythm spike ready (arrow keys + Q/E instruments).")

			: TEXT("Akbal rhythm spike ready (debug overlay fallback).");

		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, Status);

	}

}



bool AAkbalRhythmSpikePlayerController::CreateRhythmWidget()

{

	if (RhythmWidget)

	{

		return true;

	}



	RhythmWidget = CreateWidget<UAkbalRhythmSpikeWidget>(this, UAkbalRhythmSpikeWidget::StaticClass());

	if (!RhythmWidget)

	{

		return false;

	}



	RhythmWidget->ConfigureSpike(SpikeBeatsPerMinute, SpikeBeatsPerBar);

	RhythmWidget->OnTapSubmitted.AddDynamic(this, &AAkbalRhythmSpikePlayerController::HandleWidgetTap);

	RhythmWidget->AddToViewport(100);

	RhythmWidget->SetAnchorsInViewport(FAnchors(0.f, 0.f, 1.f, 1.f));

	RhythmWidget->SetAlignmentInViewport(FVector2D::ZeroVector);

	RhythmWidget->SetVisibility(ESlateVisibility::Visible);

	RhythmWidget->SetIsFocusable(false);



	return RhythmWidget->GetRootWidget() != nullptr;

}



void AAkbalRhythmSpikePlayerController::DrawDebugOverlay() const

{

	if (!GEngine || !Conductor)

	{

		return;

	}



	const FAkbalConductorDebugSnapshot Snapshot = Conductor->GetDebugSnapshot();

	const FString StateText = Snapshot.bPaused ? TEXT("Paused") : (Snapshot.bClockRunning ? TEXT("Running") : TEXT("Stopped"));

	const FString Overlay = FString::Printf(

		TEXT("Akbal Rhythm Spike | %s | %.0f BPM | Bar %d Beat %d (%.2f) | %.2fs\n")

		TEXT("Last: %s | Delta %.1f ms | Arrows: lanes | Q/E: instrument | P: pause | R: restart"),

		*StateText,

		Snapshot.BeatsPerMinute,

		Snapshot.Position.Bar,

		Snapshot.Position.Beat,

		Snapshot.Position.BeatFraction,

		Snapshot.Position.SecondsSinceTransportStart,

		*UEnum::GetValueAsString(Snapshot.LastJudgment.Judgment),

		Snapshot.LastJudgment.DeltaMs);



	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Cyan, Overlay);

}



void AAkbalRhythmSpikePlayerController::OnLeftLanePressed()

{

	OnLanePressed(EAkbalRhythmLane::LeftToRight);

}



void AAkbalRhythmSpikePlayerController::OnRightLanePressed()

{

	OnLanePressed(EAkbalRhythmLane::RightToLeft);

}



void AAkbalRhythmSpikePlayerController::OnUpLanePressed()

{

	OnLanePressed(EAkbalRhythmLane::TopToBottom);

}



void AAkbalRhythmSpikePlayerController::OnDownLanePressed()

{

	OnLanePressed(EAkbalRhythmLane::BottomToTop);

}



void AAkbalRhythmSpikePlayerController::OnLanePressed(EAkbalRhythmLane Lane)

{

	if (RhythmWidget)

	{

		RhythmWidget->ProcessLaneInput(Lane);

	}

}



void AAkbalRhythmSpikePlayerController::CycleInstrument(int32 Delta)

{

	if (RhythmWidget)

	{

		RhythmWidget->CycleActiveInstrument(Delta);

	}

}



void AAkbalRhythmSpikePlayerController::OnInstrumentPreviousPressed()

{

	CycleInstrument(-1);

}



void AAkbalRhythmSpikePlayerController::OnInstrumentNextPressed()

{

	CycleInstrument(1);

}



void AAkbalRhythmSpikePlayerController::HandleWidgetTap(const FAkbalRhythmJudgmentResult& Result)

{

	UE_LOG(LogTemp, Log, TEXT("Akbal rhythm input: %s (delta %.1f ms)"),

		*UEnum::GetValueAsString(Result.Judgment), Result.DeltaMs);

}



void AAkbalRhythmSpikePlayerController::HandleConductorBeat(const FAkbalMusicalPosition& Position)

{

	if (!bMetronomeEnabled || !Conductor || Conductor->IsEncounterPaused())

	{

		return;

	}



	const bool bAccent = Position.Beat == 1;

	const float FrequencyHz = bAccent ? MetronomeAccentFrequencyHz : MetronomeTickFrequencyHz;

	const float DurationSeconds = bAccent ? 0.08f : 0.05f;

	FAkbalMetronomeSoundFactory::PlayClickTone(this, this, FrequencyHz, DurationSeconds, MetronomeVolume);

}



void AAkbalRhythmSpikePlayerController::OnRestartPressed()

{

	if (!Conductor)

	{

		return;

	}



	Conductor->StopEncounter(true);

	Conductor->StartEncounter(SpikeBeatsPerMinute, SpikeBeatsPerBar);

	if (RhythmWidget)

	{

		RhythmWidget->RestartChart();

	}

}



void AAkbalRhythmSpikePlayerController::OnTogglePausePressed()

{

	if (!Conductor)

	{

		return;

	}



	const bool bShouldPause = !IsPaused();

	SetPause(bShouldPause);



	if (bShouldPause)

	{

		Conductor->PauseEncounter();

	}

	else

	{

		Conductor->ResumeEncounter();

	}

}



