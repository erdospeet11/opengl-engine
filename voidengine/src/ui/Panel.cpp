#include "Panel.h"
#include <algorithm>
#include <stdexcept>
#include <GLFW/glfw3.h>

namespace voidengine {
namespace ui {

Panel::Panel(const std::string& id, const glm::vec2& position, const glm::vec2& size,
             const glm::vec4& backgroundColor, bool hasBorder, const glm::vec4& borderColor)
    : UIComponent(id, position, size),
      backgroundColor_(backgroundColor),
      hasBorder_(hasBorder),
      borderColor_(borderColor) {
}

void Panel::initialize() {
    for (auto& child : children_) {
        child->initialize();
    }
}

void Panel::update(float deltaTime) {
    for (auto& child : children_) {
        child->update(deltaTime);
    }
}

void Panel::render() {
    if (!isVisible_) {
        return;
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(backgroundColor_.r, backgroundColor_.g, backgroundColor_.b, backgroundColor_.a);
    glBegin(GL_QUADS);
    glVertex2f(position_.x, position_.y);
    glVertex2f(position_.x + size_.x, position_.y);
    glVertex2f(position_.x + size_.x, position_.y + size_.y);
    glVertex2f(position_.x, position_.y + size_.y);
    glEnd();
    
    if (hasBorder_) {
        glColor4f(borderColor_.r, borderColor_.g, borderColor_.b, borderColor_.a);
        glLineWidth(borderWidth_);
        glBegin(GL_LINE_LOOP);
        glVertex2f(position_.x, position_.y);
        glVertex2f(position_.x + size_.x, position_.y);
        glVertex2f(position_.x + size_.x, position_.y + size_.y);
        glVertex2f(position_.x, position_.y + size_.y);
        glEnd();
    }
    
    for (auto& child : children_) {
        child->render();
    }
}

void Panel::addComponent(std::shared_ptr<UIComponent> component) {
    if (!component) {
        throw std::invalid_argument("Cannot add null component to panel");
    }
    
    auto it = std::find_if(children_.begin(), children_.end(),
                          [&](const std::shared_ptr<UIComponent>& child) {
                              return child->getId() == component->getId();
                          });
    
    if (it != children_.end()) {
        throw std::invalid_argument("Component with ID " + component->getId() + " already exists in panel");
    }
    
    children_.push_back(component);
}

void Panel::removeComponent(const std::string& componentId) {
    auto it = std::find_if(children_.begin(), children_.end(),
                          [&](const std::shared_ptr<UIComponent>& child) {
                              return child->getId() == componentId;
                          });
    
    if (it != children_.end()) {
        children_.erase(it);
    }
}

std::shared_ptr<UIComponent> Panel::getComponent(const std::string& componentId) {
    auto it = std::find_if(children_.begin(), children_.end(),
                          [&](const std::shared_ptr<UIComponent>& child) {
                              return child->getId() == componentId;
                          });
    
    if (it != children_.end()) {
        return *it;
    }
    
    return nullptr;
}

} // namespace ui
} // namespace voidengine 