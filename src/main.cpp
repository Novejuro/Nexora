#include "LauncherUi.h"

#include <RmlUi/Core.h>
#include <RmlUi_Backend.h>

#include <cstdio>
#include <filesystem>
#include <string_view>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace {

std::filesystem::path executablePath() {
#ifdef _WIN32
    std::wstring path(32768, L'\0');
    const auto length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
    if (!length || length >= path.size()) {
        return {};
    }
    path.resize(length);
    return path;
#else
    std::error_code error;
    return std::filesystem::read_symlink("/proc/self/exe", error);
#endif
}

class AssetFiles final : public Rml::FileInterface {
public:
    explicit AssetFiles(std::filesystem::path root) : root_(std::move(root)) {}

    Rml::FileHandle Open(const Rml::String& name) override {
        const auto relative = std::filesystem::path(name);
        if (!relative.has_filename() || relative.has_parent_path() || relative.has_root_path() ||
            relative == "." || relative == "..") {
            return 0;
        }
        const auto path = root_ / relative;
#ifdef _WIN32
        return reinterpret_cast<Rml::FileHandle>(_wfopen(path.c_str(), L"rb"));
#else
        return reinterpret_cast<Rml::FileHandle>(std::fopen(path.c_str(), "rb"));
#endif
    }

    void Close(Rml::FileHandle file) override { std::fclose(stream(file)); }
    size_t Read(void* buffer, size_t size, Rml::FileHandle file) override {
        return std::fread(buffer, 1, size, stream(file));
    }
    bool Seek(Rml::FileHandle file, long offset, int origin) override {
        return std::fseek(stream(file), offset, origin) == 0;
    }
    size_t Tell(Rml::FileHandle file) override { return static_cast<size_t>(std::ftell(stream(file))); }

private:
    static std::FILE* stream(Rml::FileHandle file) { return reinterpret_cast<std::FILE*>(file); }

    std::filesystem::path root_;
};

int showError(const char* message, bool smokeTest) {
    std::fprintf(stderr, "%s\n", message);
#ifdef _WIN32
    if (!smokeTest) {
        MessageBoxA(nullptr, message, "Nexora Launcher", MB_OK | MB_ICONERROR);
    }
#else
    static_cast<void>(smokeTest);
#endif
    return 1;
}

bool checkUi(Rml::Context& context, Rml::ElementDocument& document);

int runLauncher(bool smokeTest) {
    const Rml::Vector2i size{1080, 750};
    const auto executable = executablePath();
    if (executable.empty()) {
        return showError("Could not locate the Nexora executable.", smokeTest);
    }
    AssetFiles files(executable.parent_path() / "assets");
    if (!Backend::Initialize("Nexora Launcher", size.x, size.y, true)) {
        return showError("Could not create the Nexora window. Check your display and OpenGL support.", smokeTest);
    }

    Rml::SetFileInterface(&files);
    Rml::SetSystemInterface(Backend::GetSystemInterface());
    Rml::SetRenderInterface(Backend::GetRenderInterface());
    if (!Rml::Initialise()) {
        Backend::Shutdown();
        return showError("Could not initialize the Nexora renderer.", smokeTest);
    }

    auto* context = Rml::CreateContext("nexora", size);
    const bool fontsLoaded = Rml::LoadFontFace("LatoLatin-Regular.ttf") &&
        Rml::LoadFontFace("LatoLatin-Bold.ttf");
    auto* document = fontsLoaded && context ? context->LoadDocument("launcher.rml") : nullptr;
    const char* error = nullptr;
    if (document) {
        nexora::PreviewServices services;
        nexora::LauncherUi ui(*document, services);
        document->Show();
        if (smokeTest) {
            if (!Backend::ProcessEvents(context) || !checkUi(*context, *document)) {
                error = "Nexora UI smoke test failed.";
            }
        } else {
            while (Backend::ProcessEvents(context, nullptr, true)) {
                context->Update();
                Backend::BeginFrame();
                context->Render();
                Backend::PresentFrame();
            }
        }
    } else {
        error = "Could not load the Nexora interface. Keep the assets folder next to the executable.";
    }

    Rml::Shutdown();
    Backend::Shutdown();
    return error ? showError(error, smokeTest) : 0;
}

