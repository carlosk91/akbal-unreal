// Copyright Epic Games, Inc. All Rights Reserved.

#include "Audio/AkbalMusicalPosition.h"

float FAkbalMusicalPosition::GetTotalBeats(int32 InBeatsPerBar) const
{
	const int32 SafeBeatsPerBar = FMath::Max(1, InBeatsPerBar);
	const float ZeroBasedBar = static_cast<float>(FMath::Max(0, Bar - 1));
	const float ZeroBasedBeat = static_cast<float>(FMath::Max(0, Beat - 1));
	return (ZeroBasedBar * SafeBeatsPerBar) + ZeroBasedBeat + BeatFraction;
}

FAkbalMusicalPosition FAkbalMusicalPosition::FromTotalBeats(float TotalBeats, int32 InBeatsPerBar, float SecondsSinceStart)
{
	FAkbalMusicalPosition Position;
	Position.SecondsSinceTransportStart = SecondsSinceStart;

	const int32 SafeBeatsPerBar = FMath::Max(1, InBeatsPerBar);
	const float ClampedTotalBeats = FMath::Max(0.f, TotalBeats);
	const int32 ZeroBasedBeatIndex = FMath::FloorToInt(ClampedTotalBeats);
	const int32 BarIndex = ZeroBasedBeatIndex / SafeBeatsPerBar;
	const int32 BeatIndex = ZeroBasedBeatIndex % SafeBeatsPerBar;

	Position.Bar = BarIndex + 1;
	Position.Beat = BeatIndex + 1;
	Position.BeatFraction = ClampedTotalBeats - static_cast<float>(ZeroBasedBeatIndex);
	return Position;
}

FAkbalMusicalPosition FAkbalMusicalPosition::FromSeconds(float Seconds, float BeatsPerMinute, int32 InBeatsPerBar)
{
	const float SecondsPerBeat = (BeatsPerMinute > KINDA_SMALL_NUMBER) ? (60.f / BeatsPerMinute) : 0.f;
	const float TotalBeats = (SecondsPerBeat > KINDA_SMALL_NUMBER) ? (Seconds / SecondsPerBeat) : 0.f;
	return FromTotalBeats(TotalBeats, InBeatsPerBar, Seconds);
}
