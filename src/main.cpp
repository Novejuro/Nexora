#include "app/App.hpp"

#include <SDL3/SDL_main.h>

auto main(int, char**) -> int {
	Nexora::App app;
	return app.run();
}
