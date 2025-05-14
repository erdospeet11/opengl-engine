#include "window/Window.h"
#include <iostream>

int main() {
    try {
        voidengine::window::Window window(800, 600, "VoidEngine Demo");

        while (!window.shouldClose()) {
            window.clear(0.1f, 0.1f, 0.2f, 1.0f);

            //swap buffers and poll events
            window.swapBuffers();
            window.pollEvents();
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 