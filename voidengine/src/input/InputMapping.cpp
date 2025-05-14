#include "InputMapping.h"
#include <algorithm>
#include <iostream>

namespace voidengine {
namespace input {

std::unique_ptr<InputMapping> gInputMapping = nullptr;

void initializeInputMapping() {
    if (!gInputMapping) {
        gInputMapping = std::make_unique<InputMapping>();
    }
    gInputMapping->initialize();
}

InputMapping::InputMapping()
    : initialized_(false), nextCallbackId_(0), inputCallbackId_(-1) {
}

InputMapping::~InputMapping() {
    if (inputCallbackId_ >= 0 && gInputSystem) {
        gInputSystem->removeCallback(inputCallbackId_);
    }
}

void InputMapping::initialize() {
    if (initialized_) {
        return;
    }
    
    if (gInputSystem) {
        inputCallbackId_ = gInputSystem->addCallback(
            EventType::KEY, 
            [this](const InputEvent& event) { this->onInputEvent(event); }
        );
        
        gInputSystem->addCallback(
            EventType::MOUSE_BUTTON, 
            [this](const InputEvent& event) { this->onInputEvent(event); }
        );
        
        gInputSystem->addCallback(
            EventType::MOUSE_MOVE, 
            [this](const InputEvent& event) { this->onInputEvent(event); }
        );
        
        gInputSystem->addCallback(
            EventType::MOUSE_SCROLL, 
            [this](const InputEvent& event) { this->onInputEvent(event); }
        );
        
        gInputSystem->addCallback(
            EventType::GAMEPAD_BUTTON, 
            [this](const InputEvent& event) { this->onInputEvent(event); }
        );
        
        gInputSystem->addCallback(
            EventType::GAMEPAD_AXIS, 
            [this](const InputEvent& event) { this->onInputEvent(event); }
        );
    }
    
    initialized_ = true;
    std::cout << "Input mapping initialized" << std::endl;
}

void InputMapping::update() {
    if (!initialized_) {
        return;
    }
    
    std::unordered_map<std::string, bool> previousActiveStates;
    for (auto& pair : actionStates_) {
        previousActiveStates[pair.first] = pair.second.active;
        
        pair.second.justActivated = false;
        pair.second.justDeactivated = false;
    }
    
    for (auto& pair : actionBindings_) {
        const std::string& actionName = pair.first;
        const std::vector<InputBinding>& bindings = pair.second;
        
        ActionState& state = actionStates_[actionName];
        state.active = false;
        state.value = 0.0f;
        state.vector = glm::vec2(0.0f);
        
        for (const auto& binding : bindings) {
            updateActionStateFromBinding(binding, state);
        }
        
        bool wasActive = previousActiveStates[actionName];
        if (state.active && !wasActive) {
            state.justActivated = true;
        } else if (!state.active && wasActive) {
            state.justDeactivated = true;
        }
        
        if (state.justActivated || state.justDeactivated) {
            for (const auto& callbackPair : callbacks_) {
                if (callbackPair.first == actionName) {
                    callbackPair.second(actionName, state);
                }
            }
        }
    }
}

void InputMapping::bindKeyToAction(const std::string& actionName, int keyCode) {
    InputBinding binding;
    binding.actionName = actionName;
    binding.type = InputType::KEYBOARD;
    binding.primaryCode = keyCode;
    
    actionBindings_[actionName].push_back(binding);
}

void InputMapping::bindMouseButtonToAction(const std::string& actionName, int button) {
    InputBinding binding;
    binding.actionName = actionName;
    binding.type = InputType::MOUSE_BUTTON;
    binding.primaryCode = button;
    
    actionBindings_[actionName].push_back(binding);
}

void InputMapping::bindMouseAxisToAction(const std::string& actionName, int axisX, int axisY, bool invert) {
    InputBinding binding;
    binding.actionName = actionName;
    binding.type = InputType::MOUSE_AXIS;
    binding.primaryCode = axisX;
    binding.secondaryCode = axisY;
    binding.invert = invert;
    
    actionBindings_[actionName].push_back(binding);
}

void InputMapping::bindGamepadButtonToAction(const std::string& actionName, int button, int gamepadId) {
    InputBinding binding;
    binding.actionName = actionName;
    binding.type = InputType::GAMEPAD_BUTTON;
    binding.primaryCode = button;
    binding.gamepadId = gamepadId;
    
    actionBindings_[actionName].push_back(binding);
}

void InputMapping::bindGamepadAxisToAction(const std::string& actionName, int axisX, int axisY, int gamepadId, bool invert) {
    InputBinding binding;
    binding.actionName = actionName;
    binding.type = InputType::GAMEPAD_AXIS;
    binding.primaryCode = axisX;
    binding.secondaryCode = axisY;
    binding.gamepadId = gamepadId;
    binding.invert = invert;
    
    actionBindings_[actionName].push_back(binding);
}

void InputMapping::clearActionBindings(const std::string& actionName) {
    actionBindings_.erase(actionName);
}

void InputMapping::clearAllBindings() {
    actionBindings_.clear();
}

bool InputMapping::isActionActive(const std::string& actionName) const {
    auto it = actionStates_.find(actionName);
    if (it != actionStates_.end()) {
        return it->second.active;
    }
    return false;
}

bool InputMapping::wasActionJustActivated(const std::string& actionName) const {
    auto it = actionStates_.find(actionName);
    if (it != actionStates_.end()) {
        return it->second.justActivated;
    }
    return false;
}

bool InputMapping::wasActionJustDeactivated(const std::string& actionName) const {
    auto it = actionStates_.find(actionName);
    if (it != actionStates_.end()) {
        return it->second.justDeactivated;
    }
    return false;
}

float InputMapping::getActionValue(const std::string& actionName) const {
    auto it = actionStates_.find(actionName);
    if (it != actionStates_.end()) {
        return it->second.value;
    }
    return 0.0f;
}

glm::vec2 InputMapping::getActionVector(const std::string& actionName) const {
    auto it = actionStates_.find(actionName);
    if (it != actionStates_.end()) {
        return it->second.vector;
    }
    return glm::vec2(0.0f);
}

int InputMapping::addActionCallback(const std::string& actionName, const ActionCallback& callback) {
    int id = nextCallbackId_++;
    callbacks_.push_back(std::make_pair(actionName, callback));
    return id;
}

void InputMapping::removeActionCallback(int callbackId) {
    if (!callbacks_.empty()) {
        callbacks_.pop_back();
    }
}

void InputMapping::onInputEvent(const InputEvent& event) {
}

void InputMapping::updateActionStateFromBinding(const InputBinding& binding, ActionState& state) {
    if (!gInputSystem) {
        return;
    }
    
    switch (binding.type) {
        case InputType::KEYBOARD: {
            ButtonState keyState = gInputSystem->getKeyState(binding.primaryCode);
            bool isActive = keyState == ButtonState::PRESSED || 
                           keyState == ButtonState::PRESSED_THIS_FRAME || 
                           keyState == ButtonState::HELD;
            
            if (isActive) {
                state.active = true;
                state.value = 1.0f;
            }
            break;
        }
        
        case InputType::MOUSE_BUTTON: {
            ButtonState buttonState = gInputSystem->getMouseButtonState(binding.primaryCode);
            bool isActive = buttonState == ButtonState::PRESSED || 
                           buttonState == ButtonState::PRESSED_THIS_FRAME || 
                           buttonState == ButtonState::HELD;
            
            if (isActive) {
                state.active = true;
                state.value = 1.0f;
            }
            break;
        }
        
        case InputType::MOUSE_AXIS: {
            if (binding.primaryCode == 0) {
                float delta = gInputSystem->getMouseDelta().x;
                if (binding.invert) delta = -delta;
                
                if (std::abs(delta) > 0.01f) {
                    state.active = true;
                    state.value = delta;
                    state.vector.x = delta;
                }
            } else if (binding.primaryCode == 1) {
                float delta = gInputSystem->getMouseDelta().y;
                if (binding.invert) delta = -delta;
                
                if (std::abs(delta) > 0.01f) {
                    state.active = true;
                    state.value = delta;
                    state.vector.y = delta;
                }
            } else if (binding.primaryCode == 2) {
                //mouse scroll x
            } else if (binding.primaryCode == 3) {
                //mouse scroll y
            }
            
            if (binding.secondaryCode >= 0) {
                glm::vec2 mouseDelta = gInputSystem->getMouseDelta();
                if (binding.invert) {
                    mouseDelta = -mouseDelta;
                }
                
                if (glm::length(mouseDelta) > 0.01f) {
                    state.active = true;
                    state.value = glm::length(mouseDelta);
                    state.vector = mouseDelta;
                }
            }
            break;
        }
        
        case InputType::GAMEPAD_BUTTON: {
            ButtonState buttonState = gInputSystem->getGamepadButtonState(binding.gamepadId, binding.primaryCode);
            bool isActive = buttonState == ButtonState::PRESSED || 
                           buttonState == ButtonState::PRESSED_THIS_FRAME || 
                           buttonState == ButtonState::HELD;
            
            if (isActive) {
                state.active = true;
                state.value = 1.0f;
            }
            break;
        }
        
        case InputType::GAMEPAD_AXIS: {
            float value = 0.0f;
            auto axisValue = gInputSystem->getGamepadAxis(binding.gamepadId, binding.primaryCode, binding.secondaryCode);
            
            if (binding.secondaryCode < 0) {
                value = axisValue.x;
                if (binding.invert) value = -value;
                
                if (std::abs(value) > 0.01f) {
                    state.active = true;
                    state.value = value;
                    
                    state.vector.x = value;
                }
            } else {
                glm::vec2 vector = axisValue;
                if (binding.invert) vector = -vector;
                
                float length = glm::length(vector);
                if (length > 0.01f) {
                    state.active = true;
                    state.value = length;
                    state.vector = vector;
                }
            }
            break;
        }
    }
}

} // namespace input
} // namespace voidengine 