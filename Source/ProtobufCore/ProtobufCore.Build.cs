using System.IO;
using UnrealBuildTool;

public class ProtobufCore : ModuleRules
{
    public ProtobufCore(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Lib", "Win64", "libprotobuf.lib"));

        PublicDefinitions.Add("GOOGLE_PROTOBUF_INTERNAL_DONATE_STEAL_INLINE=1");
    }
}
