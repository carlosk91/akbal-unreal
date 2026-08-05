// Copyright Epic Games, Inc. All Rights Reserved.

#include "Spike/AkbalRhythmSpikeActor.h"

#include "Audio/AkbalMusicConductorSubsystem.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MetasoundSource.h"

AAkbalRhythmSpikeActor::AAkbalRhythmSpikeActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAkbalRhythmSpikeActor::BeginPlay()
{
	Super::BeginPlay();

	Conductor = UAkbalMusicConductorSubsystem::Get(this);
	if (Conductor && bAutoStartEncounter)
	{
		Conductor->StartEncounter(SpikeBeatsPerMinute, SpikeBeatsPerBar);
	}

	if (SpikeMetaSound)
	{
		MetaSoundComponent = UGameplayStatics::SpawnSound2D(this, SpikeMetaSound);
	}

	BindInput();
}

void AAkbalRhythmSpikeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bShowDebugOverlay || !Conductor)
	{
		return;
	}

	DrawDebugOverlay(Conductor->GetDebugSnapshot());
}

void AAkbalRhythmSpikeActor::BindInput()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		return;
	}

	EnableInput(PlayerController);
	if (!InputComponent)
	{
		return;
	}

	InputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnTapPressed);
	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnRestartPressed);
	InputComponent->BindKey(EKeys::P, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnTogglePausePressed);
}

void AAkbalRhythmSpikeActor::OnTapPressed()
{
	if (!Conductor)
	{
		return;
	}

	const float InputSeconds = Conductor->GetSecondsSinceTransportStart();
	const FAkbalRhythmJudgmentResult Result = Conductor->JudgeInputAtSeconds(InputSeconds);
	UE_LOG(LogTemp, Log, TEXT("Akbal rhythm tap: %s (delta %.1f ms)"),
		*UEnum::GetValueAsString(Result.Judgment), Result.DeltaMs);

	TryPlayMetaSoundClick();
}

void AAkbalRhythmSpikeActor::OnRestartPressed()
{
	if (!Conductor)
	{
		return;
	}

	Conductor->StopEncounter(true);
	Conductor->StartEncounter(SpikeBeatsPerMinute, SpikeBeatsPerBar);
}

void AAkbalRhythmSpikeActor::OnTogglePausePressed()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		return;
	}

	const bool bShouldPause = !PlayerController->IsPaused();
	PlayerController->SetPause(bShouldPause);

	if (!Conductor)
	{
		return;
	}

	if (bShouldPause)
	{
		Conductor->PauseEncounter();
	}
	else
	{
		Conductor->ResumeEncounter();
	}
}

void AAkbalRhythmSpikeActor::DrawDebugOverlay(const FAkbalConductorDebugSnapshot& Snapshot) const
{
	if (!GEngine)
	{
		return;
	}

	const FString StateText = Snapshot.bPaused ? TEXT("Paused") : (Snapshot.bClockRunning ? TEXT("Running") : TEXT("Stopped"));
	const FString Overlay = FString::Printf(
		TEXT("Akbal Rhythm Spike | %s | %.0f BPM | Bar %d Beat %d (%.2f) | %.2fs | Beats: %d | Latency RT %.1fms G->A %.1fms\n")
		TEXT("Last: %s | Delta %.1f ms | [Space] Tap  [P] Pause  [R] Restart"),
		*StateText,
		Snapshot.BeatsPerMinute,
		Snapshot.Position.Bar,
		Snapshot.Position.Beat,
		Snapshot.Position.BeatFraction,
		Snapshot.Position.SecondsSinceTransportStart,
		Snapshot.BeatCallbackCount,
		Snapshot.RoundTripLatencyMs,
		Snapshot.GameToAudioLatencyMs,
		*UEnum::GetValueAsString(Snapshot.LastJudgment.Judgment),
		Snapshot.LastJudgment.DeltaMs);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Cyan, Overlay);
}

void AAkbalRhythmSpikeActor::TryPlayMetaSoundClick()
{
	if (MetaSoundComponent && SpikeMetaSound)
	{
		MetaSoundComponent->Play();
	}
}
