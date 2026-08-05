// Copyright Epic Games, Inc. All Rights Reserved.

#include "Audio/AkbalMusicConductorSubsystem.h"

#include "Engine/World.h"
#include "Input/AkbalRhythmJudgmentEvaluator.h"
#include "Quartz/QuartzSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"

const FName UAkbalMusicConductorSubsystem::EncounterClockName(TEXT("AkbalEncounterClock"));

UAkbalMusicConductorSubsystem* UAkbalMusicConductorSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	return World ? World->GetSubsystem<UAkbalMusicConductorSubsystem>() : nullptr;
}

void UAkbalMusicConductorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	TimingWindows = FAkbalRhythmTimingWindows();
}

void UAkbalMusicConductorSubsystem::Deinitialize()
{
	StopEncounter(true);
	Super::Deinitialize();
}

bool UAkbalMusicConductorSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE || WorldType == EWorldType::Editor;
}

void UAkbalMusicConductorSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TStatId UAkbalMusicConductorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UAkbalMusicConductorSubsystem, STATGROUP_Tickables);
}

bool UAkbalMusicConductorSubsystem::StartEncounter(float BeatsPerMinute, int32 BeatsPerBar)
{
	if (BeatsPerMinute <= KINDA_SMALL_NUMBER)
	{
		return false;
	}

	if (bEncounterRunning && !bEncounterPaused)
	{
		return true;
	}

	ActiveBeatsPerMinute = BeatsPerMinute;
	ActiveBeatsPerBar = FMath::Max(1, BeatsPerBar);
	EnsureClockExists();

	UQuartzClockHandle* Handle = GetClockHandle();
	UQuartzSubsystem* QuartzSubsystem = GetQuartzSubsystem();
	if (!Handle || !QuartzSubsystem)
	{
		return false;
	}

	if (bEncounterPaused)
	{
		Handle->ResumeClock(this, ClockHandle);
		bEncounterPaused = false;
		bEncounterRunning = true;
		return true;
	}

	FQuartzQuantizationBoundary QuantizationBoundary;
	FOnQuartzCommandEventBP CommandDelegate;
	Handle->SetBeatsPerMinute(this, QuantizationBoundary, CommandDelegate, ClockHandle, ActiveBeatsPerMinute);
	Handle->StartClock(this, ClockHandle);
	SubscribeToBeatEvents();

	bEncounterRunning = true;
	bEncounterPaused = false;
	BeatCallbackCount = 0;
	return true;
}

void UAkbalMusicConductorSubsystem::StopEncounter(bool bCancelPendingEvents)
{
	if (UQuartzClockHandle* Handle = GetClockHandle())
	{
		UnsubscribeFromBeatEvents();
		Handle->StopClock(this, bCancelPendingEvents, ClockHandle);
	}

	bEncounterRunning = false;
	bEncounterPaused = false;
}

void UAkbalMusicConductorSubsystem::PauseEncounter()
{
	if (!bEncounterRunning || bEncounterPaused)
	{
		return;
	}

	if (UQuartzClockHandle* Handle = GetClockHandle())
	{
		Handle->PauseClock(this, ClockHandle);
		bEncounterPaused = true;
	}
}

void UAkbalMusicConductorSubsystem::ResumeEncounter()
{
	if (!bEncounterRunning || !bEncounterPaused)
	{
		return;
	}

	if (UQuartzClockHandle* Handle = GetClockHandle())
	{
		Handle->ResumeClock(this, ClockHandle);
		bEncounterPaused = false;
	}
}

FAkbalMusicalPosition UAkbalMusicConductorSubsystem::GetMusicalPosition()
{
	FAkbalMusicalPosition Position;
	Position.SecondsSinceTransportStart = GetSecondsSinceTransportStart();

	if (UQuartzClockHandle* Handle = GetClockHandle())
	{
		const FQuartzTransportTimeStamp Timestamp = Handle->GetCurrentTimestamp(this);
		Position.Bar = Timestamp.Bars + 1;
		Position.Beat = Timestamp.Beat + 1;
		Position.BeatFraction = Timestamp.BeatFraction;
		return Position;
	}

	return FAkbalMusicalPosition::FromSeconds(Position.SecondsSinceTransportStart, ActiveBeatsPerMinute, ActiveBeatsPerBar);
}

float UAkbalMusicConductorSubsystem::GetSecondsSinceTransportStart()
{
	if (UQuartzClockHandle* Handle = GetClockHandle())
	{
		return Handle->GetEstimatedRunTime(this);
	}
	return 0.f;
}

