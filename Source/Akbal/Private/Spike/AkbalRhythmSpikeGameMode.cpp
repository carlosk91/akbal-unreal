// Copyright Epic Games, Inc. All Rights Reserved.

#include "Spike/AkbalRhythmSpikeGameMode.h"

#include "Spike/AkbalRhythmSpikeActor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "TimerManager.h"

AAkbalRhythmSpikeGameMode::AAkbalRhythmSpikeGameMode()
{
	DefaultPawnClass = ASpectatorPawn::StaticClass();
}

void AAkbalRhythmSpikeGameMode::StartPlay()
{
	Super::StartPlay();

	if (!GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &AAkbalRhythmSpikeGameMode::SpawnSpikeActor));
}

void AAkbalRhythmSpikeGameMode::SpawnSpikeActor()
{
	if (!GetWorld())
	{
		return;
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(InputMode);
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AAkbalRhythmSpikeActor>(
		AAkbalRhythmSpikeActor::StaticClass(),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams);
}
