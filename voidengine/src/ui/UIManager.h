#pragma once

#include "UIComponent.h"
#include "Panel.h"
#include "Button.h"
#include "Text.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace voidengine {

namespace window {
    class Window;
}

namespace ui {

class UIManager {
public:
    explicit UIManager(window::Window* window);
    ~UIManager();
    
    void initialize();
    void update(float deltaTime);
    void render();
    
    std::shared_ptr<Panel> createPanel(const std::string& id, const glm::vec2& position, const glm::vec2& size,
                                       const glm::vec4& backgroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.8f));
    
    std::shared_ptr<Button> createButton(const std::string& id, const glm::vec2& position, const glm::vec2& size,
                                         const std::string& text = "", 
                                         const Button::ButtonCallback& onClick = nullptr);
    
    std::shared_ptr<Text> createText(const std::string& id, const glm::vec2& position, const std::string& text = "",
                                     float fontSize = 12.0f, 
                                     const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    void addComponent(std::shared_ptr<UIComponent> component);
    void removeComponent(const std::string& id);
    std::shared_ptr<UIComponent> getComponent(const std::string& id);
    
    void onMouseMove(double x, double y);
    void onMouseButton(int button, int action, int mods, double x, double y);
    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int codepoint);
    
    void setScreenSize(int width, int height);
    
private:
    window::Window* window_;
    std::vector<std::shared_ptr<UIComponent>> rootComponents_;
    std::unordered_map<std::string, std::shared_ptr<UIComponent>> componentsById_;
    
    int screenWidth_;
    int screenHeight_;
    glm::vec2 lastMousePos_;
    
    std::shared_ptr<UIComponent> findComponentAt(const glm::vec2& point);
};

} // namespace ui
} // namespace voidengine 