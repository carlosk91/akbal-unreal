// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "Input/AkbalRhythmChartTypes.h"

#include "Input/AkbalRhythmJudgmentTypes.h"

#include "Input/AkbalRitualInputTypes.h"

#include "AkbalRitualInputSession.generated.h"



class UAkbalMusicConductorSubsystem;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAkbalRitualInputJudgment, const FAkbalRhythmJudgmentResult&, Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAkbalRitualInputSessionChanged);



UCLASS(BlueprintType)

class AKBAL_API UAkbalRitualInputSession : public UObject

{

	GENERATED_BODY()



public:

	void Initialize(UAkbalMusicConductorSubsystem* InConductor, const FAkbalRitualInputHudConfig& InConfig);



	const FAkbalRitualInputHudConfig& GetConfig() const { return Config; }

	const TArray<FAkbalRhythmChartNoteState>& GetChartStates() const { return ChartStates; }

	EAkbalRhythmInstrument GetActiveInstrument() const { return ActiveInstrument; }

	const FAkbalRhythmJudgmentResult& GetLastJudgment() const { return LastJudgment; }

	float GetJudgmentFlashTimeRemaining() const { return JudgmentFlashTimeRemaining; }



	bool IsInstrumentAvailable(EAkbalRhythmInstrument Instrument) const;

	void ApplyConfig(const FAkbalRitualInputHudConfig& NewConfig, bool bRestartEncounter = true);

	void RestartChart();

	bool ProcessLaneInput(EAkbalRhythmLane Lane);

	void SetActiveInstrument(EAkbalRhythmInstrument Instrument);

	void CycleActiveInstrument(int32 Delta);

	void UpdateChartState(float CurrentSeconds);

	void Tick(float DeltaSeconds);



	UPROPERTY(BlueprintAssignable, Category = "Akbal|Ritual")

	FOnAkbalRitualInputJudgment OnJudgment;



	UPROPERTY(BlueprintAssignable, Category = "Akbal|Ritual")

	FOnAkbalRitualInputSessionChanged OnSessionChanged;



private:

	void BuildChart();

	void ClampActiveInstrument();

	void BroadcastSessionChanged();



	UPROPERTY()

	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;



	FAkbalRitualInputHudConfig Config;

	TArray<FAkbalRhythmChartNoteState> ChartStates;

	FAkbalRhythmJudgmentResult LastJudgment;

	EAkbalRhythmInstrument ActiveInstrument = EAkbalRhythmInstrument::Drum;

	float JudgmentFlashTimeRemaining = 0.f;

};



