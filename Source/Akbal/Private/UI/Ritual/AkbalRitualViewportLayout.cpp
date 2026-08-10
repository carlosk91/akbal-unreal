// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/Ritual/AkbalRitualViewportLayout.h"

float FAkbalRitualViewportLayout::ComputeSquareHudSize(
	int32 ViewportX,
	int32 ViewportY,
	float MaxViewportSizeFraction)
{
	const int32 MinDimension = FMath::Min(ViewportX, ViewportY);
	return static_cast<float>(MinDimension) * FMath::Clamp(MaxViewportSizeFraction, 0.05f, 0.5f);
}

FVector2D FAkbalRitualViewportLayout::ComputeFallbackScreenCenter(int32 ViewportX, int32 ViewportY)
{
	return FVector2D(static_cast<float>(ViewportX) * 0.5f, static_cast<float>(ViewportY) * 0.5f);
}
