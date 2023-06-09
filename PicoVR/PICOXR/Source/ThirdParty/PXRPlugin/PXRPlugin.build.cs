//Unreal® Engine, Copyright 1998 – 2023, Epic Games, Inc. All rights reserved.

using UnrealBuildTool;
using System.IO;

public class PXRPlugin : ModuleRules
{
	public PXRPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "PXRPlugin/Include"));

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "PXRPlugin/Lib/armeabi-v7a/libpxr_api.so"));
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "PXRPlugin/Lib/arm64-v8a/libpxr_api.so"));
		}
	}
}
