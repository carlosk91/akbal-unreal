// Copyright Epic Games, Inc. All Rights Reserved.

#include "Blueprint/WidgetTree.h"
#include "Components/Widget.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Input/AkbalRitualInputTypes.h"
#include "Misc/AutomationTest.h"
#include "Widgets/SWidget.h"
#include "UI/Dev/AkbalRhythmHarnessWidget.h"
#include "UI/Ritual/AkbalRitualInputHud.h"
#include "UI/Ritual/AkbalRitualInputSession.h"
#include "UI/Ritual/AkbalRitualViewportLayout.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace AkbalRitualUiTest
{
	static UWorld* FindAutomationWorld()
	{
		if (!GEngine)
		{
			return nullptr;
		}

		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (UWorld* World = Context.World())
			{
				if (World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::PIE)
				{
					return World;
				}
			}
		}

		return nullptr;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRitualViewportLayoutTest,
	"Akbal.UI.ViewportLayout",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAkbalRitualViewportLayoutTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Square HUD size uses smaller viewport dimension"), 216.f,
		FAkbalRitualViewportLayout::ComputeSquareHudSize(1920, 1080, 0.2f));
	TestEqual(TEXT("Fallback center"), FVector2D(960.f, 540.f),
		FAkbalRitualViewportLayout::ComputeFallbackScreenCenter(1920, 1080));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRitualHudConfigDefaultsTest,
	"Akbal.UI.RitualHudConfigDefaults",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAkbalRitualHudConfigDefaultsTest::RunTest(const FString& Parameters)
{
	const FAkbalRitualInputHudConfig Config;
	TestEqual(TEXT("Default BPM"), 80.f, Config.BeatsPerMinute);
	TestEqual(TEXT("Default preview seconds"), 1.f, Config.NotePreviewSeconds);
	TestEqual(TEXT("Default approach seconds"), 1.f, Config.NoteApproachSeconds);
	TestEqual(TEXT("Default HUD fraction"), 0.28f, Config.MaxViewportSizeFraction);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalHarnessWidgetBuildTest,
	"Akbal.UI.Harness.BuildsWidgetTree",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAkbalHarnessWidgetBuildTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRitualUiTest;

	UWorld* World = FindAutomationWorld();
	if (!TestNotNull(TEXT("Automation world available"), World))
	{
		return false;
	}

	UAkbalRhythmHarnessWidget* Harness = CreateWidget<UAkbalRhythmHarnessWidget>(
		World, UAkbalRhythmHarnessWidget::StaticClass());
	if (!TestNotNull(TEXT("Harness widget created"), Harness))
	{
		return false;
	}

	UAkbalRitualInputSession* Session = NewObject<UAkbalRitualInputSession>(Harness);
	Harness->BindHarness(Session, nullptr, nullptr);

	TestNotNull(TEXT("Harness widget tree exists"), Harness->WidgetTree.Get());
	TestNotNull(TEXT("Harness root widget built"), Harness->WidgetTree ? Harness->WidgetTree->RootWidget.Get() : nullptr);

	if (const TSharedPtr<SWidget> SlateWidget = Harness->TakeWidget())
	{
		SlateWidget->SlatePrepass(1.f);
		const FVector2D DesiredSize = SlateWidget->GetDesiredSize();
		TestTrue(TEXT("Harness reports non-zero desired width"), DesiredSize.X > 1.f);
		TestTrue(TEXT("Harness reports non-zero desired height"), DesiredSize.Y > 1.f);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAkbalRitualHudWidgetBuildTest,
	"Akbal.UI.RitualHud.CreatesAndBinds",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAkbalRitualHudWidgetBuildTest::RunTest(const FString& Parameters)
{
	using namespace AkbalRitualUiTest;

	UWorld* World = FindAutomationWorld();
	if (!TestNotNull(TEXT("Automation world available"), World))
	{
		return false;
	}

	UAkbalRitualInputSession* Session = NewObject<UAkbalRitualInputSession>(World);
	Session->Initialize(nullptr, FAkbalRitualInputHudConfig());

	UAkbalRitualInputHud* Hud = CreateWidget<UAkbalRitualInputHud>(World, UAkbalRitualInputHud::StaticClass());
	if (!TestNotNull(TEXT("Ritual HUD created"), Hud))
	{
		return false;
	}

	Hud->BindSession(Session, nullptr);
	TestTrue(TEXT("Ritual HUD has chart notes"), Session->GetChartStates().Num() > 0);
	const TSharedRef<SWidget> SlateWidget = Hud->TakeWidget();
	SlateWidget->SlatePrepass(1.f);
	TestTrue(TEXT("Ritual HUD slate desired width"), SlateWidget->GetDesiredSize().X >= 0.f);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
