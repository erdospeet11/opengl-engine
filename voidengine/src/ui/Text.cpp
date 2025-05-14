#include "Text.h"
#include "FontRenderer.h"
#include <cstring>
#include <GLFW/glfw3.h>
#include <iostream>

namespace voidengine {
namespace ui {

Text::Text(const std::string& id, const glm::vec2& position, const std::string& text,
           float fontSize, const glm::vec4& color)
    : UIComponent(id, position, glm::vec2(0.0f)), text_(text), color_(color), fontSize_(fontSize) {
    calculateSize();
}

void Text::initialize() {
}

void Text::update(float deltaTime) {
}

void Text::render() {
    if (!isVisible_ || text_.empty()) {
        return;
    }
    
    if (gFontRenderer) {
        float x = position_.x;
        float y = position_.y;
        
        if (alignment_ == TextAlignment::CENTER) {
            x -= size_.x / 2.0f;
        } else if (alignment_ == TextAlignment::RIGHT) {
            x -= size_.x;
        }
        
        float scale = fontSize_ / 32.0f;
        
        gFontRenderer->renderText(text_, x, y, scale, color_);
    } else {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(color_.r, color_.g, color_.b, color_.a);
        
        float startX = position_.x;
        switch (alignment_) {
            case TextAlignment::LEFT:
                break;
            case TextAlignment::CENTER:
                startX = position_.x - (size_.x / 2.0f);
                break;
            case TextAlignment::RIGHT:
                startX = position_.x - size_.x;
                break;
        }
        
        float x = startX;
        float y = position_.y;
        float charSize = fontSize_;
        
        for (size_t i = 0; i < text_.length(); i++) {
            char c = text_[i];
            
            if (c == '\n') {
                x = startX;
                y += charSize;
                continue;
            }
            
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + charSize * 0.75f, y);
            glVertex2f(x + charSize * 0.75f, y + charSize);
            glVertex2f(x, y + charSize);
            glEnd();
            
            x += charSize;
        }
    }
}

void Text::setText(const std::string& text) {
    if (text_ != text) {
        text_ = text;
        calculateSize();
    }
}

void Text::setFontSize(float fontSize) {
    if (fontSize_ != fontSize) {
        fontSize_ = fontSize;
        calculateSize();
    }
}

void Text::calculateSize() {
    if (gFontRenderer) {
        float scale = fontSize_ / 32.0f;
        size_ = gFontRenderer->getTextDimensions(text_, scale);
    } else {
        float charWidth = fontSize_;
        
        size_t maxLineLength = 0;
        size_t lineCount = 1;
        
        for (size_t i = 0; i < text_.length(); i++) {
            if (text_[i] == '\n') {
                lineCount++;
            }
        }
        
        size_t startPos = 0;
        size_t endPos = text_.find('\n');
        
        while (endPos != std::string::npos) {
            maxLineLength = std::max(maxLineLength, endPos - startPos);
            startPos = endPos + 1;
            endPos = text_.find('\n', startPos);
        }
        
        maxLineLength = std::max(maxLineLength, text_.length() - startPos);
        
        float estimatedWidth = maxLineLength * charWidth;
        float estimatedHeight = lineCount * fontSize_;
        
        size_ = glm::vec2(estimatedWidth, estimatedHeight);
    }
}

} // namespace ui
} // namespace voidengine 