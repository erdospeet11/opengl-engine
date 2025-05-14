#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace voidengine {
namespace ui {

struct Character {
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class FontRenderer {
public:
    FontRenderer();
    ~FontRenderer();

    bool initialize();

    bool loadFont(const std::string& fontPath, unsigned int fontSize);

    void renderText(const std::string& text, float x, float y, 
                   float scale, const glm::vec4& color);

    glm::vec2 getTextDimensions(const std::string& text, float scale);

private:
    FT_Library ft;
    FT_Face face;
    std::map<char, Character> characters;
    bool isInitialized;
    bool fontLoaded;
    
    unsigned int createTexture(unsigned char* data, unsigned int width, unsigned int height);
};

extern std::unique_ptr<FontRenderer> gFontRenderer;

bool initializeFontSystem();

void shutdownFontSystem();

} // namespace ui
} // namespace voidengine 