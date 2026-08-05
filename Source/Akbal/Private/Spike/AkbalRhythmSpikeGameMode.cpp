// Copyright Epic Games, Inc. All Rights Reserved.

#include "Spike/AkbalRhythmSpikeGameMode.h"

#include "Spike/AkbalRhythmSpikeActor.h"
#include "Spike/AkbalRhythmSpikePlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "TimerManager.h"

AAkbalRhythmSpikeGameMode::AAkbalRhythmSpikeGameMode()
{
	DefaultPawnClass = ASpectatorPawn::StaticClass();
	PlayerControllerClass = AAkbalRhythmSpikePlayerController::StaticClass();
}

void AAkbalRhythmSpikeGameMode::StartPlay()
{
	Super::StartPlay();
}