bool checkUi(Rml::Context& context, Rml::ElementDocument& document) {
    const auto element = [&](const char* id) { return document.GetElementById(id); };
    const auto click = [](Rml::Element* button) { button->DispatchEvent(Rml::EventId::Click, {}); };
    const auto setValue = [&](const char* id, const char* value) {
        auto* control = static_cast<Rml::ElementFormControl*>(element(id));
        control->SetValue(value);
        control->DispatchEvent(Rml::EventId::Change, {});
    };
    const auto render = [&] {
        context.Update();
        Backend::BeginFrame();
        const bool success = context.Render();
        Backend::PresentFrame();
        return success;
    };
    const auto statusHas = [&](const char* text) {
        return element("status")->GetInnerRML().contains(text);
    };
    const auto rowAction = [&](const char* list, int row = 0) {
        return element(list)->GetChild(row)->GetLastChild();
    };
    const auto hasText = [](const Rml::Element* control, const char* label) {
        if (control && control->GetFontFaceHandle() && !control->GetInnerRML().empty()) {
            return true;
        }
        std::fprintf(stderr, "Missing text or font for %s.\n", label);
        return false;
    };
    const auto selectHasText = [&](const char* id) {
        const auto* select = element(id);
        // RmlUi adds the displayed value as an internal child of the select.
        for (int i = 0; i < select->GetNumChildren(true); ++i) {
            const auto* child = select->GetChild(i);
            if (child->GetTagName() == "selectvalue") {
                return hasText(child, id);
            }
        }
        return hasText(nullptr, id);
    };
    const auto pageFillsWorkspace = [&](const char* pageId, const char* listId) {
        auto* page = element(pageId);
        const auto pageWidth = page->GetBox().GetSize(Rml::BoxArea::Border).x;
        const auto horizontalPadding = page->GetBox().GetSize(Rml::BoxArea::Padding).x -
            page->GetBox().GetSize(Rml::BoxArea::Content).x;
        const auto contentWidth = page->GetClientWidth() - horizontalPadding;
        const auto listWidth = element(listId)->GetBox().GetSize(Rml::BoxArea::Border).x;
        auto* row = element(listId)->GetFirstChild();
        const auto rowWidth = row ? row->GetBox().GetSize(Rml::BoxArea::Border).x : 0;
        const auto workspaceWidth = element("status")->GetBox().GetSize(Rml::BoxArea::Border).x;
        if (pageWidth >= workspaceWidth - 2 && listWidth >= contentWidth - 2 &&
            rowWidth >= listWidth - 2 && listWidth >= 200) {
            return true;
        }
        std::fprintf(stderr, "%s layout collapsed: workspace %.0f, page %.0f, content %.0f, list %.0f, row %.0f.\n",
            pageId, workspaceWidth, pageWidth, contentWidth, listWidth, rowWidth);
        return false;
    };

    if (!render() || !hasText(element("breadcrumb"), "breadcrumb") ||
        !hasText(element("nav-play"), "Play tab") || !hasText(element("nav-games"), "Games tab") ||
        !hasText(element("nav-mods"), "Mods tab") || !hasText(element("launch-button"), "Launch button") ||
        !selectHasText("play-instance") || element("instance-list")->GetNumChildren() != 3 ||
        !hasText(rowAction("instance-list"), "Select button") ||
        element("play-hero")->GetBox().GetSize(Rml::BoxArea::Content).y < 150 ||
        !pageFillsWorkspace("page-play", "instance-list")) {
        std::fprintf(stderr, "Play page has missing text, a collapsed banner, or missing version rows.\n");
        return false;
    }
    click(rowAction("instance-list", 1));
    click(element("launch-button"));
    if (!element("instance-list")->GetChild(1)->IsClassSet("selected") || !statusHas("vanilla")) {
        return false;
    }

    click(element("nav-games"));
    if (!render() || !pageFillsWorkspace("page-games", "game-list")) {
        return false;
    }
    setValue("game-channel", "Snapshot");
    setValue("game-search", "24w21a");
    if (element("page-games")->IsClassSet("hidden") || element("game-list")->GetNumChildren() != 1 ||
        !render() || !pageFillsWorkspace("page-games", "game-list") ||
        !selectHasText("game-channel") || !hasText(rowAction("game-list"), "Game download button")) {
        std::fprintf(stderr, "Game downloads page did not render the filtered results and controls.\n");
        return false;
    }
    click(rowAction("game-list"));
    if (!statusHas("24w21a")) {
        return false;
    }

    click(element("nav-mods"));
    if (!render() || !pageFillsWorkspace("page-mods", "mod-list")) {
        return false;
    }
    setValue("mod-instance", "workshop");
    setValue("mod-search", "recipes");
    if (element("page-mods")->IsClassSet("hidden") || element("mod-list")->GetNumChildren() != 1 ||
        !render() || !pageFillsWorkspace("page-mods", "mod-list") ||
        !selectHasText("mod-instance") || !hasText(rowAction("mod-list"), "Mod download button")) {
        std::fprintf(stderr, "Mod downloads page did not render the filtered results and controls.\n");
        return false;
    }
    click(rowAction("mod-list"));
    if (!statusHas("jei") || !statusHas("workshop")) {
        return false;
    }
    setValue("mod-instance", "vanilla");
    if (!element("mod-list")->GetInnerRML().contains("Select an instance with Fabric or Forge")) {
        return false;
    }

    context.SetDensityIndependentPixelRatio(1.25f);
    context.SetDimensions({750, 650});
    click(element("nav-play"));
    if (!render() || !pageFillsWorkspace("page-play", "instance-list")) {
        return false;
    }
    click(element("nav-games"));
    if (!render() || !pageFillsWorkspace("page-games", "game-list")) {
        return false;
    }
    click(element("nav-mods"));
    return render() && pageFillsWorkspace("page-mods", "mod-list");
}

} // namespace

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR arguments, int) {
    return runLauncher(std::string_view(arguments) == "--smoke-test");
}
#else
int main(int argc, char* argv[]) {
    return runLauncher(argc == 2 && std::string_view(argv[1]) == "--smoke-test");
}
#endif
