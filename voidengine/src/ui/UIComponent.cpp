#include "UIComponent.h"

namespace voidengine {
namespace ui {

UIComponent::UIComponent(const std::string& id, const glm::vec2& position, const glm::vec2& size)
    : id_(id), position_(position), size_(size) {
}

} // namespace ui
} // namespace voidengine 