// Copyright Epic Games, Inc. All Rights Reserved.

#include "Spike/AkbalRhythmSpikeGameMode.h"

#include "Spike/AkbalRhythmSpikePlayerController.h"
#include "Spike/AkbalRhythmSpikeCharacter.h"

AAkbalRhythmSpikeGameMode::AAkbalRhythmSpikeGameMode()
{
	DefaultPawnClass = AAkbalRhythmSpikeCharacter::StaticClass();
	PlayerControllerClass = AAkbalRhythmSpikePlayerController::StaticClass();
}

void AAkbalRhythmSpikeGameMode::StartPlay()
{
	Super::StartPlay();
}
