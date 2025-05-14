#pragma once

#include "UIComponent.h"
#include <string>
#include <glm/glm.hpp>

namespace voidengine {
namespace ui {

enum class TextAlignment {
    LEFT,
    CENTER,
    RIGHT
};

class Text : public UIComponent {
public:
    Text(const std::string& id, const glm::vec2& position, const std::string& text = "",
         float fontSize = 16.0f, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    virtual ~Text() = default;
    
    void initialize() override;
    void update(float deltaTime) override;
    void render() override;
    
    void setText(const std::string& text);
    const std::string& getText() const { return text_; }
    
    void setColor(const glm::vec4& color) { color_ = color; }
    const glm::vec4& getColor() const { return color_; }
    
    void setFontSize(float fontSize);
    float getFontSize() const { return fontSize_; }
    
    void setAlignment(TextAlignment alignment) { alignment_ = alignment; }
    TextAlignment getAlignment() const { return alignment_; }
    
    void calculateSize();
    
private:
    std::string text_;
    glm::vec4 color_;
    float fontSize_;
    TextAlignment alignment_ = TextAlignment::LEFT;
};

} // namespace ui
} // namespace voidengine 