#include "FontRenderer.h"
#include <iostream>

namespace voidengine {
namespace ui {

std::unique_ptr<FontRenderer> gFontRenderer = nullptr;

bool initializeFontSystem() {
    if (!gFontRenderer) {
        gFontRenderer = std::make_unique<FontRenderer>();
    }
    return gFontRenderer->initialize();
}

void shutdownFontSystem() {
    gFontRenderer.reset();
}

FontRenderer::FontRenderer() 
    : isInitialized(false), fontLoaded(false) {
}

FontRenderer::~FontRenderer() {
    for (auto& pair : characters) {
        glDeleteTextures(1, &pair.second.textureID);
    }
    
    if (fontLoaded) {
        FT_Done_Face(face);
    }
    
    if (isInitialized) {
        FT_Done_FreeType(ft);
    }
}

bool FontRenderer::initialize() {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not initialize FreeType Library" << std::endl;
        return false;
    }
    
    isInitialized = true;
    return true;
}

bool FontRenderer::loadFont(const std::string& fontPath, unsigned int fontSize) {
    if (!isInitialized) {
        std::cerr << "ERROR::FONTRENDERER: FreeType not initialized" << std::endl;
        return false;
    }
    
    if (fontLoaded) {
        for (auto& pair : characters) {
            glDeleteTextures(1, &pair.second.textureID);
        }
        characters.clear();
        
        FT_Done_Face(face);
        fontLoaded = false;
    }
    
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font at " << fontPath << std::endl;
        return false;
    }
    
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (unsigned char c = 32; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character " << c << std::endl;
            continue;
        }
        
        unsigned int texture = createTexture(
            face->glyph->bitmap.buffer,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows
        );
        
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        
        characters.insert(std::pair<char, Character>(c, character));
    }
    
    fontLoaded = true;
    return true;
}

unsigned int FontRenderer::createTexture(unsigned char* data, unsigned int width, unsigned int height) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if (width > 0 && height > 0) {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            width,
            height,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            data
        );
    } else {
        unsigned char emptyData = 0;
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            1,
            1,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            &emptyData
        );
    }
    
    return texture;
}

void FontRenderer::renderText(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    if (!fontLoaded) {
        return;
    }
    
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_TEXTURE_2D);
    
    glColor4f(color.r, color.g, color.b, color.a);
    
    float xpos = x;
    float ypos = y + (face->size->metrics.height >> 6) * scale * 0.75f;
    
    for (char c : text) {
        if (c == '\n') {
            xpos = x;
            ypos += (face->size->metrics.height >> 6) * scale;
            continue;
        }
        
        if (characters.find(c) == characters.end()) {
            continue;
        }
        
        Character ch = characters[c];
        
        float x0 = xpos + ch.bearing.x * scale;
        float y0 = ypos - ch.bearing.y * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        
        if (ch.size.x > 0 && ch.size.y > 0) {
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            
            glBegin(GL_QUADS);
            
            glTexCoord2f(0.0f, 0.0f); glVertex2f(x0, y0);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(x0 + w, y0);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(x0 + w, y0 + h);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(x0, y0 + h);
            
            glEnd();
        }
        
        xpos += (ch.advance >> 6) * scale;
    }
    
    glPopAttrib();
}

glm::vec2 FontRenderer::getTextDimensions(const std::string& text, float scale) {
    if (!fontLoaded || text.empty()) {
        return glm::vec2(0.0f);
    }
    
    float maxWidth = 0.0f;
    float width = 0.0f;
    float height = (face->size->metrics.height >> 6) * scale;
    int numLines = 1;
    
    for (char c : text) {
        if (c == '\n') {
            maxWidth = std::max(maxWidth, width);
            width = 0.0f;
            numLines++;
            continue;
        }
        
        if (characters.find(c) == characters.end()) {
            continue;
        }
        
        Character ch = characters[c];
        width += (ch.advance >> 6) * scale;
    }
    
    maxWidth = std::max(maxWidth, width);
    
    float totalHeight = height * numLines;
    
    return glm::vec2(maxWidth, totalHeight);
}

} // namespace ui
} // namespace voidengine 