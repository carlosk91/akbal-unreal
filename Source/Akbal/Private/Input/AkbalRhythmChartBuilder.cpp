// Copyright Epic Games, Inc. All Rights Reserved.



#include "Input/AkbalRhythmChartTypes.h"



#include "Input/AkbalRhythmJudgmentEvaluator.h"

#include "InputCoreTypes.h"



EAkbalRhythmLane FAkbalRhythmLaneInput::LaneForArrowKey(const FKey& Key)

{

	if (Key == EKeys::Left)

	{

		return EAkbalRhythmLane::LeftToRight;

	}

	if (Key == EKeys::Right)

	{

		return EAkbalRhythmLane::RightToLeft;

	}

	if (Key == EKeys::Up)

	{

		return EAkbalRhythmLane::TopToBottom;

	}

	if (Key == EKeys::Down)

	{

		return EAkbalRhythmLane::BottomToTop;

	}



	return EAkbalRhythmLane::LeftToRight;

}



FString FAkbalRhythmLaneInput::GetArrowKeyLabel(EAkbalRhythmLane Lane)

{

	switch (Lane)

	{

	case EAkbalRhythmLane::LeftToRight: return TEXT("Left");

	case EAkbalRhythmLane::RightToLeft: return TEXT("Right");

	case EAkbalRhythmLane::TopToBottom: return TEXT("Up");

	case EAkbalRhythmLane::BottomToTop: return TEXT("Down");

	default: return TEXT("?");

	}

}



EAkbalRhythmInstrument FAkbalRhythmInstrumentLibrary::InstrumentFromIndex(int32 Index)

{

	switch (Index % InstrumentCount)

	{

	case 0: return EAkbalRhythmInstrument::Drum;

	case 1: return EAkbalRhythmInstrument::Chime;

	case 2: return EAkbalRhythmInstrument::String;

	default: return EAkbalRhythmInstrument::Wind;

	}

}



int32 FAkbalRhythmInstrumentLibrary::IndexFromInstrument(EAkbalRhythmInstrument Instrument)

{

	switch (Instrument)

	{

	case EAkbalRhythmInstrument::Drum: return 0;

	case EAkbalRhythmInstrument::Chime: return 1;

	case EAkbalRhythmInstrument::String: return 2;

	default: return 3;

	}

}



FString FAkbalRhythmInstrumentLibrary::GetDisplayName(EAkbalRhythmInstrument Instrument)

{

	switch (Instrument)

	{

	case EAkbalRhythmInstrument::Drum: return TEXT("Drum");

	case EAkbalRhythmInstrument::Chime: return TEXT("Chime");

	case EAkbalRhythmInstrument::String: return TEXT("String");

	default: return TEXT("Wind");

	}

}



FLinearColor FAkbalRhythmInstrumentLibrary::GetPrimaryColor(EAkbalRhythmInstrument Instrument)

{

	switch (Instrument)

	{

	case EAkbalRhythmInstrument::Drum: return FLinearColor(0.95f, 0.35f, 0.25f, 1.f);

	case EAkbalRhythmInstrument::Chime: return FLinearColor(0.25f, 0.85f, 0.95f, 1.f);

	case EAkbalRhythmInstrument::String: return FLinearColor(0.95f, 0.8f, 0.25f, 1.f);

	default: return FLinearColor(0.35f, 0.9f, 0.45f, 1.f);

	}

}



EAkbalRhythmLane FAkbalRhythmChartBuilder::LaneFromIndex(int32 LaneIndex)

{

	switch (LaneIndex % 4)

	{

	case 0: return EAkbalRhythmLane::LeftToRight;

	case 1: return EAkbalRhythmLane::RightToLeft;

	case 2: return EAkbalRhythmLane::TopToBottom;

	default: return EAkbalRhythmLane::BottomToTop;

	}

}



EAkbalRhythmInstrument FAkbalRhythmChartBuilder::InstrumentFromIndex(int32 InstrumentIndex)

{

	return FAkbalRhythmInstrumentLibrary::InstrumentFromIndex(InstrumentIndex);

}



TArray<FAkbalRhythmChartNote> FAkbalRhythmChartBuilder::BuildSpikeTestChart(float BeatsPerMinute)

{

	struct FNoteTemplate

	{

		EAkbalRhythmLane Lane;

		EAkbalRhythmInstrument Instrument;

		int32 BeatNumber;

		bool bRequired;

	};



	static const FNoteTemplate Templates[] = {
		{EAkbalRhythmLane::LeftToRight, EAkbalRhythmInstrument::Drum, 1, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::Chime, 2, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::String, 3, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::Wind, 4, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::Chime, 5, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::Drum, 6, true},
		{EAkbalRhythmLane::LeftToRight, EAkbalRhythmInstrument::Wind, 7, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::String, 8, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::Drum, 9, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::Chime, 10, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::String, 11, true},
		{EAkbalRhythmLane::LeftToRight, EAkbalRhythmInstrument::Wind, 12, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::Chime, 13, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::Drum, 14, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::String, 15, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::Wind, 16, true},
		{EAkbalRhythmLane::LeftToRight, EAkbalRhythmInstrument::Chime, 17, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::Drum, 18, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::String, 19, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::Wind, 20, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::Drum, 21, true},
		{EAkbalRhythmLane::LeftToRight, EAkbalRhythmInstrument::Chime, 22, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::String, 23, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::Wind, 24, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::Chime, 25, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::Drum, 26, true},
		{EAkbalRhythmLane::LeftToRight, EAkbalRhythmInstrument::String, 27, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::Wind, 28, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::Drum, 29, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::Chime, 30, true},
		{EAkbalRhythmLane::LeftToRight, EAkbalRhythmInstrument::Wind, 31, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::String, 32, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::Chime, 33, true},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::Drum, 34, true},
		{EAkbalRhythmLane::LeftToRight, EAkbalRhythmInstrument::String, 35, true},
		{EAkbalRhythmLane::TopToBottom, EAkbalRhythmInstrument::Wind, 36, true},
		{EAkbalRhythmLane::RightToLeft, EAkbalRhythmInstrument::Drum, 38, false},
		{EAkbalRhythmLane::BottomToTop, EAkbalRhythmInstrument::Chime, 40, true},
	};



	const float SecondsPerBeat = FAkbalRhythmJudgmentEvaluator::GetSecondsPerBeat(BeatsPerMinute);

	TArray<FAkbalRhythmChartNote> Notes;

	Notes.Reserve(UE_ARRAY_COUNT(Templates));



	for (int32 Index = 0; Index < UE_ARRAY_COUNT(Templates); ++Index)

	{

		const FNoteTemplate& Template = Templates[Index];

		FAkbalRhythmChartNote Note;

		Note.NoteId = Index;

		Note.Lane = Template.Lane;

		Note.Instrument = Template.Instrument;

		Note.TargetSeconds = SecondsPerBeat * static_cast<float>(Template.BeatNumber);

		Note.bRequired = Template.bRequired;

		Notes.Add(Note);

	}



	return Notes;

}



