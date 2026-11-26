#include "MessageBox.hpp"
#include <SDL3/SDL.h>

namespace Nexora::Core {

void MessageBox::Show(
    const char* title,
    const char* message,
    Type type
) {
    SDL_MessageBoxFlags flags = SDL_MESSAGEBOX_ERROR;

    switch(type) {
        case Type::Error: {
            flags = SDL_MESSAGEBOX_ERROR;
            break;
        }
        case Type::Warning: {
            flags = SDL_MESSAGEBOX_WARNING;
            break;
        }
        case Type::Info: {
            flags = SDL_MESSAGEBOX_INFORMATION;
            break;
        }
    }

    const bool shown = SDL_ShowSimpleMessageBox(
        flags,
        title,
        message,
        nullptr
    );

    if (!shown) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "SDL_ShowSimpleMessageBox failed: %s",
            SDL_GetError()
        );
    }
}

void MessageBox::ShowConfirm(
    const char* title,
    const char* message
) {
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "OK" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "CLEAN" }
    };

    const SDL_MessageBoxData MessageBoxData = {
        SDL_MESSAGEBOX_INFORMATION,
        nullptr,
        title,
        message,
        SDL_arraysize(buttons),
        buttons,
        nullptr
    };

    int ButtonID = 0;

    SDL_ShowMessageBox(&MessageBoxData, &ButtonID);
}

} // namespace Nexora::Core