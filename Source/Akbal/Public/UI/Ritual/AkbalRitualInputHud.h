// Copyright Epic Games, Inc. All Rights Reserved.



#pragma once



#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Input/AkbalRhythmChartTypes.h"

#include "Input/AkbalRhythmJudgmentTypes.h"

#include "AkbalRitualInputHud.generated.h"



class UAkbalMusicConductorSubsystem;

class UAkbalRitualInputSession;



/** Production-facing ritual input overlay: lane notes traveling toward the sync center. */

UCLASS()

class AKBAL_API UAkbalRitualInputHud : public UUserWidget

{

	GENERATED_BODY()



public:

	void BindSession(UAkbalRitualInputSession* InSession, UAkbalMusicConductorSubsystem* InConductor);



protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual int32 NativePaint(

		const FPaintArgs& Args,

		const FGeometry& AllottedGeometry,

		const FSlateRect& MyCullingRect,

		FSlateWindowElementList& OutDrawElements,

		int32 LayerId,

		const FWidgetStyle& InWidgetStyle,

		bool bParentEnabled) const override;



private:

	FVector2D GetLanePosition(const FVector2D& Center, const FVector2D& Size, EAkbalRhythmLane Lane, float Progress, float EdgePadding) const;

	void DrawInstrumentNote(

		const FGeometry& AllottedGeometry,

		FSlateWindowElementList& OutDrawElements,

		int32 LayerId,

		const FVector2D& Position,

		float Radius,

		EAkbalRhythmInstrument Instrument,

		const FLinearColor& Color,

		bool bGhostNote) const;

	FLinearColor GetJudgmentColor(EAkbalRhythmJudgment Judgment) const;



	UPROPERTY()

	TObjectPtr<UAkbalRitualInputSession> Session;



	UPROPERTY()

	TObjectPtr<UAkbalMusicConductorSubsystem> Conductor;



	float GhostNoteAlpha = 0.42f;

	float GhostOutlineAlpha = 0.62f;

	float GhostNoteScale = 0.88f;

};


