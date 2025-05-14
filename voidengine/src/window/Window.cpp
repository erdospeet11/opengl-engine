#include "Window.h"
#include "../ui/UIManager.h"
#include "../ui/FontRenderer.h"
#include "../input/Input.h"
#include <stdexcept>
#include <filesystem>
#include <iostream>

namespace voidengine {
namespace window {

Window::Window(int width, int height, const std::string& title)
    : width_(width), height_(height), title_(title) {
    
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window_);
    
    initializeInputSystem(window_);
    
    initializeInputMapping();
   
    if (!ui::initializeFontSystem()) {
        throw std::runtime_error("Failed to initialize font system");
    }
    
    bool fontLoaded = false;
    
    std::vector<std::string> fontPaths = {
        std::filesystem::current_path().string() + "/src/fonts/BlockCraft.otf",
        "../src/fonts/BlockCraft.otf",
        "src/fonts/BlockCraft.otf"
    };
    
    for (const auto& path : fontPaths) {
        try {
            if (std::filesystem::exists(path)) {
                std::cout << "Loading font from: " << path << std::endl;
                if (ui::gFontRenderer->loadFont(path, 32)) {
                    fontLoaded = true;
                    break;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error checking font path " << path << ": " << e.what() << std::endl;
        }
    }
    
    if (!fontLoaded) {
        std::cerr << "Warning: Failed to load default font. Using fallback rendering." << std::endl;
    }
    
    uiManager_ = std::make_unique<ui::UIManager>(this);
    
    setupCallbacks();
}

Window::~Window() {
    uiManager_.reset();

    ui::shutdownFontSystem();
    
    shutdownInputSystem();
    
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Window::swapBuffers() {
    if (uiManager_) {
        uiManager_->render();
    }
    
    glfwSwapBuffers(window_);
}

void Window::pollEvents() {
    if (uiManager_) {
        uiManager_->update(0.016f);
    }
    
    glfwPollEvents();
    
    if (gInputSystem) {
        gInputSystem->update();
    }
    
    if (gInputMapping) {
        gInputMapping->update();
    }
    
    if (uiManager_) {
        uiManager_->update(0.016f);
    }
}

void Window::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

input::InputSystem* Window::getInputSystem() const {
    return gInputSystem.get();

void Window::setupCallbacks() {
    glfwSetWindowUserPointer(window_, this);
    
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
    
    glfwSetCursorPosCallback(window_, mouseMoveCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetCharCallback(window_, charCallback);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowPtr) {
        windowPtr->width_ = width;
        windowPtr->height_ = height;
        
        glViewport(0, 0, width, height);
        
        if (windowPtr->uiManager_) {
            windowPtr->uiManager_->setScreenSize(width, height);
        }
    }
}

void Window::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowPtr && windowPtr->uiManager_) {
        windowPtr->uiManager_->onMouseMove(xpos, ypos);
    }
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowPtr && windowPtr->uiManager_) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        windowPtr->uiManager_->onMouseButton(button, action, mods, xpos, ypos);
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowPtr && windowPtr->uiManager_) {
        windowPtr->uiManager_->onKey(key, scancode, action, mods);
    }
}

void Window::charCallback(GLFWwindow* window, unsigned int codepoint) {
    Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowPtr && windowPtr->uiManager_) {
        windowPtr->uiManager_->onChar(codepoint);
    }
}

} // namespace window
} // namespace voidengine 