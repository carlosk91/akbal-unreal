// Copyright Epic Games, Inc. All Rights Reserved.



#include "UI/Ritual/AkbalRitualInputHud.h"



#include "Audio/AkbalMusicConductorSubsystem.h"

#include "Input/AkbalRhythmChartEvaluator.h"
#include "Input/AkbalRhythmChartTypes.h"
#include "Input/AkbalRitualInputTypes.h"
#include "Rendering/DrawElements.h"

#include "Styling/CoreStyle.h"

#include "UI/Ritual/AkbalRitualInputSession.h"
#include "UI/Ritual/AkbalRitualUiLog.h"

void UAkbalRitualInputHud::BindSession(UAkbalRitualInputSession* InSession, UAkbalMusicConductorSubsystem* InConductor)

{

	Session = InSession;

	Conductor = InConductor;

}



void UAkbalRitualInputHud::NativeConstruct()

{

	Super::NativeConstruct();

	SetIsFocusable(false);

}



void UAkbalRitualInputHud::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)

{

	Super::NativeTick(MyGeometry, InDeltaTime);



	if (Session)

	{

		Session->Tick(InDeltaTime);

	}



	Invalidate(EInvalidateWidget::Paint);

}



FVector2D UAkbalRitualInputHud::GetLanePosition(

	const FVector2D& Center,

	const FVector2D& Size,

	EAkbalRhythmLane Lane,

	float Progress,

	float EdgePadding) const

{

	FVector2D Start = Center;

	switch (Lane)

	{

	case EAkbalRhythmLane::LeftToRight:

		Start = FVector2D(EdgePadding, Center.Y);

		break;

	case EAkbalRhythmLane::RightToLeft:

		Start = FVector2D(Size.X - EdgePadding, Center.Y);

		break;

	case EAkbalRhythmLane::TopToBottom:

		Start = FVector2D(Center.X, EdgePadding);

		break;

	case EAkbalRhythmLane::BottomToTop:

		Start = FVector2D(Center.X, Size.Y - EdgePadding);

		break;

	default:

		break;

	}



	return FMath::Lerp(Start, Center, Progress);

}



void UAkbalRitualInputHud::DrawInstrumentNote(

	const FGeometry& AllottedGeometry,

	FSlateWindowElementList& OutDrawElements,

	int32 LayerId,

	const FVector2D& Position,

	float Radius,

	EAkbalRhythmInstrument Instrument,

	const FLinearColor& Color,

	bool bGhostNote) const

{

	const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush("WhiteBrush");



	auto ScaleAlpha = [](const FLinearColor& Source, float Multiplier)

	{

		return FLinearColor(Source.R, Source.G, Source.B, Source.A * Multiplier);

	};



	auto DrawBox = [&](const FVector2D& TopLeft, const FVector2D& BoxSize, const FLinearColor& BoxColor, int32 LocalLayer)

	{

		FSlateDrawElement::MakeBox(

			OutDrawElements,

			LocalLayer,

			AllottedGeometry.ToPaintGeometry(FVector2f(BoxSize), FSlateLayoutTransform(FVector2f(TopLeft))),

			WhiteBrush,

			ESlateDrawEffect::None,

			BoxColor);

	};



	auto DrawPolyline = [&](const TArray<FVector2D>& Points, const FLinearColor& LineColor, float Thickness, int32 LocalLayer)

	{

		FSlateDrawElement::MakeLines(

			OutDrawElements,

			LocalLayer,

			AllottedGeometry.ToPaintGeometry(),

			Points,

			ESlateDrawEffect::None,

			LineColor,

			true,

			Thickness);

	};



	if (bGhostNote)

	{

		const FLinearColor OutlineColor = FLinearColor::White.CopyWithNewOpacity(GhostOutlineAlpha);

		DrawBox(

			Position - FVector2D(Radius + 2.f, Radius + 2.f),

			FVector2D((Radius + 2.f) * 2.f, (Radius + 2.f) * 2.f),

			OutlineColor,

			LayerId);

	}



	switch (Instrument)

	{

	case EAkbalRhythmInstrument::Drum:

		DrawBox(Position - FVector2D(Radius, Radius), FVector2D(Radius * 2.f, Radius * 2.f), Color, LayerId + 1);

		break;

	case EAkbalRhythmInstrument::Chime:

	{

		TArray<FVector2D> Diamond;

		Diamond.Add(Position + FVector2D(0.f, -Radius));

		Diamond.Add(Position + FVector2D(Radius, 0.f));

		Diamond.Add(Position + FVector2D(0.f, Radius));

		Diamond.Add(Position + FVector2D(-Radius, 0.f));

		Diamond.Add(Position + FVector2D(0.f, -Radius));

		DrawPolyline(Diamond, Color, bGhostNote ? 3.f : 4.f, LayerId + 1);

		DrawBox(

			Position - FVector2D(Radius * 0.45f, Radius * 0.45f),

			FVector2D(Radius * 0.9f, Radius * 0.9f),

			ScaleAlpha(Color, 0.5f),

			LayerId + 2);

		break;

	}

	case EAkbalRhythmInstrument::String:

		DrawBox(

			Position - FVector2D(Radius * 1.35f, Radius * 0.65f),

			FVector2D(Radius * 2.7f, Radius * 1.3f),

			Color,

			LayerId + 1);

		break;

	case EAkbalRhythmInstrument::Wind:

	default:

	{

		TArray<FVector2D> Triangle;

		Triangle.Add(Position + FVector2D(0.f, -Radius));

		Triangle.Add(Position + FVector2D(Radius * 0.95f, Radius * 0.85f));

		Triangle.Add(Position + FVector2D(-Radius * 0.95f, Radius * 0.85f));

		Triangle.Add(Position + FVector2D(0.f, -Radius));

		DrawPolyline(Triangle, Color, bGhostNote ? 3.f : 4.f, LayerId + 1);

		DrawBox(

			Position - FVector2D(Radius * 0.35f, Radius * 0.35f),

			FVector2D(Radius * 0.7f, Radius * 0.7f),

			ScaleAlpha(Color, 0.45f),

			LayerId + 2);

		break;

	}

	}

}



