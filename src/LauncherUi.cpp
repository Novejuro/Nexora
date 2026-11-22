#include "LauncherUi.h"

#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Elements/ElementFormControl.h>
#include <RmlUi/Core/Elements/ElementFormControlSelect.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/StringUtilities.h>

#include <algorithm>
#include <cctype>
#include <string_view>

namespace nexora {
namespace {

using Rml::StringUtilities::EncodeRml;

struct Page {
    const char* key;
    const char* label;
    const char* section;
    const char* nav;
};

constexpr Page pages[] = {
    {"play", "Play", "page-play", "nav-play"},
    {"games", "Game downloads", "page-games", "nav-games"},
    {"mods", "Mod downloads", "page-mods", "nav-mods"},
};

std::string lower(std::string text) {
    std::ranges::transform(text, text.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return text;
}

std::string makeRow(const char* tileClass, const char* icon, const std::string& name,
    const std::string& description, const char* action, const std::string& id,
    const char* label, bool selected = false) {
    std::string rml = selected ? "<div class='row selected'>" : "<div class='row'>";
    rml += std::string("<div class='tile ") + tileClass + "'>" + icon + "</div>";
    rml += "<div class='item-copy'><div class='item-name'>" + EncodeRml(name) +
        "</div><div class='muted'>" + EncodeRml(description) + "</div></div>";
    rml += std::string("<button class='row-action' data-action='") + action + "' data-id=\"" +
        EncodeRml(id) + "\">" + label + "</button></div>";
    return rml;
}

void setList(Rml::Element& element, const std::string& rows, const std::string& emptyMessage) {
    element.SetInnerRML(rows.empty() ? "<div class='empty'>" + EncodeRml(emptyMessage) + "</div>" : rows);
}

} // namespace

LauncherUi::LauncherUi(Rml::ElementDocument& document, LauncherServices& services)
    : document_(document), services_(services), instances_(services.listInstances()),
      gameVersions_(services.listGameVersions()), mods_(services.listMods()) {
    for (const char* id : {"play-instance", "mod-instance"}) {
        auto& select = static_cast<Rml::ElementFormControlSelect&>(element(id));
        for (const auto& instance : instances_) {
            select.Add(EncodeRml(instance.name + " / " + instance.gameVersion + " / " + instance.loader), instance.id);
        }
        if (instances_.empty()) {
            select.Add("No instances", "", -1, false);
            select.SetDisabled(true);
        } else {
            select.SetSelection(0);
        }
    }
    if (instances_.empty()) {
        element("launch-button").SetAttribute("disabled", true);
    }
    renderInstances();
    renderGames();
    renderMods();
    document_.AddEventListener(Rml::EventId::Click, this);
    document_.AddEventListener(Rml::EventId::Change, this);
}

LauncherUi::~LauncherUi() {
    document_.RemoveEventListener(Rml::EventId::Click, this);
    document_.RemoveEventListener(Rml::EventId::Change, this);
}

Rml::Element& LauncherUi::element(const char* id) const {
    return *document_.GetElementById(id);
}

std::string LauncherUi::value(const char* id) const {
    return static_cast<Rml::ElementFormControl&>(element(id)).GetValue();
}

const Instance* LauncherUi::selectedInstance(const char* id) const {
    const auto selected = value(id);
    const auto it = std::ranges::find(instances_, selected, &Instance::id);
    return it == instances_.end() ? nullptr : &*it;
}

void LauncherUi::showPage(const std::string& page) {
    for (const auto& entry : pages) {
        const bool active = page == entry.key;
        element(entry.section).SetClass("hidden", !active);
        element(entry.nav).SetClass("active", active);
        if (active) {
            element("breadcrumb").SetInnerRML(entry.label);
        }
    }
    auto& status = element("status");
    status.SetInnerRML("Preview mode. Launch and download actions use sample data.");
    status.SetClass("notice", false);
    status.SetClass("accepted", false);
}

void LauncherUi::showStatus(const ActionResult& result) {
    auto& status = element("status");
    status.SetInnerRML(EncodeRml(result.message));
    status.SetClass("notice", !result.accepted);
    status.SetClass("accepted", result.accepted);
}

void LauncherUi::renderInstances() {
    const auto selected = value("play-instance");
    // Keep the existing controls alive when selection changes so keyboard focus stays in place.
    auto& list = element("instance-list");
    if (list.GetNumChildren() == 0) {
        std::string rows;
        for (const auto& instance : instances_) {
            rows += makeRow("", "I", instance.name, instance.gameVersion + " / " + instance.loader,
                "select", instance.id, "Select", instance.id == selected);
        }
        setList(list, rows, "No instances in the version library.");
    }
    for (int i = 0; i < list.GetNumChildren() && i < static_cast<int>(instances_.size()); ++i) {
        list.GetChild(i)->SetClass("selected", instances_[i].id == selected);
    }
}

void LauncherUi::renderGames() {
    const auto query = lower(value("game-search"));
    const auto channel = value("game-channel");
    std::string rows;
    for (const auto& version : gameVersions_) {
        if ((channel.empty() || channel == version.channel) && lower(version.label).contains(query)) {
            rows += makeRow("game", version.channel == "Snapshot" ? "S" : "J", version.label,
                version.channel, "game", version.id, "Download");
        }
    }
    setList(element("game-list"), rows, "No versions match your search.");
}

void LauncherUi::renderMods() {
    auto& list = element("mod-list");
    const auto* instance = selectedInstance("mod-instance");
    if (!instance || instance->loader == "Vanilla") {
        setList(list, "", "Select an instance with Fabric or Forge to browse sample mods.");
        return;
    }
    const auto query = lower(value("mod-search"));
    std::string rows;
    for (const auto& mod : mods_) {
        if (std::ranges::contains(mod.loaders, instance->loader) &&
            lower(mod.name + " " + mod.description).contains(query)) {
            rows += makeRow("mod", "M", mod.name, mod.description, "mod", mod.id, "Download");
        }
    }
    setList(list, rows, "No mods match your search for " + instance->loader + ".");
}

void LauncherUi::ProcessEvent(Rml::Event& event) {
    auto* target = event.GetTargetElement();
    if (event.GetId() == Rml::EventId::Change) {
        const auto& id = target->GetId();
        if (id == "play-instance") {
            renderInstances();
        } else if (id == "game-search" || id == "game-channel") {
            renderGames();
        } else if (id == "mod-search" || id == "mod-instance") {
            renderMods();
        }
        return;
    }
    for (auto* button = target; button && button != &document_; button = button->GetParentNode()) {
        if (button->GetTagName() != "button" || button->HasAttribute("disabled")) {
            continue;
        }
        const auto page = button->GetAttribute<std::string>("data-page", "");
        const auto action = button->GetAttribute<std::string>("data-action", "");
        const auto id = button->GetAttribute<std::string>("data-id", "");
        if (!page.empty()) {
            showPage(page);
        } else if (action == "select") {
            static_cast<Rml::ElementFormControlSelect&>(element("play-instance")).SetValue(id);
            renderInstances();
        } else if (action == "launch") {
            if (const auto* instance = selectedInstance("play-instance")) {
                showStatus(services_.requestLaunch(instance->id));
            }
        } else if (action == "game") {
            showStatus(services_.requestGameDownload(id));
        } else if (action == "mod") {
            if (const auto* instance = selectedInstance("mod-instance")) {
                showStatus(services_.requestModDownload(id, instance->id));
            }
        }
        return;
    }
}

} // namespace nexora
