// Copyright Epic Games, Inc. All Rights Reserved.

#include "Spike/AkbalRhythmSpikeGameMode.h"

#include "Spike/AkbalRhythmSpikeActor.h"
#include "GameFramework/SpectatorPawn.h"

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

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = TEXT("AkbalRhythmSpikeActor");
	GetWorld()->SpawnActor<AAkbalRhythmSpikeActor>(AAkbalRhythmSpikeActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}
