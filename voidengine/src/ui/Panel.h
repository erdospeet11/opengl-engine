#pragma once

#include "UIComponent.h"
#include <vector>
#include <glm/glm.hpp>

namespace voidengine {
namespace ui {

class Panel : public UIComponent {
public:
    Panel(const std::string& id, const glm::vec2& position, const glm::vec2& size, 
          const glm::vec4& backgroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.8f),
          bool hasBorder = true, const glm::vec4& borderColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    
    virtual ~Panel() = default;
    
    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    
    void addComponent(std::shared_ptr<UIComponent> component);
    void removeComponent(const std::string& componentId);
    std::shared_ptr<UIComponent> getComponent(const std::string& componentId);
    
    void setBackgroundColor(const glm::vec4& color) { backgroundColor_ = color; }
    const glm::vec4& getBackgroundColor() const { return backgroundColor_; }
    
    void setBorderEnabled(bool enabled) { hasBorder_ = enabled; }
    bool isBorderEnabled() const { return hasBorder_; }
    
    void setBorderColor(const glm::vec4& color) { borderColor_ = color; }
    const glm::vec4& getBorderColor() const { return borderColor_; }
    
private:
    std::vector<std::shared_ptr<UIComponent>> children_;
    glm::vec4 backgroundColor_;
    bool hasBorder_;
    glm::vec4 borderColor_;
    float borderWidth_ = 1.0f;
};

} // namespace ui
} // namespace voidengine 