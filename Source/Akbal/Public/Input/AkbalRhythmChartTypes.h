// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"

#include "AkbalRhythmChartTypes.generated.h"



UENUM(BlueprintType)

enum class EAkbalRhythmLane : uint8

{

	LeftToRight UMETA(DisplayName = "Left To Right"),

	RightToLeft UMETA(DisplayName = "Right To Left"),

	TopToBottom UMETA(DisplayName = "Top To Bottom"),

	BottomToTop UMETA(DisplayName = "Bottom To Top")

};



UENUM(BlueprintType)

enum class EAkbalRhythmInstrument : uint8

{

	Drum UMETA(DisplayName = "Drum"),

	Chime UMETA(DisplayName = "Chime"),

	String UMETA(DisplayName = "String"),

	Wind UMETA(DisplayName = "Wind")

};



USTRUCT(BlueprintType)

struct FAkbalRhythmChartNote

{

	GENERATED_BODY()



	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")

	int32 NoteId = INDEX_NONE;



	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")

	EAkbalRhythmLane Lane = EAkbalRhythmLane::LeftToRight;



	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")

	EAkbalRhythmInstrument Instrument = EAkbalRhythmInstrument::Drum;



	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")

	float TargetSeconds = 0.f;



	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")

	bool bRequired = true;

};



USTRUCT(BlueprintType)

struct FAkbalRhythmChartNoteState

{

	GENERATED_BODY()



	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")

	FAkbalRhythmChartNote Note;



	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")

	bool bConsumed = false;



	UPROPERTY(BlueprintReadOnly, Category = "Rhythm")

	bool bMissed = false;

};



class AKBAL_API FAkbalRhythmLaneInput

{

public:

	static EAkbalRhythmLane LaneForArrowKey(const FKey& Key);

	static FString GetArrowKeyLabel(EAkbalRhythmLane Lane);

};



class AKBAL_API FAkbalRhythmInstrumentLibrary

{

public:

	static constexpr int32 InstrumentCount = 4;



	static EAkbalRhythmInstrument InstrumentFromIndex(int32 Index);

	static int32 IndexFromInstrument(EAkbalRhythmInstrument Instrument);

	static FString GetDisplayName(EAkbalRhythmInstrument Instrument);

	static FLinearColor GetPrimaryColor(EAkbalRhythmInstrument Instrument);

};



class AKBAL_API FAkbalRhythmChartBuilder

{

public:

	/** Mixed spike chart: varied lanes and instruments for instrument-switch testing. */

	static TArray<FAkbalRhythmChartNote> BuildSpikeTestChart(float BeatsPerMinute);



	static EAkbalRhythmLane LaneFromIndex(int32 LaneIndex);

	static EAkbalRhythmInstrument InstrumentFromIndex(int32 InstrumentIndex);

};