FLinearColor UAkbalRitualInputHud::GetJudgmentColor(EAkbalRhythmJudgment Judgment) const

{

	switch (Judgment)

	{

	case EAkbalRhythmJudgment::Perfect: return FLinearColor(1.f, 0.84f, 0.2f, 1.f);

	case EAkbalRhythmJudgment::Good: return FLinearColor(0.35f, 0.9f, 0.45f, 1.f);

	case EAkbalRhythmJudgment::Pass: return FLinearColor(0.95f, 0.75f, 0.2f, 1.f);

	default: return FLinearColor(0.95f, 0.3f, 0.3f, 1.f);

	}

}



int32 UAkbalRitualInputHud::NativePaint(

	const FPaintArgs& Args,

	const FGeometry& AllottedGeometry,

	const FSlateRect& MyCullingRect,

	FSlateWindowElementList& OutDrawElements,

	int32 LayerId,

	const FWidgetStyle& InWidgetStyle,

	bool bParentEnabled) const

{

	if (!Session || !Conductor)

	{

		return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	}



	const FVector2D Size = AllottedGeometry.GetLocalSize();
	if (Size.X < 1.f || Size.Y < 1.f)
	{
		return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	}

	const float SquareSize = FMath::Min(Size.X, Size.Y);

	const FVector2D Center(Size.X * 0.5f, Size.Y * 0.5f);

	const float EdgePadding = SquareSize * 0.08f;

	const float CenterZoneRadius = SquareSize * 0.18f;

	const float NoteRadius = SquareSize * 0.055f;

	const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush("WhiteBrush");



	const FAkbalRitualInputHudConfig& Config = Session->GetConfig();

	const EAkbalRhythmInstrument ActiveInstrument = Session->GetActiveInstrument();

	const float CurrentSeconds = Conductor->GetSecondsSinceTransportStart();



	auto DrawLine = [&](const FVector2D& Start, const FVector2D& End, const FLinearColor& Color, float Thickness)

	{

		TArray<FVector2D> Points;

		Points.Add(Start);

		Points.Add(End);

		FSlateDrawElement::MakeLines(

			OutDrawElements,

			LayerId + 1,

			AllottedGeometry.ToPaintGeometry(),

			Points,

			ESlateDrawEffect::None,

			Color,

			true,

			Thickness);

	};



	auto DrawCircle = [&](const FVector2D& Position, float Radius, const FLinearColor& Color, int32 LocalLayer)

	{

		const FVector2f TopLeft(Position.X - Radius, Position.Y - Radius);

		const FVector2f BoxSize(Radius * 2.f, Radius * 2.f);

		FSlateDrawElement::MakeBox(

			OutDrawElements,

			LocalLayer,

			AllottedGeometry.ToPaintGeometry(BoxSize, FSlateLayoutTransform(TopLeft)),

			WhiteBrush,

			ESlateDrawEffect::None,

			Color);

	};



	const FLinearColor ActiveLaneColor = FAkbalRhythmInstrumentLibrary::GetPrimaryColor(ActiveInstrument).CopyWithNewOpacity(0.65f);

	DrawLine(FVector2D(EdgePadding, Center.Y), Center, ActiveLaneColor, 3.f);

	DrawLine(FVector2D(Size.X - EdgePadding, Center.Y), Center, ActiveLaneColor, 3.f);

	DrawLine(FVector2D(Center.X, EdgePadding), Center, ActiveLaneColor, 3.f);

	DrawLine(FVector2D(Center.X, Size.Y - EdgePadding), Center, ActiveLaneColor, 3.f);



	FLinearColor CenterColor = FAkbalRhythmInstrumentLibrary::GetPrimaryColor(ActiveInstrument).CopyWithNewOpacity(0.22f);

	if (Session->GetJudgmentFlashTimeRemaining() > 0.f)

	{

		const float FlashAlpha = FMath::Clamp(Session->GetJudgmentFlashTimeRemaining() / 0.35f, 0.f, 1.f);

		CenterColor = FMath::Lerp(

			CenterColor,

			GetJudgmentColor(Session->GetLastJudgment().Judgment).CopyWithNewOpacity(0.55f),

			FlashAlpha);

	}

	DrawCircle(Center, CenterZoneRadius, CenterColor, LayerId + 2);

	DrawCircle(

		Center,

		CenterZoneRadius,

		FAkbalRhythmInstrumentLibrary::GetPrimaryColor(ActiveInstrument).CopyWithNewOpacity(0.35f),

		LayerId + 3);



	const int32 GhostNoteLayer = LayerId + 5;

	const int32 ActiveNoteLayer = LayerId + 8;

	int32 VisibleNoteCount = 0;



	auto DrawVisibleNote = [&](const FAkbalRhythmChartNoteState& State, bool bActiveInst, int32 LocalLayer)

	{

		if (!FAkbalRhythmChartEvaluator::IsNoteVisible(

			CurrentSeconds,

			State.Note.TargetSeconds,

			Config.NotePreviewSeconds,

			Config.NoteApproachSeconds))

		{

			return;

		}



		const float Progress = FAkbalRhythmChartEvaluator::GetNoteApproachProgress(

			CurrentSeconds, State.Note.TargetSeconds, Config.NoteApproachSeconds);

		const FVector2D NotePosition = GetLanePosition(Center, Size, State.Note.Lane, Progress, EdgePadding);

		const FLinearColor BaseColor = FAkbalRhythmInstrumentLibrary::GetPrimaryColor(State.Note.Instrument);

		const float Radius = bActiveInst ? NoteRadius : NoteRadius * GhostNoteScale;

		const FLinearColor NoteColor = bActiveInst ? BaseColor : BaseColor.CopyWithNewOpacity(GhostNoteAlpha);

		DrawInstrumentNote(AllottedGeometry, OutDrawElements, LocalLayer, NotePosition, Radius, State.Note.Instrument, NoteColor, !bActiveInst);

		++VisibleNoteCount;

	};



	for (const FAkbalRhythmChartNoteState& State : Session->GetChartStates())

	{

		if (State.bConsumed || State.bMissed || !State.Note.bRequired)

		{

			continue;

		}



		if (State.Note.Instrument == ActiveInstrument)

		{

			continue;

		}



		DrawVisibleNote(State, false, GhostNoteLayer);

	}



	for (const FAkbalRhythmChartNoteState& State : Session->GetChartStates())

	{

		if (State.bConsumed || State.bMissed || !State.Note.bRequired)

		{

			continue;

		}



		if (State.Note.Instrument != ActiveInstrument)

		{

			continue;

		}



		DrawVisibleNote(State, true, ActiveNoteLayer);

	}

	static double LastPaintLogSeconds = 0.0;
	const double NowSeconds = FPlatformTime::Seconds();
	if (NowSeconds - LastPaintLogSeconds > 2.0)
	{
		LastPaintLogSeconds = NowSeconds;
		UE_LOG(LogAkbalRitualUi, Verbose,
			TEXT("Ritual HUD paint: size=(%.0f, %.0f) t=%.2f visibleNotes=%d chartNotes=%d"),
			Size.X,
			Size.Y,
			CurrentSeconds,
			VisibleNoteCount,
			Session->GetChartStates().Num());
	}

	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId + 10, InWidgetStyle, bParentEnabled);

}


