#include "UIManager.h"
#include "../window/Window.h"
#include <algorithm>
#include <stdexcept>

namespace voidengine {
namespace ui {

UIManager::UIManager(window::Window* window)
    : window_(window), screenWidth_(800), screenHeight_(600), lastMousePos_(0.0f, 0.0f) {
    
    if (!window_) {
        throw std::invalid_argument("Window cannot be null");
    }
    
    screenWidth_ = window_->getWidth();
    screenHeight_ = window_->getHeight();
}

UIManager::~UIManager() {
    rootComponents_.clear();
    componentsById_.clear();
}

void UIManager::initialize() {
    for (auto& component : rootComponents_) {
        component->initialize();
    }
}

void UIManager::update(float deltaTime) {
    for (auto& component : rootComponents_) {
        component->update(deltaTime);
    }
}

void UIManager::render() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth_, screenHeight_, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    
    for (auto& component : rootComponents_) {
        component->render();
    }
    
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

std::shared_ptr<Panel> UIManager::createPanel(const std::string& id, const glm::vec2& position, 
                                             const glm::vec2& size, const glm::vec4& backgroundColor) {
    if (componentsById_.find(id) != componentsById_.end()) {
        throw std::invalid_argument("Component with ID '" + id + "' already exists");
    }
    
    auto panel = std::make_shared<Panel>(id, position, size, backgroundColor);
    
    rootComponents_.push_back(panel);
    componentsById_[id] = panel;
    
    return panel;
}

std::shared_ptr<Button> UIManager::createButton(const std::string& id, const glm::vec2& position, 
                                               const glm::vec2& size, const std::string& text,
                                               const Button::ButtonCallback& onClick) {
    if (componentsById_.find(id) != componentsById_.end()) {
        throw std::invalid_argument("Component with ID '" + id + "' already exists");
    }
    
    auto button = std::make_shared<Button>(id, position, size, text, onClick);
    
    rootComponents_.push_back(button);
    componentsById_[id] = button;
    
    return button;
}

std::shared_ptr<Text> UIManager::createText(const std::string& id, const glm::vec2& position, 
                                           const std::string& text, float fontSize,
                                           const glm::vec4& color) {
    if (componentsById_.find(id) != componentsById_.end()) {
        throw std::invalid_argument("Component with ID '" + id + "' already exists");
    }
    
    auto textComponent = std::make_shared<Text>(id, position, text, fontSize, color);
    
    rootComponents_.push_back(textComponent);
    componentsById_[id] = textComponent;
    
    return textComponent;
}

void UIManager::addComponent(std::shared_ptr<UIComponent> component) {
    if (!component) {
        throw std::invalid_argument("Cannot add null component");
    }
    
    const std::string& id = component->getId();
    
    if (componentsById_.find(id) != componentsById_.end()) {
        throw std::invalid_argument("Component with ID '" + id + "' already exists");
    }
    
    rootComponents_.push_back(component);
    componentsById_[id] = component;
}

void UIManager::removeComponent(const std::string& id) {
    auto it = componentsById_.find(id);
    if (it != componentsById_.end()) {
        auto component = it->second;
        rootComponents_.erase(std::remove(rootComponents_.begin(), rootComponents_.end(), component),
                             rootComponents_.end());
        
        componentsById_.erase(it);
    }
}

std::shared_ptr<UIComponent> UIManager::getComponent(const std::string& id) {
    auto it = componentsById_.find(id);
    if (it != componentsById_.end()) {
        return it->second;
    }
    return nullptr;
}

void UIManager::onMouseMove(double x, double y) {
    lastMousePos_ = glm::vec2(x, y);
    
    for (auto& component : rootComponents_) {
        if (auto button = std::dynamic_pointer_cast<Button>(component)) {
            button->onMouseMove(lastMousePos_);
        }
    }
}

void UIManager::onMouseButton(int button, int action, int mods, double x, double y) {
    lastMousePos_ = glm::vec2(x, y);
    
    for (auto& component : rootComponents_) {
        if (auto btn = std::dynamic_pointer_cast<Button>(component)) {
            btn->onMouseButton(button, action, lastMousePos_);
        }
    }
}

void UIManager::onKey(int key, int scancode, int action, int mods) {
}

void UIManager::onChar(unsigned int codepoint) {
}

void UIManager::setScreenSize(int width, int height) {
    screenWidth_ = width;
    screenHeight_ = height;
}

std::shared_ptr<UIComponent> UIManager::findComponentAt(const glm::vec2& point) {
    for (auto it = rootComponents_.rbegin(); it != rootComponents_.rend(); ++it) {
        auto component = *it;
        
        if (auto button = std::dynamic_pointer_cast<Button>(component)) {
            if (button->isVisible() && button->isPointInside(point)) {
                return button;
            }
        }
        
    }
    
    return nullptr;
}

} // namespace ui
} // namespace voidengine 