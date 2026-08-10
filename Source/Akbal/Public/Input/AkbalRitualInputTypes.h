// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"

#include "AkbalRitualInputTypes.generated.h"



USTRUCT(BlueprintType)

struct FAkbalRitualInputHudConfig

{

	GENERATED_BODY()



	/** Encounter tempo used by the conductor and test chart. */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual")

	float BeatsPerMinute = 80.f;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual")

	int32 BeatsPerBar = 4;



	/** How many instrument channels are active (1-4). */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual", meta = (ClampMin = "1", ClampMax = "4"))

	int32 AvailableInstrumentCount = 4;



	/** Seconds for a note to travel from the lane edge to the center. */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual", meta = (ClampMin = "0.5"))

	float NoteApproachSeconds = 1.f;



	/** Only show notes that will hit within this many seconds. */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual", meta = (ClampMin = "0.1"))

	float NotePreviewSeconds = 1.f;



	/** Square HUD uses at most this fraction of the smaller viewport dimension. */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual", meta = (ClampMin = "0.05", ClampMax = "0.5"))

	float MaxViewportSizeFraction = 0.28f;

};



