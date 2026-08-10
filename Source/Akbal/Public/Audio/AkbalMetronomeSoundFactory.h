// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"



class USoundWaveProcedural;



class AKBAL_API FAkbalMetronomeSoundFactory

{

public:

	static USoundWaveProcedural* CreateClickTone(

		UObject* Outer,

		float FrequencyHz,

		float DurationSeconds,

		float Volume = 0.8f);



	static void PlayClickTone(

		UObject* WorldContextObject,

		UObject* Outer,

		float FrequencyHz,

		float DurationSeconds,

		float Volume = 0.8f);

};

