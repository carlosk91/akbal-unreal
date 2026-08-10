// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Pure viewport math for ritual HUD placement (unit-testable, no UMG dependency). */
struct FAkbalRitualViewportLayout
{
	static float ComputeSquareHudSize(int32 ViewportX, int32 ViewportY, float MaxViewportSizeFraction);

	static FVector2D ComputeFallbackScreenCenter(int32 ViewportX, int32 ViewportY);
};
