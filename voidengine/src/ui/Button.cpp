#include "Button.h"
#include "Text.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

namespace voidengine {
namespace ui {

Button::Button(const std::string& id, const glm::vec2& position, const glm::vec2& size,
               const std::string& text, const ButtonCallback& onClick)
    : UIComponent(id, position, size), text_(text), onClick_(onClick) {
    textComponent_ = std::make_shared<Text>(id + "_text", 
                                           glm::vec2(0, 0),
                                           text,
                                           size.y * 0.5f,
                                           glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    textComponent_->setAlignment(TextAlignment::CENTER);
}

void Button::initialize() {
    textComponent_->initialize();
}

void Button::update(float deltaTime) {
    if (!isEnabled_) {
        state_ = ButtonState::DISABLED;
    } else if (isMousePressed_) {
        state_ = ButtonState::PRESSED;
    } else if (isMouseOver_) {
        state_ = ButtonState::HOVER;
    } else {
        state_ = ButtonState::NORMAL;
    }
    
    textComponent_->update(deltaTime);
}

void Button::render() {
    if (!isVisible_) {
        return;
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    const glm::vec4& color = getStateColor(state_);
    glColor4f(color.r, color.g, color.b, color.a);
    
    glBegin(GL_QUADS);
    glVertex2f(position_.x, position_.y);
    glVertex2f(position_.x + size_.x, position_.y);
    glVertex2f(position_.x + size_.x, position_.y + size_.y);
    glVertex2f(position_.x, position_.y + size_.y);
    glEnd();
    
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(position_.x, position_.y);
    glVertex2f(position_.x + size_.x, position_.y);
    glVertex2f(position_.x + size_.x, position_.y + size_.y);
    glVertex2f(position_.x, position_.y + size_.y);
    glEnd();
    
    float textX = position_.x + (size_.x / 2.0f);
    
    float textHeight = textComponent_->getFontSize();
    float textY = position_.y + (size_.y / 2.0f) - (textHeight / 4.0f);
    
    textComponent_->setPosition(glm::vec2(textX, textY));
    
    if (state_ == ButtonState::DISABLED) {
        textComponent_->setColor(glm::vec4(0.7f, 0.7f, 0.7f, 0.7f));
    } else if (state_ == ButtonState::PRESSED) {
        textComponent_->setColor(glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    } else {
        textComponent_->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    
    textComponent_->render();
}

void Button::setStateColor(ButtonState state, const glm::vec4& color) {
    switch (state) {
        case ButtonState::NORMAL:
            normalColor_ = color;
            break;
        case ButtonState::HOVER:
            hoverColor_ = color;
            break;
        case ButtonState::PRESSED:
            pressedColor_ = color;
            break;
        case ButtonState::DISABLED:
            disabledColor_ = color;
            break;
    }
}

const glm::vec4& Button::getStateColor(ButtonState state) const {
    switch (state) {
        case ButtonState::NORMAL:
            return normalColor_;
        case ButtonState::HOVER:
            return hoverColor_;
        case ButtonState::PRESSED:
            return pressedColor_;
        case ButtonState::DISABLED:
            return disabledColor_;
        default:
            return normalColor_;
    }
}

void Button::setText(const std::string& text) {
    text_ = text;
    if (textComponent_) {
        textComponent_->setText(text);
    }
}

const std::string& Button::getText() const {
    return text_;
}

bool Button::isPointInside(const glm::vec2& point) const {
    return (point.x >= position_.x && point.x <= position_.x + size_.x &&
            point.y >= position_.y && point.y <= position_.y + size_.y);
}

void Button::onMouseMove(const glm::vec2& point) {
    isMouseOver_ = isPointInside(point);
}

void Button::onMouseButton(int button, int action, const glm::vec2& point) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if (isPointInside(point)) {
                isMousePressed_ = true;
            }
        } else if (action == GLFW_RELEASE) {
            if (isMousePressed_ && isPointInside(point) && isEnabled_ && onClick_) {
                onClick_();
            }
            isMousePressed_ = false;
        }
    }
}

} // namespace ui
} // namespace voidengine 