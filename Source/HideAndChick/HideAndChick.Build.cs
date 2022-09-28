// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HideAndChick : ModuleRules
{
	public HideAndChick(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemSteam", "OnlineSubsystemEOS", "OnlineSubsystemEOSPlus", "VoiceChat", "EOSVoiceChat" });

        bEnableUndefinedIdentifierWarnings = false;


    }

    //string EngineSourceDirectory = Path.GetFullPath(Target.RelativeEnginePath);
    //PrivateIncludePaths.Add(Path.Combine(EngineSourceDirectory, "Plugins\Online\OnlineSubsystemEOS\Source\OnlineSubsystemEOS\Private"));

    
}


