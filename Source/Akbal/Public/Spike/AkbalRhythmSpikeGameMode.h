// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AkbalRhythmSpikeGameMode.generated.h"

UCLASS()
class AKBAL_API AAkbalRhythmSpikeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAkbalRhythmSpikeGameMode();

	virtual void StartPlay() override;
};
