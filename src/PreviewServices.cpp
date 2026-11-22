#include "LauncherServices.h"

namespace nexora {

std::vector<Instance> PreviewServices::listInstances() const {
    return {
        {"survival", "Survival", "1.21.1", "Fabric"},
        {"vanilla", "Vanilla", "1.20.6", "Vanilla"},
        {"workshop", "Workshop", "1.20.1", "Forge"},
    };
}

std::vector<GameVersion> PreviewServices::listGameVersions() const {
    return {
        {"1.21.1", "Minecraft 1.21.1", "Release"},
        {"1.21", "Minecraft 1.21", "Release"},
        {"1.20.6", "Minecraft 1.20.6", "Release"},
        {"1.20.1", "Minecraft 1.20.1", "Release"},
        {"24w21a", "Minecraft 24w21a", "Snapshot"},
        {"24w14a", "Minecraft 24w14a", "Snapshot"},
    };
}

std::vector<Mod> PreviewServices::listMods() const {
    return {
        {"sodium", "Sodium", "Improves rendering performance.", {"Fabric"}},
        {"iris", "Iris Shaders", "Adds support for shader packs.", {"Fabric"}},
        {"lithium", "Lithium", "Optimizes game logic and simulation.", {"Fabric"}},
        {"jei", "Just Enough Items", "Shows item recipes and uses in game.", {"Forge"}},
        {"ferritecore", "FerriteCore", "Reduces memory usage.", {"Fabric", "Forge"}},
        {"journeymap", "JourneyMap", "Adds a map and minimap.", {"Fabric", "Forge"}},
    };
}

ActionResult PreviewServices::requestLaunch(std::string_view instanceId) {
    return {false, "Preview only. Launch is unavailable for " + std::string(instanceId) + "."};
}

ActionResult PreviewServices::requestGameDownload(std::string_view versionId) {
    return {false, "Preview only. No game files were downloaded for " + std::string(versionId) + "."};
}

ActionResult PreviewServices::requestModDownload(std::string_view modId, std::string_view instanceId) {
    return {false, "Preview only. " + std::string(modId) + " was not downloaded for " + std::string(instanceId) + "."};
}

} // namespace nexora
