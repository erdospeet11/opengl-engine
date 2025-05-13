#include "Window.h"
#include <stdexcept>

namespace voidengine {

Window::Window(int width, int height, const std::string& title)
    : width_(width), height_(height), title_(title) {
    
    //initialize glfw
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    //configure glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    //make window's context current
    glfwMakeContextCurrent(window_);

    //window resize callback
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window_);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

} // namespace voidengine 