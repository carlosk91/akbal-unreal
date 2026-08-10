// Copyright Epic Games, Inc. All Rights Reserved.



#include "Audio/AkbalMetronomeSoundFactory.h"



#include "Kismet/GameplayStatics.h"

#include "Sound/SoundWaveProcedural.h"



USoundWaveProcedural* FAkbalMetronomeSoundFactory::CreateClickTone(

	UObject* Outer,

	float FrequencyHz,

	float DurationSeconds,

	float Volume)

{

	if (!Outer || DurationSeconds <= KINDA_SMALL_NUMBER || FrequencyHz <= KINDA_SMALL_NUMBER)

	{

		return nullptr;

	}



	const int32 SampleRate = 44100;

	const int32 NumSamples = FMath::Max(1, FMath::RoundToInt(SampleRate * DurationSeconds));

	TArray<int16> Samples;

	Samples.SetNum(NumSamples);



	for (int32 SampleIndex = 0; SampleIndex < NumSamples; ++SampleIndex)

	{

		const float Time = static_cast<float>(SampleIndex) / SampleRate;

		const float Envelope = FMath::Exp(-Time * 40.f);

		const float Sine = FMath::Sin(2.f * PI * FrequencyHz * Time);

		Samples[SampleIndex] = static_cast<int16>(FMath::Clamp(Sine * Envelope * Volume, -1.f, 1.f) * 28000.f);

	}



	const int32 DataSize = NumSamples * sizeof(int16);

	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>(Outer);

	SoundWave->SetSampleRate(SampleRate);

	SoundWave->Duration = DurationSeconds;

	SoundWave->NumChannels = 1;

	SoundWave->TotalSamples = NumSamples;

	SoundWave->bLooping = false;

	SoundWave->QueueAudio(reinterpret_cast<const uint8*>(Samples.GetData()), DataSize);



	return SoundWave;

}



void FAkbalMetronomeSoundFactory::PlayClickTone(

	UObject* WorldContextObject,

	UObject* Outer,

	float FrequencyHz,

	float DurationSeconds,

	float Volume)

{

	if (!WorldContextObject || !Outer)

	{

		return;

	}



	if (USoundWaveProcedural* ClickTone = CreateClickTone(Outer, FrequencyHz, DurationSeconds, Volume))

	{

		UGameplayStatics::PlaySound2D(WorldContextObject, ClickTone, Volume);

	}

}

