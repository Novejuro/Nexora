#include "App.hpp"

#include <RmlUi/Core.h>
#include <RmlUi_Backend.h>

namespace Nexora {

int App::run() {
    constexpr int window_width = 1280;
    constexpr int window_height = 720;

    if (!Backend::Initialize("Nexora",
        window_width, window_height, true
    )) {
        return 1;
    }

    Rml::SetSystemInterface(Backend::GetSystemInterface());
    Rml::SetRenderInterface(Backend::GetRenderInterface());

    if (!Rml::Initialise()) {
        Backend::Shutdown();
        return 1;
    }

    Rml::Context* context = Rml::CreateContext("main",
        {
            window_width, window_height
        }
    );
    if (context == nullptr) {
        Rml::Shutdown();
        Backend::Shutdown();
        return 1;
    }

    bool running = true;
    while (running) {
        running = Backend::ProcessEvents(context, nullptr, true);
        context->Update();
        Backend::BeginFrame();
        context->Render();
        Backend::PresentFrame();
    }

    Rml::Shutdown();
    Backend::Shutdown();
    return 0;
}

} // namespace Nexora
