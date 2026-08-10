// Copyright Epic Games, Inc. All Rights Reserved.



#include "Spike/AkbalRhythmSpikePlayerController.h"



#include "Audio/AkbalMetronomeSoundFactory.h"

#include "Audio/AkbalMusicConductorSubsystem.h"

#include "Audio/AkbalMusicalPosition.h"

#include "Engine/Engine.h"

#include "Kismet/GameplayStatics.h"

#include "UI/Dev/AkbalRhythmHarnessWidget.h"

#include "UI/Ritual/AkbalRitualInputHud.h"

#include "UI/Ritual/AkbalRitualInputSession.h"
#include "UI/Ritual/AkbalRitualViewportLayout.h"
#include "UI/Ritual/AkbalRitualUiLog.h"
#include "TimerManager.h"



AAkbalRhythmSpikePlayerController::AAkbalRhythmSpikePlayerController()

{

	bShowMouseCursor = true;

	bEnableClickEvents = true;

	bEnableMouseOverEvents = true;

	PrimaryActorTick.bCanEverTick = true;

}



void AAkbalRhythmSpikePlayerController::BeginPlay()

{

	Super::BeginPlay();



	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	SetShowMouseCursor(true);



	InitializeSpike();

}



void AAkbalRhythmSpikePlayerController::SetupInputComponent()

{

	Super::SetupInputComponent();



	if (!InputComponent)

	{

		return;

	}



	InputComponent->BindKey(EKeys::Left, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnLeftLanePressed);

	InputComponent->BindKey(EKeys::Right, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnRightLanePressed);

	InputComponent->BindKey(EKeys::Up, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnUpLanePressed);

	InputComponent->BindKey(EKeys::Down, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnDownLanePressed);

	InputComponent->BindKey(EKeys::Q, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnInstrumentPreviousPressed);

	InputComponent->BindKey(EKeys::E, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnInstrumentNextPressed);

	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnRestartPressed);

	InputComponent->BindKey(EKeys::P, IE_Pressed, this, &AAkbalRhythmSpikePlayerController::OnTogglePausePressed);

}



bool AAkbalRhythmSpikePlayerController::InputKey(const FInputKeyEventArgs& Params)

{

	if (TryHandleRhythmKey(Params.Key, Params.Event))

	{

		return true;

	}



	return Super::InputKey(Params);

}



bool AAkbalRhythmSpikePlayerController::TryHandleRhythmKey(const FKey& Key, EInputEvent Event)

{

	if (Event != IE_Pressed)

	{

		return false;

	}



	if (Key == EKeys::Left) { OnLeftLanePressed(); return true; }

	if (Key == EKeys::Right) { OnRightLanePressed(); return true; }

	if (Key == EKeys::Up) { OnUpLanePressed(); return true; }

	if (Key == EKeys::Down) { OnDownLanePressed(); return true; }

	if (Key == EKeys::Q) { OnInstrumentPreviousPressed(); return true; }

	if (Key == EKeys::E) { OnInstrumentNextPressed(); return true; }



	return false;

}



void AAkbalRhythmSpikePlayerController::Tick(float DeltaSeconds)

{

	Super::Tick(DeltaSeconds);



	UpdateRitualHudLayout();



	if (!bShowDebugOverlay || !Conductor)

	{

		return;

	}



	if (bUseRitualHud && RitualInputHud)

	{

		return;

	}



	DrawDebugOverlay();

}



void AAkbalRhythmSpikePlayerController::InitializeSpike()

{

	if (bSpikeInitialized)

	{

		return;

	}



	Conductor = UAkbalMusicConductorSubsystem::Get(this);



	RitualInputSession = NewObject<UAkbalRitualInputSession>(this);

	if (RitualInputSession && Conductor)

	{

		RitualInputSession->Initialize(Conductor, DefaultHudConfig);

		RitualInputSession->OnJudgment.AddDynamic(this, &AAkbalRhythmSpikePlayerController::HandleSessionJudgment);



		if (bAutoStartEncounter)

		{

			Conductor->StartEncounter(DefaultHudConfig.BeatsPerMinute, DefaultHudConfig.BeatsPerBar);

		}



		Conductor->OnBeat.AddDynamic(this, &AAkbalRhythmSpikePlayerController::HandleConductorBeat);

	}



	const bool bWidgetsReady = CreateRitualWidgets();
	bSpikeInitialized = true;

	LogRitualUiState();

	if (!bWidgetsReady && GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			WidgetRetryHandle,
			this,
			&AAkbalRhythmSpikePlayerController::RetryCreateRitualWidgets,
			0.1f,
			false);
	}

	if (GEngine)

	{

		const FString Status = bWidgetsReady

			? TEXT("Akbal ritual input ready (centered square HUD + harness).")

			: TEXT("Akbal ritual input failed to create widgets.");

		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, Status);

	}

}



