// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KingOfTheGun : ModuleRules
{
	public KingOfTheGun(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EngineSettings","InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore","GameplayTags","GameplayTasks", "Niagara" });
	}
}
