// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GlitchWiz : ModuleRules
{
	public GlitchWiz(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GlitchWiz",
			"GlitchWiz/Variant_Horror",
			"GlitchWiz/Variant_Horror/UI",
			"GlitchWiz/Variant_Shooter",
			"GlitchWiz/Variant_Shooter/AI",
			"GlitchWiz/Variant_Shooter/UI",
			"GlitchWiz/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