bool AAkbalRhythmSpikePlayerController::CreateRitualWidgets()

{

	if (!RitualInputSession)

	{

		UE_LOG(LogAkbalRitualUi, Warning, TEXT("CreateRitualWidgets: no session"));

		return false;

	}



	if (bUseRitualHud && !RitualInputHud)

	{

		RitualInputHud = CreateWidget<UAkbalRitualInputHud>(this, UAkbalRitualInputHud::StaticClass());

		if (RitualInputHud)
		{
			RitualInputHud->BindSession(RitualInputSession, Conductor);
			RitualInputHud->AddToViewport(50);
			RitualInputHud->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

	}



	if (bUseRhythmHarness && !RhythmHarness)

	{

		RhythmHarness = CreateWidget<UAkbalRhythmHarnessWidget>(this, UAkbalRhythmHarnessWidget::StaticClass());

		if (RhythmHarness)
		{
			RhythmHarness->BindHarness(RitualInputSession, Conductor, this);
			RhythmHarness->EnsureHarnessBuilt();
			RhythmHarness->AddToViewport(1000);
			RhythmHarness->SetAnchorsInViewport(FAnchors(0.f, 0.f, 0.f, 0.f));
			RhythmHarness->SetAlignmentInViewport(FVector2D(0.f, 0.f));
			RhythmHarness->SetPositionInViewport(FVector2D(16.f, 16.f));

			const FVector2D HarnessSize = RhythmHarness->MeasureHarnessSize();
			RhythmHarness->SetDesiredSizeInViewport(HarnessSize);
			RhythmHarness->SetVisibility(ESlateVisibility::Visible);
			RhythmHarness->ForceLayoutPrepass();

			UE_LOG(LogAkbalRitualUi, Log,
				TEXT("Harness added to viewport: built=%d size=(%.0f, %.0f)"),
				RhythmHarness->HasBuiltContent(),
				HarnessSize.X,
				HarnessSize.Y);
		}
		else
		{
			UE_LOG(LogAkbalRitualUi, Error, TEXT("Failed to create harness widget"));
		}

	}



	UpdateRitualHudLayout();

	return RitualInputHud != nullptr || RhythmHarness != nullptr;

}



void AAkbalRhythmSpikePlayerController::RetryCreateRitualWidgets()

{

	if (CreateRitualWidgets())

	{

		LogRitualUiState();

		if (GetWorld())

		{

			GetWorld()->GetTimerManager().ClearTimer(WidgetRetryHandle);

		}

	}

}



void AAkbalRhythmSpikePlayerController::LogRitualUiState() const

{

	int32 ViewportX = 0;
	int32 ViewportY = 0;
	GetViewportSize(ViewportX, ViewportY);

	const float HudSize = RitualInputSession
		? FAkbalRitualViewportLayout::ComputeSquareHudSize(
			ViewportX,
			ViewportY,
			RitualInputSession->GetConfig().MaxViewportSizeFraction)
		: 0.f;

	UE_LOG(LogAkbalRitualUi, Log,
		TEXT("Ritual UI state: viewport=%dx%d hud=%s harness=%s hudSize=%.0f harnessBuilt=%d chartNotes=%d"),
		ViewportX,
		ViewportY,
		RitualInputHud ? TEXT("yes") : TEXT("no"),
		RhythmHarness ? TEXT("yes") : TEXT("no"),
		HudSize,
		RhythmHarness && RhythmHarness->HasBuiltContent(),
		RitualInputSession ? RitualInputSession->GetChartStates().Num() : 0);

	if (GEngine)
	{
		const FString Overlay = FString::Printf(
			TEXT("Akbal UI | HUD:%s Harness:%s | Viewport %dx%d"),
			RitualInputHud ? TEXT("OK") : TEXT("missing"),
			(RhythmHarness && RhythmHarness->HasBuiltContent()) ? TEXT("OK") : TEXT("missing"),
			ViewportX,
			ViewportY);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 8.f, FColor::Green, Overlay);
	}

}



void AAkbalRhythmSpikePlayerController::UpdateRitualHudLayout()

