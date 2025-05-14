#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace voidengine {
namespace ui {

class UIContext;

class UIComponent {
public:
    UIComponent(const std::string& id, const glm::vec2& position, const glm::vec2& size);
    virtual ~UIComponent() = default;

    virtual void initialize() {}
    virtual void update(float deltaTime) {}
    virtual void render() = 0;

    const std::string& getId() const { return id_; }
    
    const glm::vec2& getPosition() const { return position_; }
    void setPosition(const glm::vec2& position) { position_ = position; }
    
    const glm::vec2& getSize() const { return size_; }
    void setSize(const glm::vec2& size) { size_ = size; }
    
    bool isVisible() const { return isVisible_; }
    void setVisible(bool visible) { isVisible_ = visible; }
    
    bool isEnabled() const { return isEnabled_; }
    void setEnabled(bool enabled) { isEnabled_ = enabled; }

protected:
    std::string id_;
    glm::vec2 position_;
    glm::vec2 size_;
    bool isVisible_ = true;
    bool isEnabled_ = true;
};

} // namespace ui
} // namespace voidengine 