// Copyright Epic Games, Inc. All Rights Reserved.

#include "Spike/AkbalRhythmSpikeActor.h"

#include "Audio/AkbalMusicConductorSubsystem.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Input/AkbalRhythmChartTypes.h"
#include "Kismet/GameplayStatics.h"
#include "MetasoundSource.h"
#include "Spike/AkbalRhythmSpikePlayerController.h"
#include "TimerManager.h"
#include "UI/Ritual/AkbalRitualInputSession.h"

AAkbalRhythmSpikeActor::AAkbalRhythmSpikeActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAkbalRhythmSpikeActor::BeginPlay()
{
	Super::BeginPlay();

	Conductor = UAkbalMusicConductorSubsystem::Get(this);
	if (SpikeMetaSound)
	{
		MetaSoundComponent = UGameplayStatics::SpawnSound2D(this, SpikeMetaSound);
	}

	TryInitializeSpikeUI();
}

void AAkbalRhythmSpikeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Conductor || !bShowDebugOverlay)
	{
		return;
	}

	DrawDebugOverlay(Conductor->GetDebugSnapshot());
}

void AAkbalRhythmSpikeActor::TryInitializeSpikeUI()
{
	if (bSpikeUIInitialized)
	{
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				SpikeInitRetryHandle,
				this,
				&AAkbalRhythmSpikeActor::TryInitializeSpikeUI,
				0.1f,
				false);
		}
		return;
	}

	bSpikeUIInitialized = true;
	BindInput();
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

	InputComponent->BindKey(EKeys::Left, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnLeftLanePressed);
	InputComponent->BindKey(EKeys::Right, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnRightLanePressed);
	InputComponent->BindKey(EKeys::Up, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnUpLanePressed);
	InputComponent->BindKey(EKeys::Down, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnDownLanePressed);
	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnRestartPressed);
	InputComponent->BindKey(EKeys::P, IE_Pressed, this, &AAkbalRhythmSpikeActor::OnTogglePausePressed);
}

void AAkbalRhythmSpikeActor::OnLeftLanePressed()
{
	OnLanePressed(EAkbalRhythmLane::LeftToRight);
}

void AAkbalRhythmSpikeActor::OnRightLanePressed()
{
	OnLanePressed(EAkbalRhythmLane::RightToLeft);
}

void AAkbalRhythmSpikeActor::OnUpLanePressed()
{
	OnLanePressed(EAkbalRhythmLane::TopToBottom);
}

void AAkbalRhythmSpikeActor::OnDownLanePressed()
{
	OnLanePressed(EAkbalRhythmLane::BottomToTop);
}

void AAkbalRhythmSpikeActor::OnLanePressed(EAkbalRhythmLane Lane)
{
	if (AAkbalRhythmSpikePlayerController* SpikePC = Cast<AAkbalRhythmSpikePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (UAkbalRitualInputSession* Session = SpikePC->GetRitualInputSession())
		{
			Session->ProcessLaneInput(Lane);
			return;
		}
	}

	if (!Conductor)
	{
		return;
	}

	const FAkbalRhythmJudgmentResult Result = Conductor->JudgeInputAtSeconds(Conductor->GetSecondsSinceTransportStart());
	HandleWidgetTap(Result);
}

void AAkbalRhythmSpikeActor::HandleWidgetTap(const FAkbalRhythmJudgmentResult& Result)
{
	UE_LOG(LogTemp, Log, TEXT("Akbal rhythm tap: %s (delta %.1f ms)"),
		*UEnum::GetValueAsString(Result.Judgment), Result.DeltaMs);

	TryPlayMetaSoundClick();
}

void AAkbalRhythmSpikeActor::OnRestartPressed()
{
	if (AAkbalRhythmSpikePlayerController* SpikePC = Cast<AAkbalRhythmSpikePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (UAkbalRitualInputSession* Session = SpikePC->GetRitualInputSession())
		{
			Session->RestartChart();
		}
	}
}

void AAkbalRhythmSpikeActor::OnTogglePausePressed()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		const bool bShouldPause = !PlayerController->IsPaused();
		PlayerController->SetPause(bShouldPause);
		if (Conductor)
		{
			if (bShouldPause)
			{
				Conductor->PauseEncounter();
			}
			else
			{
				Conductor->ResumeEncounter();
			}
		}
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
		TEXT("Akbal Rhythm Spike Actor | %s | %.0f BPM | Bar %d Beat %d"),
		*StateText,
		Snapshot.BeatsPerMinute,
		Snapshot.Position.Bar,
		Snapshot.Position.Beat);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Cyan, Overlay);
}

void AAkbalRhythmSpikeActor::TryPlayMetaSoundClick()
{
	if (MetaSoundComponent && SpikeMetaSound)
	{
		MetaSoundComponent->SetSound(SpikeMetaSound);
		MetaSoundComponent->Play();
	}
}
