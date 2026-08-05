// Copyright Epic Games, Inc. All Rights Reserved.

#include "Spike/AkbalRhythmSpikePlayerController.h"

#include "Audio/AkbalMusicConductorSubsystem.h"
#include "Audio/AkbalMusicalPosition.h"
#include "Engine/Engine.h"
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

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	InitializeSpike();
}

void AAkbalRhythmSpikePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
	{
		return;
	}

	InputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnTapPressed);
	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnRestartPressed);
	InputComponent->BindKey(EKeys::P, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnTogglePausePressed);
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
	if (Conductor && bAutoStartEncounter)
	{
		Conductor->StartEncounter(SpikeBeatsPerMinute, SpikeBeatsPerBar);
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
			? TEXT("Akbal rhythm spike ready (widget).")
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
		TEXT("Last: %s | Delta %.1f ms | [Space] Tap  [P] Pause  [R] Restart"),
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

void AAkbalRhythmSpikePlayerController::OnTapPressed()
{
	if (!Conductor || (bUseRhythmWidget && RhythmWidget))
	{
		return;
	}

	const FAkbalRhythmJudgmentResult Result = Conductor->JudgeInputAtSeconds(Conductor->GetSecondsSinceTransportStart());
	HandleWidgetTap(Result);
}

void AAkbalRhythmSpikePlayerController::HandleWidgetTap(const FAkbalRhythmJudgmentResult& Result)
{
	UE_LOG(LogTemp, Log, TEXT("Akbal rhythm tap: %s (delta %.1f ms)"),
		*UEnum::GetValueAsString(Result.Judgment), Result.DeltaMs);
}

void AAkbalRhythmSpikePlayerController::OnRestartPressed()
{
	if (!Conductor)
	{
		return;
	}

	Conductor->StopEncounter(true);
	Conductor->StartEncounter(SpikeBeatsPerMinute, SpikeBeatsPerBar);
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
