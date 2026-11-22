#pragma once

#include "LauncherServices.h"

#include <RmlUi/Core/EventListener.h>

namespace Rml {
class Element;
class ElementDocument;
class ElementFormControlSelect;
}

namespace nexora {

class LauncherUi final : public Rml::EventListener {
public:
    LauncherUi(Rml::ElementDocument& document, LauncherServices& services);
    ~LauncherUi() override;
    void ProcessEvent(Rml::Event& event) override;

private:
    Rml::Element& element(const char* id) const;
    std::string value(const char* id) const;
    const Instance* selectedInstance(const char* id) const;
    void showPage(const std::string& page);
    void showStatus(const ActionResult& result);
    void renderInstances();
    void renderGames();
    void renderMods();

    Rml::ElementDocument& document_;
    LauncherServices& services_;
    std::vector<Instance> instances_;
    std::vector<GameVersion> gameVersions_;
    std::vector<Mod> mods_;
};

} // namespace nexora
