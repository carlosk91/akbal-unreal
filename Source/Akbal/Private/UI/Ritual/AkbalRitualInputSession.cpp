// Copyright Epic Games, Inc. All Rights Reserved.



#include "UI/Ritual/AkbalRitualInputSession.h"



#include "Audio/AkbalMusicConductorSubsystem.h"

#include "Input/AkbalRhythmChartEvaluator.h"



void UAkbalRitualInputSession::Initialize(UAkbalMusicConductorSubsystem* InConductor, const FAkbalRitualInputHudConfig& InConfig)

{

	Conductor = InConductor;

	Config = InConfig;

	Config.AvailableInstrumentCount = FMath::Clamp(Config.AvailableInstrumentCount, 1, FAkbalRhythmInstrumentLibrary::InstrumentCount);

	ClampActiveInstrument();

	BuildChart();

}



bool UAkbalRitualInputSession::IsInstrumentAvailable(EAkbalRhythmInstrument Instrument) const

{

	return FAkbalRhythmInstrumentLibrary::IndexFromInstrument(Instrument) < Config.AvailableInstrumentCount;

}



void UAkbalRitualInputSession::ApplyConfig(const FAkbalRitualInputHudConfig& NewConfig, bool bRestartEncounter)

{

	Config = NewConfig;

	Config.AvailableInstrumentCount = FMath::Clamp(Config.AvailableInstrumentCount, 1, FAkbalRhythmInstrumentLibrary::InstrumentCount);

	ClampActiveInstrument();

	BuildChart();



	if (Conductor && bRestartEncounter)

	{

		Conductor->StopEncounter(true);

		Conductor->StartEncounter(Config.BeatsPerMinute, Config.BeatsPerBar);

	}



	BroadcastSessionChanged();

}



void UAkbalRitualInputSession::RestartChart()

{

	if (Conductor)

	{

		Conductor->StopEncounter(true);

		Conductor->StartEncounter(Config.BeatsPerMinute, Config.BeatsPerBar);

	}



	BuildChart();

	BroadcastSessionChanged();

}



void UAkbalRitualInputSession::BuildChart()

{

	ChartStates.Reset();

	const TArray<FAkbalRhythmChartNote> Notes = FAkbalRhythmChartBuilder::BuildSpikeTestChart(

		Config.BeatsPerMinute,

		Config.AvailableInstrumentCount);



	for (const FAkbalRhythmChartNote& Note : Notes)

	{

		FAkbalRhythmChartNoteState State;

		State.Note = Note;

		ChartStates.Add(State);

	}

}



void UAkbalRitualInputSession::ClampActiveInstrument()

{

	if (!IsInstrumentAvailable(ActiveInstrument))

	{

		ActiveInstrument = FAkbalRhythmInstrumentLibrary::InstrumentFromIndex(0);

	}

}



void UAkbalRitualInputSession::SetActiveInstrument(EAkbalRhythmInstrument Instrument)

{

	if (!IsInstrumentAvailable(Instrument))

	{

		return;

	}



	ActiveInstrument = Instrument;

	BroadcastSessionChanged();

}



void UAkbalRitualInputSession::CycleActiveInstrument(int32 Delta)

{

	if (Config.AvailableInstrumentCount <= 1)

	{

		return;

	}



	int32 CurrentIndex = FAkbalRhythmInstrumentLibrary::IndexFromInstrument(ActiveInstrument);

	for (int32 Step = 0; Step < Config.AvailableInstrumentCount; ++Step)

	{

		CurrentIndex = (CurrentIndex + Delta + FAkbalRhythmInstrumentLibrary::InstrumentCount)

			% FAkbalRhythmInstrumentLibrary::InstrumentCount;

		const EAkbalRhythmInstrument Candidate = FAkbalRhythmInstrumentLibrary::InstrumentFromIndex(CurrentIndex);

		if (IsInstrumentAvailable(Candidate))

		{

			SetActiveInstrument(Candidate);

			return;

		}

	}

}



void UAkbalRitualInputSession::UpdateChartState(float CurrentSeconds)

{

	if (!Conductor)

	{

		return;

	}



	const FAkbalRhythmTimingWindows& Windows = Conductor->TimingWindows;

	for (FAkbalRhythmChartNoteState& State : ChartStates)

	{

		if (State.bConsumed || State.bMissed || !State.Note.bRequired)

		{

			continue;

		}



		if (FAkbalRhythmChartEvaluator::ShouldAutoMissNote(CurrentSeconds, State.Note.TargetSeconds, Windows))

		{

			State.bMissed = true;

		}

	}

}



bool UAkbalRitualInputSession::ProcessLaneInput(EAkbalRhythmLane Lane)

{

	if (!Conductor)

	{

		return false;

	}



	const float RawInputSeconds = Conductor->GetSecondsSinceTransportStart();

	const float AdjustedInputSeconds = Conductor->ApplyLatencyCompensation(RawInputSeconds);



	int32 MatchedNoteIndex = INDEX_NONE;

	LastJudgment = FAkbalRhythmChartEvaluator::JudgeInputAgainstChart(

		AdjustedInputSeconds,

		Lane,

		ActiveInstrument,

		ChartStates,

		Conductor->TimingWindows,

		MatchedNoteIndex);

	LastJudgment.InputSeconds = RawInputSeconds;



	if (MatchedNoteIndex != INDEX_NONE)

	{

		ChartStates[MatchedNoteIndex].bConsumed = true;

	}



	JudgmentFlashTimeRemaining = 0.35f;

	OnJudgment.Broadcast(LastJudgment);

	return MatchedNoteIndex != INDEX_NONE;

}



void UAkbalRitualInputSession::Tick(float DeltaSeconds)

{

	if (!Conductor)

	{

		return;

	}



	UpdateChartState(Conductor->GetSecondsSinceTransportStart());

	if (JudgmentFlashTimeRemaining > 0.f)

	{

		JudgmentFlashTimeRemaining = FMath::Max(0.f, JudgmentFlashTimeRemaining - DeltaSeconds);

	}

}



void UAkbalRitualInputSession::BroadcastSessionChanged()

{

	OnSessionChanged.Broadcast();

}



