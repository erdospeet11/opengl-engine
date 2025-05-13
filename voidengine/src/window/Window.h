#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace voidengine {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void clear(float r, float g, float b, float a);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    GLFWwindow* getNativeWindow() const { return window_; }

private:
    GLFWwindow* window_;
    int width_;
    int height_;
    std::string title_;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
}; 

} // namespace voidengine 