FAkbalRhythmJudgmentResult UAkbalMusicConductorSubsystem::JudgeInputAtSeconds(float InputSeconds)
{
	if (!bEncounterRunning)
	{
		LastJudgment = FAkbalRhythmJudgmentResult();
		LastJudgment.InputSeconds = InputSeconds;
		LastJudgment.Judgment = EAkbalRhythmJudgment::Miss;
		return LastJudgment;
	}

	LastJudgment = FAkbalRhythmJudgmentEvaluator::JudgeInputAgainstNearestBeat(InputSeconds, ActiveBeatsPerMinute, TimingWindows);
	return LastJudgment;
}

FAkbalConductorDebugSnapshot UAkbalMusicConductorSubsystem::GetDebugSnapshot()
{
	FAkbalConductorDebugSnapshot Snapshot;
	Snapshot.bClockRunning = bEncounterRunning;
	Snapshot.bPaused = bEncounterPaused;
	Snapshot.BeatsPerMinute = ActiveBeatsPerMinute;
	Snapshot.BeatsPerBar = ActiveBeatsPerBar;
	Snapshot.Position = GetMusicalPosition();
	Snapshot.BeatCallbackCount = BeatCallbackCount;
	Snapshot.LastJudgment = LastJudgment;

	if (UQuartzSubsystem* QuartzSubsystem = GetQuartzSubsystem())
	{
		Snapshot.RoundTripLatencyMs = QuartzSubsystem->GetRoundTripAverageLatency(this) * 1000.f;
		Snapshot.GameToAudioLatencyMs = QuartzSubsystem->GetGameThreadToAudioRenderThreadAverageLatency(this) * 1000.f;
	}

	return Snapshot;
}

void UAkbalMusicConductorSubsystem::EnsureClockExists()
{
	UQuartzSubsystem* QuartzSubsystem = GetQuartzSubsystem();
	if (!QuartzSubsystem)
	{
		return;
	}

	if (QuartzSubsystem->DoesClockExist(this, EncounterClockName))
	{
		ClockHandle = QuartzSubsystem->GetHandleForClock(this, EncounterClockName);
		return;
	}

	FQuartzClockSettings ClockSettings;
	ClockSettings.TimeSignature.NumBeats = ActiveBeatsPerBar;
	ClockHandle = QuartzSubsystem->CreateNewClock(this, EncounterClockName, ClockSettings, true, true);
}

void UAkbalMusicConductorSubsystem::SubscribeToBeatEvents()
{
	UQuartzClockHandle* Handle = GetClockHandle();
	if (!Handle)
	{
		return;
	}

	FOnQuartzMetronomeEventBP BeatDelegate;
	BeatDelegate.BindUFunction(this, FName("HandleBeatEvent"));
	Handle->SubscribeToQuantizationEvent(this, EQuartzCommandQuantization::Beat, BeatDelegate, ClockHandle);
}

void UAkbalMusicConductorSubsystem::UnsubscribeFromBeatEvents()
{
	if (UQuartzClockHandle* Handle = GetClockHandle())
	{
		Handle->UnsubscribeFromTimeDivision(this, EQuartzCommandQuantization::Beat, ClockHandle);
	}
}

void UAkbalMusicConductorSubsystem::HandleBeatEvent(
	FName ClockName,
	EQuartzCommandQuantization QuantizationType,
	int32 NumBars,
	int32 Beat,
	float BeatFraction)
{
	FAkbalMusicalPosition Position;
	Position.Bar = NumBars + 1;
	Position.Beat = Beat + 1;
	Position.BeatFraction = BeatFraction;
	Position.SecondsSinceTransportStart = GetSecondsSinceTransportStart();
	LastBeatPosition = Position;
	++BeatCallbackCount;
	OnBeat.Broadcast(Position);
}

UQuartzSubsystem* UAkbalMusicConductorSubsystem::GetQuartzSubsystem()
{
	return UQuartzSubsystem::Get(GetWorld());
}

UQuartzClockHandle* UAkbalMusicConductorSubsystem::GetClockHandle()
{
	if (ClockHandle)
	{
		return ClockHandle;
	}

	if (UQuartzSubsystem* QuartzSubsystem = GetQuartzSubsystem())
	{
		return QuartzSubsystem->GetHandleForClock(this, EncounterClockName);
	}

	return nullptr;
}
