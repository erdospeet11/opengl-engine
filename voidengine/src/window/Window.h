#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <memory>

namespace voidengine {

namespace ui {
class UIManager;
}

namespace input {
class InputSystem;
}

namespace window {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();
    
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    const std::string& getTitle() const { return title_; }
    GLFWwindow* getNativeWindow() const { return window_; }
    
    ui::UIManager* getUIManager() const { return uiManager_.get(); }
    input::InputSystem* getInputSystem() const;
    
private:
    void setupCallbacks();
    
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    
    GLFWwindow* window_;
    int width_;
    int height_;
    std::string title_;
    std::unique_ptr<ui::UIManager> uiManager_;
    
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    
    void setupCallbacks();
}; 

} // namespace window
} // namespace voidengine 