#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace nexora {

struct Instance {
    std::string id;
    std::string name;
    std::string gameVersion;
    std::string loader;
};

struct GameVersion {
    std::string id;
    std::string label;
    std::string channel;
};

struct Mod {
    std::string id;
    std::string name;
    std::string description;
    std::vector<std::string> loaders;
};

struct ActionResult {
    bool accepted;
    std::string message;
};

class LauncherServices {
public:
    virtual ~LauncherServices() = default;

    virtual std::vector<Instance> listInstances() const = 0;
    virtual std::vector<GameVersion> listGameVersions() const = 0;
    virtual std::vector<Mod> listMods() const = 0;

    // Implementations should return promptly. Real operations can queue background work.
    virtual ActionResult requestLaunch(std::string_view instanceId) = 0;
    virtual ActionResult requestGameDownload(std::string_view versionId) = 0;
    virtual ActionResult requestModDownload(std::string_view modId, std::string_view instanceId) = 0;
};

class PreviewServices final : public LauncherServices {
public:
    std::vector<Instance> listInstances() const override;
    std::vector<GameVersion> listGameVersions() const override;
    std::vector<Mod> listMods() const override;
    ActionResult requestLaunch(std::string_view instanceId) override;
    ActionResult requestGameDownload(std::string_view versionId) override;
    ActionResult requestModDownload(std::string_view modId, std::string_view instanceId) override;
};

} // namespace nexora