{

	if (!RitualInputHud || !RitualInputSession)

	{

		return;

	}



	int32 ViewportX = 0;

	int32 ViewportY = 0;

	GetViewportSize(ViewportX, ViewportY);

	if (ViewportX <= 0 || ViewportY <= 0)

	{

		return;

	}



	const float HudSize = FAkbalRitualViewportLayout::ComputeSquareHudSize(
		ViewportX,
		ViewportY,
		RitualInputSession->GetConfig().MaxViewportSizeFraction);

	FVector2D ScreenPosition = FAkbalRitualViewportLayout::ComputeFallbackScreenCenter(ViewportX, ViewportY);
	if (const APawn* ControlledPawn = GetPawn())
	{
		FVector2D ProjectedPosition;
		if (ProjectWorldLocationToScreen(ControlledPawn->GetActorLocation(), ProjectedPosition, true))
		{
			ScreenPosition = ProjectedPosition;
		}
	}

	RitualInputHud->SetAnchorsInViewport(FAnchors(0.f, 0.f, 0.f, 0.f));
	RitualInputHud->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
	RitualInputHud->SetDesiredSizeInViewport(FVector2D(HudSize, HudSize));
	RitualInputHud->SetPositionInViewport(ScreenPosition, false);

}



void AAkbalRhythmSpikePlayerController::DrawDebugOverlay() const

{

	if (!GEngine || !Conductor)

	{

		return;

	}



	const FAkbalConductorDebugSnapshot Snapshot = Conductor->GetDebugSnapshot();

	const FString StateText = Snapshot.bPaused ? TEXT("Paused") : (Snapshot.bClockRunning ? TEXT("Running") : TEXT("Stopped"));

	const FString Overlay = FString::Printf(

		TEXT("Akbal Rhythm | %s | %.0f BPM | Bar %d Beat %d"),

		*StateText,

		Snapshot.BeatsPerMinute,

		Snapshot.Position.Bar,

		Snapshot.Position.Beat);



	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Cyan, Overlay);

}



void AAkbalRhythmSpikePlayerController::OnLeftLanePressed()

{

	OnLanePressed(EAkbalRhythmLane::LeftToRight);

}



void AAkbalRhythmSpikePlayerController::OnRightLanePressed()

{

	OnLanePressed(EAkbalRhythmLane::RightToLeft);

}



void AAkbalRhythmSpikePlayerController::OnUpLanePressed()

{

	OnLanePressed(EAkbalRhythmLane::TopToBottom);

}



void AAkbalRhythmSpikePlayerController::OnDownLanePressed()

{

	OnLanePressed(EAkbalRhythmLane::BottomToTop);

}



void AAkbalRhythmSpikePlayerController::OnLanePressed(EAkbalRhythmLane Lane)

{

	if (RitualInputSession)

	{

		RitualInputSession->ProcessLaneInput(Lane);

	}

}



void AAkbalRhythmSpikePlayerController::CycleInstrument(int32 Delta)

{

	if (RitualInputSession)

	{

		RitualInputSession->CycleActiveInstrument(Delta);

	}

}



void AAkbalRhythmSpikePlayerController::OnInstrumentPreviousPressed()

{

	CycleInstrument(-1);

}



void AAkbalRhythmSpikePlayerController::OnInstrumentNextPressed()

{

	CycleInstrument(1);

}



void AAkbalRhythmSpikePlayerController::HandleSessionJudgment(const FAkbalRhythmJudgmentResult& Result)

{

	UE_LOG(LogTemp, Log, TEXT("Akbal ritual input: %s (delta %.1f ms)"),

		*UEnum::GetValueAsString(Result.Judgment), Result.DeltaMs);

}



void AAkbalRhythmSpikePlayerController::HandleConductorBeat(const FAkbalMusicalPosition& Position)

{

	if (!bMetronomeEnabled || !Conductor || Conductor->IsEncounterPaused())

	{

		return;

	}



	const bool bAccent = Position.Beat == 1;

	const float FrequencyHz = bAccent ? MetronomeAccentFrequencyHz : MetronomeTickFrequencyHz;

	const float DurationSeconds = bAccent ? 0.08f : 0.05f;

	FAkbalMetronomeSoundFactory::PlayClickTone(this, this, FrequencyHz, DurationSeconds, MetronomeVolume);

}



void AAkbalRhythmSpikePlayerController::OnRestartPressed()

{

	if (RitualInputSession)

	{

		RitualInputSession->RestartChart();

	}

}



void AAkbalRhythmSpikePlayerController::OnTogglePausePressed()

{

	if (!Conductor)

	{

		return;

	}



	const bool bShouldPause = !IsPaused();

	SetPause(bShouldPause);



	if (bShouldPause)

	{

		Conductor->PauseEncounter();

	}

	else

	{

		Conductor->ResumeEncounter();

	}

}


