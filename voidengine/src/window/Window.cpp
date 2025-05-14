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
    
    glfwSetWindowUserPointer(window_, this);
    
    if (!gInputSystem) {
        gInputSystem = std::make_unique<input::InputSystem>();
    }
    gInputSystem->initialize(window_);
    
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
    
    // Now set up our callbacks that will handle both UI and input system
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
}

void Window::setupCallbacks() {
    // Set the framebuffer callback
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
    
    // Set up the mouse move callback to work with both UI and InputSystem
    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
        // First, call our UI callback
        Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (windowPtr && windowPtr->uiManager_) {
            windowPtr->uiManager_->onMouseMove(xpos, ypos);
        }
        
        // Manually update the input system with the mouse move event
        if (gInputSystem) {
            // Set the mouse position directly in the InputSystem
            gInputSystem->setMousePosition(glm::vec2(xpos, ypos));
            
            // Create and dispatch the event
            input::InputEvent event;
            event.type = input::EventType::MOUSE_MOVE;
            event.position = glm::vec2(xpos, ypos);
            event.delta = glm::vec2(0, 0); // Delta will be calculated in update
            gInputSystem->processInputEvent(event);
        }
    });
    
    // Set up the mouse button callback to work with both UI and InputSystem
    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods) {
        // First, call our UI callback
        Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (windowPtr && windowPtr->uiManager_) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            windowPtr->uiManager_->onMouseButton(button, action, mods, xpos, ypos);
        }
        
        // Manually update the input system with the mouse button event
        if (gInputSystem) {
            input::ButtonState state;
            switch (action) {
                case GLFW_PRESS:
                    state = input::ButtonState::PRESSED_THIS_FRAME;
                    break;
                case GLFW_RELEASE:
                    state = input::ButtonState::RELEASED_THIS_FRAME;
                    break;
                default:
                    state = input::ButtonState::RELEASED;
                    break;
            }
            
            gInputSystem->updateMouseButtonState(button, state);
            
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            input::InputEvent event;
            event.type = input::EventType::MOUSE_BUTTON;
            event.code = button;
            event.state = state;
            event.position = glm::vec2(xpos, ypos);
            event.mods = mods;
            
            gInputSystem->processInputEvent(event);
        }
    });
    
    // Set up the key callback to work with both UI and InputSystem
    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // First, call our UI callback
        Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (windowPtr && windowPtr->uiManager_) {
            windowPtr->uiManager_->onKey(key, scancode, action, mods);
        }
        
        // Manually update the input system with the key event
        if (gInputSystem) {
            input::ButtonState state;
            switch (action) {
                case GLFW_PRESS:
                    state = input::ButtonState::PRESSED_THIS_FRAME;
                    break;
                case GLFW_RELEASE:
                    state = input::ButtonState::RELEASED_THIS_FRAME;
                    break;
                case GLFW_REPEAT:
                    state = input::ButtonState::HELD;
                    break;
                default:
                    state = input::ButtonState::RELEASED;
                    break;
            }
            
            gInputSystem->updateKeyState(key, state);
            
            input::InputEvent event;
            event.type = input::EventType::KEY;
            event.code = key;
            event.state = state;
            event.mods = mods;
            
            gInputSystem->processInputEvent(event);
        }
    });
    
    // Set up the char callback to work with both UI and InputSystem
    glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int codepoint) {
        // First, call our UI callback
        Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (windowPtr && windowPtr->uiManager_) {
            windowPtr->uiManager_->onChar(codepoint);
        }
        
        // Manually update the input system with the char event
        if (gInputSystem) {
            gInputSystem->appendTextInput(codepoint);
        }
    });
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