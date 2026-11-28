#include "App.hpp"

#include "MainWindow.h"

namespace Nexora {

int App::run() {
    MainWindow::create() -> run();
    return 0;
}

} // namespace Nexora
