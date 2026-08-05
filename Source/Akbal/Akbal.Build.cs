// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Akbal : ModuleRules
{
	public Akbal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AudioMixer"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"MetasoundEngine"
		});
	}
}
