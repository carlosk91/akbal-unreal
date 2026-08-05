// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/AkbalRhythmJudgmentEvaluator.h"
#include "Input/AkbalRhythmJudgmentTypes.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace AkbalRhythmTest
{
	static const FAkbalRhythmTimingWindows DefaultWindows;
	static constexpr float TestBpm = 120.f;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmJudgmentTierTest,
	"Akbal.Rhythm.JudgmentTiers",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmJudgmentTierTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRhythmTest;

	TestEqual(TEXT("Perfect at 10ms"), EAkbalRhythmJudgment::Perfect, FAkbalRhythmJudgmentEvaluator::EvaluateDeltaMs(10.f, DefaultWindows));
	TestEqual(TEXT("Perfect at boundary"), EAkbalRhythmJudgment::Perfect, FAkbalRhythmJudgmentEvaluator::EvaluateDeltaMs(25.f, DefaultWindows));
	TestEqual(TEXT("Good at 40ms"), EAkbalRhythmJudgment::Good, FAkbalRhythmJudgmentEvaluator::EvaluateDeltaMs(40.f, DefaultWindows));
	TestEqual(TEXT("Pass at 70ms"), EAkbalRhythmJudgment::Pass, FAkbalRhythmJudgmentEvaluator::EvaluateDeltaMs(70.f, DefaultWindows));
	TestEqual(TEXT("Miss beyond pass"), EAkbalRhythmJudgment::Miss, FAkbalRhythmJudgmentEvaluator::EvaluateDeltaMs(100.f, DefaultWindows));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmNearestBeatTest,
	"Akbal.Rhythm.NearestBeatTarget",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmNearestBeatTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRhythmTest;

	const float SecondsPerBeat = FAkbalRhythmJudgmentEvaluator::GetSecondsPerBeat(TestBpm);
	TestEqual(TEXT("Beat 0"), 0.f, FAkbalRhythmJudgmentEvaluator::GetNearestBeatTargetSeconds(0.f, TestBpm));
	TestEqual(TEXT("Beat 1"), SecondsPerBeat, FAkbalRhythmJudgmentEvaluator::GetNearestBeatTargetSeconds(SecondsPerBeat + 0.01f, TestBpm));
	TestEqual(TEXT("Beat 2"), SecondsPerBeat * 2.f, FAkbalRhythmJudgmentEvaluator::GetNearestBeatTargetSeconds(SecondsPerBeat * 2.f - 0.01f, TestBpm));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRhythmFrameRateIndependenceTest,
	"Akbal.Rhythm.FrameRateIndependence",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalRhythmFrameRateIndependenceTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRhythmTest;

	const float IntendedBeatSeconds = FAkbalRhythmJudgmentEvaluator::GetSecondsPerBeat(TestBpm) * 4.f;
	const TArray<float> FrameRates = {30.f, 60.f, 120.f};
	EAkbalRhythmJudgment ReferenceJudgment = EAkbalRhythmJudgment::Miss;

	for (int32 Index = 0; Index < FrameRates.Num(); ++Index)
	{
		const float FrameDuration = 1.f / FrameRates[Index];
		const float CapturedTimestamp = FAkbalRhythmJudgmentEvaluator::SimulateFrameDelayedTimestamp(IntendedBeatSeconds, FrameDuration);
		const FAkbalRhythmJudgmentResult Result = FAkbalRhythmJudgmentEvaluator::JudgeInputAgainstNearestBeat(
			CapturedTimestamp, TestBpm, DefaultWindows);

		if (Index == 0)
		{
			ReferenceJudgment = Result.Judgment;
		}
		else
		{
			TestEqual(*FString::Printf(TEXT("Judgment stable at %.0f FPS"), FrameRates[Index]), ReferenceJudgment, Result.Judgment);
		}
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalMusicalPositionRoundTripTest,
	"Akbal.Rhythm.MusicalPositionRoundTrip",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAkbalMusicalPositionRoundTripTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRhythmTest;

	const float Seconds = 2.5f;
	const FAkbalMusicalPosition Position = FAkbalMusicalPosition::FromSeconds(Seconds, TestBpm, 4);
	const float TotalBeats = Position.GetTotalBeats(4);
	const float ReconstructedSeconds = TotalBeats * FAkbalRhythmJudgmentEvaluator::GetSecondsPerBeat(TestBpm);
	TestTrue(TEXT("Seconds round-trip within 1ms"), FMath::IsNearlyEqual(Seconds, ReconstructedSeconds, 0.001f));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
