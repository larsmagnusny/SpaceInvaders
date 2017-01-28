// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SpaceInvaders : ModuleRules
{
	public SpaceInvaders(TargetInfo Target)
	{
        // SpaceInvaders.Build.cs er viktig fordi vi laster inn UMG slate og SlateCore
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
