#pragma once

#include "InputSystem.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

namespace voidengine {
namespace input {

enum class InputType {
    KEYBOARD,
    MOUSE_BUTTON,
    MOUSE_AXIS,
    GAMEPAD_BUTTON,
    GAMEPAD_AXIS
};

struct InputBinding {
    std::string actionName;
    InputType type;
    int primaryCode;
    int secondaryCode;
    int gamepadId;
    float scale;
    bool invert;
    
    InputBinding() : 
        type(InputType::KEYBOARD), 
        primaryCode(0), 
        secondaryCode(-1), 
        gamepadId(0),
        scale(1.0f),
        invert(false) {}
    
    InputBinding(const std::string& action, InputType t, int code) :
        actionName(action),
        type(t),
        primaryCode(code),
        secondaryCode(-1),
        gamepadId(0),
        scale(1.0f),
        invert(false) {}
};

struct ActionState {
    bool active;
    bool justActivated;
    bool justDeactivated;
    float value;
    glm::vec2 vector;
    
    ActionState() : 
        active(false), 
        justActivated(false), 
        justDeactivated(false), 
        value(0.0f),
        vector(0.0f) {}
};

using ActionCallback = std::function<void(const std::string& actionName, const ActionState& state)>;

class InputMapping {
public:
    InputMapping();
    ~InputMapping();
    
    void initialize();
    
    void update();
    
    void bindKeyToAction(const std::string& actionName, int keyCode);
    void bindMouseButtonToAction(const std::string& actionName, int button);
    void bindMouseAxisToAction(const std::string& actionName, int axisX, int axisY = -1, bool invert = false);
    void bindGamepadButtonToAction(const std::string& actionName, int button, int gamepadId = 0);
    void bindGamepadAxisToAction(const std::string& actionName, int axisX, int axisY = -1, int gamepadId = 0, bool invert = false);
    
    void clearActionBindings(const std::string& actionName);
    void clearAllBindings();
    
    bool isActionActive(const std::string& actionName) const;
    bool wasActionJustActivated(const std::string& actionName) const;
    bool wasActionJustDeactivated(const std::string& actionName) const;
    float getActionValue(const std::string& actionName) const;
    glm::vec2 getActionVector(const std::string& actionName) const;
    
    int addActionCallback(const std::string& actionName, const ActionCallback& callback);
    void removeActionCallback(int callbackId);
    
private:
    void onInputEvent(const InputEvent& event);
    
    void updateActionStateFromBinding(const InputBinding& binding, ActionState& state);
    
    bool initialized_;
    
    std::unordered_map<std::string, std::vector<InputBinding>> actionBindings_;
    
    std::unordered_map<std::string, ActionState> actionStates_;
    
    std::vector<std::pair<std::string, ActionCallback>> callbacks_;
    
    int nextCallbackId_;
    
    int inputCallbackId_;
};

extern std::unique_ptr<InputMapping> gInputMapping;

void initializeInputMapping();

} // namespace input
} // namespace voidengine 