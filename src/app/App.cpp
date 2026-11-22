#include "App.hpp"

#include <RmlUi/Core.h>
#include <RmlUi_Backend.h>

#include <SDL3/SDL.h>

#include <atomic>

namespace {

struct WindowEvents {
    SDL_WindowID window_id = 0;
    std::atomic_bool close_requested = false;
};

bool SDLCALL ObserveWindowEvents(void* user_data, SDL_Event* event) {
    auto& state = *static_cast<WindowEvents*>(user_data);
    if (event->type == SDL_EVENT_QUIT ||
        event->type == SDL_EVENT_TERMINATING ||
        (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
            event->window.windowID == state.window_id)) {
        state.close_requested.store(true);
    }

    return true;
}

} // namespace

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

    WindowEvents window_events;
    bool event_watch_registered = false;
    const auto shutdown = [&]() {
        if (event_watch_registered) {
            SDL_RemoveEventWatch(ObserveWindowEvents, &window_events);
        }
        Rml::Shutdown();
        Backend::Shutdown();
    };

    int window_count = 0;
    SDL_Window** windows = SDL_GetWindows(&window_count);
    if (windows == nullptr || window_count != 1) {
        SDL_free(windows);
        shutdown();
        return 1;
    }
    SDL_Window* window = windows[0];
    SDL_free(windows);

    if (!SDL_SetWindowSize(window, window_width, window_height) ||
        !SDL_SetWindowMinimumSize(window, 800, 500)) {
        shutdown();
        return 1;
    }

    int pixel_width = 0;
    int pixel_height = 0;
    if (!SDL_GetWindowSizeInPixels(window, &pixel_width, &pixel_height)) {
        shutdown();
        return 1;
    }

    Rml::Context* context = Rml::CreateContext("main",
        {pixel_width, pixel_height}
    );
    if (context == nullptr) {
        shutdown();
        return 1;
    }
    const float display_scale = SDL_GetWindowDisplayScale(window);
    context->SetDensityIndependentPixelRatio(
        display_scale > 0.f ? display_scale : 1.f
    );

    window_events.window_id = SDL_GetWindowID(window);
    if (!SDL_AddEventWatch(ObserveWindowEvents, &window_events)) {
        shutdown();
        return 1;
    }
    event_watch_registered = true;

    bool running = true;
    while (running) {
        running = Backend::ProcessEvents(context, nullptr, true);
        if (!running || window_events.close_requested.load()) {
            break;
        }
        context->Update();
        Backend::BeginFrame();
        context->Render();
        Backend::PresentFrame();
    }

    shutdown();
    return 0;
}

} // namespace Nexora
