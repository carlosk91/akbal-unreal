// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/AkbalRhythmChartEvaluator.h"
#include "Input/AkbalRhythmChartTypes.h"
#include "Input/AkbalRhythmJudgmentEvaluator.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "InputCoreTypes.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace AkbalRhythmChartTest
{
	static const FAkbalRhythmTimingWindows DefaultWindows;
	static constexpr float TestBpm = 120.f;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmLaneArrowKeyMappingTest,
	"Akbal.Rhythm.LaneArrowKeyMapping",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmLaneArrowKeyMappingTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Left arrow -> left lane"), EAkbalRhythmLane::LeftToRight, FAkbalRhythmLaneInput::LaneForArrowKey(EKeys::Left));
	TestEqual(TEXT("Right arrow -> right lane"), EAkbalRhythmLane::RightToLeft, FAkbalRhythmLaneInput::LaneForArrowKey(EKeys::Right));
	TestEqual(TEXT("Up arrow -> top lane"), EAkbalRhythmLane::TopToBottom, FAkbalRhythmLaneInput::LaneForArrowKey(EKeys::Up));
	TestEqual(TEXT("Down arrow -> bottom lane"), EAkbalRhythmLane::BottomToTop, FAkbalRhythmLaneInput::LaneForArrowKey(EKeys::Down));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmChartUsesAllInstrumentsTest,
	"Akbal.Rhythm.ChartUsesAllInstruments",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmChartUsesAllInstrumentsTest::RunTest(const FString& Parameters)
{
	const TArray<FAkbalRhythmChartNote> Notes = FAkbalRhythmChartBuilder::BuildSpikeTestChart(AkbalRhythmChartTest::TestBpm);

	bool bHasDrum = false;
	bool bHasChime = false;
	bool bHasString = false;
	bool bHasWind = false;

	for (const FAkbalRhythmChartNote& Note : Notes)
	{
		if (!Note.bRequired)
		{
			continue;
		}

		switch (Note.Instrument)
		{
		case EAkbalRhythmInstrument::Drum: bHasDrum = true; break;
		case EAkbalRhythmInstrument::Chime: bHasChime = true; break;
		case EAkbalRhythmInstrument::String: bHasString = true; break;
		case EAkbalRhythmInstrument::Wind: bHasWind = true; break;
		default: break;
		}
	}

	TestTrue(TEXT("Spike chart includes Drum"), bHasDrum);
	TestTrue(TEXT("Spike chart includes Chime"), bHasChime);
	TestTrue(TEXT("Spike chart includes String"), bHasString);
	TestTrue(TEXT("Spike chart includes Wind"), bHasWind);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmChartLaneInstrumentJudgmentTest,
	"Akbal.Rhythm.ChartLaneInstrumentJudgment",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmChartLaneInstrumentJudgmentTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRhythmChartTest;

	const float TargetSeconds = FAkbalRhythmJudgmentEvaluator::GetSecondsPerBeat(TestBpm);

	TArray<FAkbalRhythmChartNoteState> ChartStates;
	FAkbalRhythmChartNoteState State;
	State.Note.NoteId = 0;
	State.Note.Lane = EAkbalRhythmLane::LeftToRight;
	State.Note.Instrument = EAkbalRhythmInstrument::Chime;
	State.Note.TargetSeconds = TargetSeconds;
	State.Note.bRequired = true;
	ChartStates.Add(State);

	int32 MatchedIndex = INDEX_NONE;

	FAkbalRhythmJudgmentResult WrongLane = FAkbalRhythmChartEvaluator::JudgeInputAgainstChart(
		TargetSeconds,
		EAkbalRhythmLane::RightToLeft,
		EAkbalRhythmInstrument::Chime,
		ChartStates,
		DefaultWindows,
		MatchedIndex);
	TestEqual(TEXT("Wrong lane misses"), EAkbalRhythmJudgment::Miss, WrongLane.Judgment);
	TestEqual(TEXT("Wrong lane index"), INDEX_NONE, MatchedIndex);

	FAkbalRhythmJudgmentResult WrongInstrument = FAkbalRhythmChartEvaluator::JudgeInputAgainstChart(
		TargetSeconds,
		EAkbalRhythmLane::LeftToRight,
		EAkbalRhythmInstrument::Drum,
		ChartStates,
		DefaultWindows,
		MatchedIndex);
	TestEqual(TEXT("Wrong instrument misses"), EAkbalRhythmJudgment::Miss, WrongInstrument.Judgment);
	TestEqual(TEXT("Wrong instrument index"), INDEX_NONE, MatchedIndex);

	FAkbalRhythmJudgmentResult PerfectHit = FAkbalRhythmChartEvaluator::JudgeInputAgainstChart(
		TargetSeconds,
		EAkbalRhythmLane::LeftToRight,
		EAkbalRhythmInstrument::Chime,
		ChartStates,
		DefaultWindows,
		MatchedIndex);
	TestEqual(TEXT("Matching lane and instrument hits"), EAkbalRhythmJudgment::Perfect, PerfectHit.Judgment);
	TestEqual(TEXT("Matched note index"), 0, MatchedIndex);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmInstrumentLibraryTest,
	"Akbal.Rhythm.InstrumentLibrary",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmInstrumentLibraryTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Instrument count"), 4, FAkbalRhythmInstrumentLibrary::InstrumentCount);

	for (int32 Index = 0; Index < FAkbalRhythmInstrumentLibrary::InstrumentCount; ++Index)
	{
		const EAkbalRhythmInstrument Instrument = FAkbalRhythmInstrumentLibrary::InstrumentFromIndex(Index);
		TestEqual(
			*FString::Printf(TEXT("Instrument index round-trip %d"), Index),
			Index,
			FAkbalRhythmInstrumentLibrary::IndexFromInstrument(Instrument));
		TestFalse(
			*FString::Printf(TEXT("Instrument %d has display name"), Index),
			FAkbalRhythmInstrumentLibrary::GetDisplayName(Instrument).IsEmpty());
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmChartNoteCountTest,
	"Akbal.Rhythm.ChartNoteCount",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmChartNoteCountTest::RunTest(const FString& Parameters)
{
	const TArray<FAkbalRhythmChartNote> Notes = FAkbalRhythmChartBuilder::BuildSpikeTestChart(AkbalRhythmChartTest::TestBpm);

	int32 RequiredCount = 0;
	for (const FAkbalRhythmChartNote& Note : Notes)
	{
		if (Note.bRequired)
		{
			++RequiredCount;
		}
	}

	TestTrue(TEXT("Spike chart has expanded required note count"), RequiredCount >= 36);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmChartForcesInstrumentChangesTest,
	"Akbal.Rhythm.ChartForcesInstrumentChanges",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmChartForcesInstrumentChangesTest::RunTest(const FString& Parameters)
{
	const TArray<FAkbalRhythmChartNote> Notes = FAkbalRhythmChartBuilder::BuildSpikeTestChart(AkbalRhythmChartTest::TestBpm);

	int32 InstrumentChanges = 0;
	int32 LaneChanges = 0;
	EAkbalRhythmInstrument PreviousInstrument = EAkbalRhythmInstrument::Drum;
	EAkbalRhythmLane PreviousLane = EAkbalRhythmLane::LeftToRight;
	bool bHasPrevious = false;

	for (const FAkbalRhythmChartNote& Note : Notes)
	{
		if (!Note.bRequired)
		{
			continue;
		}

		if (bHasPrevious)
		{
			if (Note.Instrument != PreviousInstrument)
			{
				++InstrumentChanges;
			}
			if (Note.Lane != PreviousLane)
			{
				++LaneChanges;
			}
		}

		PreviousInstrument = Note.Instrument;
		PreviousLane = Note.Lane;
		bHasPrevious = true;
	}

	TestTrue(TEXT("Chart requires frequent instrument changes"), InstrumentChanges >= 20);
	TestTrue(TEXT("Chart requires frequent lane changes"), LaneChanges >= 20);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmInstrumentSwitchPreservesChartTest,
	"Akbal.Rhythm.InstrumentSwitchPreservesChart",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmInstrumentSwitchPreservesChartTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRhythmChartTest;

	TArray<FAkbalRhythmChartNoteState> ChartStates;
	for (int32 Index = 0; Index < 3; ++Index)
	{
		FAkbalRhythmChartNoteState State;
		State.Note.NoteId = Index;
		State.Note.Instrument = FAkbalRhythmInstrumentLibrary::InstrumentFromIndex(Index);
		State.Note.Lane = FAkbalRhythmChartBuilder::LaneFromIndex(Index);
		State.Note.TargetSeconds = FAkbalRhythmJudgmentEvaluator::GetSecondsPerBeat(TestBpm) * static_cast<float>(Index + 1);
		State.Note.bRequired = true;
		ChartStates.Add(State);
	}

	ChartStates[0].bConsumed = true;
	ChartStates[1].bMissed = true;

	const EAkbalRhythmInstrument OriginalActive = EAkbalRhythmInstrument::Drum;
	const EAkbalRhythmInstrument SwitchedActive = EAkbalRhythmInstrument::Wind;
	TestNotEqual(TEXT("Switch changes active instrument only"), OriginalActive, SwitchedActive);

	TestTrue(TEXT("Consumed note stays consumed after instrument switch"), ChartStates[0].bConsumed);
	TestTrue(TEXT("Missed note stays missed after instrument switch"), ChartStates[1].bMissed);
	TestFalse(TEXT("Upcoming note remains available after instrument switch"), ChartStates[2].bConsumed);
	TestFalse(TEXT("Upcoming note is not auto-cleared by instrument switch"), ChartStates[2].bMissed);

	FAkbalRhythmChartNote NextNote;
	const float CurrentSeconds = 0.f;
	TestTrue(
		TEXT("Next required note still found after instrument switch"),
		FAkbalRhythmChartEvaluator::FindNextRequiredNote(ChartStates, CurrentSeconds, DefaultWindows, NextNote));
	TestEqual(TEXT("Next note is the unconsumed entry"), 2, NextNote.NoteId);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmChartInstrumentCountTest,
	"Akbal.Rhythm.ChartInstrumentCount",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmChartInstrumentCountTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRhythmChartTest;

	auto UsesOnlyFirstInstruments = [](const TArray<FAkbalRhythmChartNote>& Notes, int32 InstrumentCount) -> bool
	{
		for (const FAkbalRhythmChartNote& Note : Notes)
		{
			if (!Note.bRequired)
			{
				continue;
			}

			if (FAkbalRhythmInstrumentLibrary::IndexFromInstrument(Note.Instrument) >= InstrumentCount)
			{
				return false;
			}
		}

		return true;
	};

	for (int32 InstrumentCount = 1; InstrumentCount <= 4; ++InstrumentCount)
	{
		const TArray<FAkbalRhythmChartNote> Notes = FAkbalRhythmChartBuilder::BuildSpikeTestChart(TestBpm, InstrumentCount);
		TestTrue(
			*FString::Printf(TEXT("Spike chart respects instrument count %d"), InstrumentCount),
			UsesOnlyFirstInstruments(Notes, InstrumentCount));
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmNotePreviewVisibilityTest,
	"Akbal.Rhythm.NotePreviewVisibility",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmNotePreviewVisibilityTest::RunTest(const FString& Parameters)
{
	const float TargetSeconds = 5.f;
	const float PreviewSeconds = 1.f;
	const float ApproachSeconds = 2.25f;

	TestFalse(
		TEXT("Note outside preview window is hidden"),
		FAkbalRhythmChartEvaluator::IsNoteVisible(3.5f, TargetSeconds, PreviewSeconds, ApproachSeconds));

	TestTrue(
		TEXT("Note inside preview window and approaching is visible"),
		FAkbalRhythmChartEvaluator::IsNoteVisible(4.5f, TargetSeconds, PreviewSeconds, ApproachSeconds));

	TestFalse(
		TEXT("Note at hit time is hidden"),
		FAkbalRhythmChartEvaluator::IsNoteVisible(TargetSeconds, TargetSeconds, PreviewSeconds, ApproachSeconds));

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
