#pragma once

#include "UIComponent.h"
#include <functional>
#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace voidengine {
namespace ui {

class Text;

enum class ButtonState {
    NORMAL,
    HOVER,
    PRESSED,
    DISABLED
};

class Button : public UIComponent {
public:
    using ButtonCallback = std::function<void()>;
    
    Button(const std::string& id, const glm::vec2& position, const glm::vec2& size, 
           const std::string& text = "", 
           const ButtonCallback& onClick = nullptr);
           
    virtual ~Button() = default;
    
    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    
    void setText(const std::string& text);
    const std::string& getText() const;
    
    void setOnClick(const ButtonCallback& callback) { onClick_ = callback; }
    
    void setStateColor(ButtonState state, const glm::vec4& color);
    const glm::vec4& getStateColor(ButtonState state) const;
    
    void setTextColor(const glm::vec4& color) { textColor_ = color; }
    const glm::vec4& getTextColor() const { return textColor_; }
    
    ButtonState getState() const { return state_; }
    void setState(ButtonState state) { state_ = state; }
    
    bool isPointInside(const glm::vec2& point) const;
    void onMouseMove(const glm::vec2& point);
    void onMouseButton(int button, int action, const glm::vec2& point);
    
private:
    std::string text_;
    ButtonCallback onClick_;
    ButtonState state_ = ButtonState::NORMAL;
    
    glm::vec4 normalColor_ = glm::vec4(0.3f, 0.3f, 0.8f, 1.0f);
    glm::vec4 hoverColor_ = glm::vec4(0.4f, 0.4f, 0.9f, 1.0f);
    glm::vec4 pressedColor_ = glm::vec4(0.2f, 0.2f, 0.7f, 1.0f);
    glm::vec4 disabledColor_ = glm::vec4(0.5f, 0.5f, 0.5f, 0.7f);
    
    glm::vec4 textColor_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool isMouseOver_ = false;
    bool isMousePressed_ = false;
    
    std::shared_ptr<Text> textComponent_;
};

} // namespace ui
} // namespace voidengine 