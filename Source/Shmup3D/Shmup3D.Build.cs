// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Shmup3D : ModuleRules
{
	public Shmup3D(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] {
        });
        
		PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Slate",
            "ShiftyPlugin"
        });
	}
}
