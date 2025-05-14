#include "InputSystem.h"
#include <iostream>
#include <algorithm>

namespace voidengine {
namespace input {

InputSystem* InputSystem::instance_ = nullptr;

std::unique_ptr<InputSystem> gInputSystem = nullptr;

void initializeInputSystem(GLFWwindow* window) {
    if (!gInputSystem) {
        gInputSystem = std::make_unique<InputSystem>();
    }
    gInputSystem->initialize(window);
}

void shutdownInputSystem() {
    if (gInputSystem) {
        gInputSystem->shutdown();
        gInputSystem.reset();
    }
}

InputSystem::InputSystem()
    : window_(nullptr), initialized_(false), nextCallbackId_(0) {
    instance_ = this;
}

InputSystem::~InputSystem() {
    shutdown();
    
    if (instance_ == this) {
        instance_ = nullptr;
    }
}

void InputSystem::initialize(GLFWwindow* window) {
    if (initialized_) {
        return;
    }
    
    window_ = window;
    initialized_ = true;
      
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetCharCallback(window_, charCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetCursorPosCallback(window_, cursorPosCallback);
    glfwSetScrollCallback(window_, scrollCallback);
    glfwSetJoystickCallback(joystickCallback);
    
    double x, y;
    glfwGetCursorPos(window_, &x, &y);
    mousePosition_ = glm::vec2(x, y);
    lastMousePosition_ = mousePosition_;
    
    std::cout << "Input system initialized" << std::endl;
}

void InputSystem::shutdown() {
    if (!initialized_) {
        return;
    }
    
    if (instance_ == this && window_) {
        glfwSetKeyCallback(window_, nullptr);
        glfwSetCharCallback(window_, nullptr);
        glfwSetMouseButtonCallback(window_, nullptr);
        glfwSetCursorPosCallback(window_, nullptr);
        glfwSetScrollCallback(window_, nullptr);
        glfwSetJoystickCallback(nullptr);
    }
    
    keyStates_.clear();
    mouseButtonStates_.clear();
    gamepadButtonStates_.clear();
    gamepadAxisValues_.clear();
    
    callbacks_.clear();
    
    window_ = nullptr;
    initialized_ = false;
    
    std::cout << "Input system shutdown" << std::endl;
}

void InputSystem::update() {
    if (!initialized_) {
        return;
    }
    
    mouseDelta_ = mousePosition_ - lastMousePosition_;
    lastMousePosition_ = mousePosition_;
    
    for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
        if (glfwJoystickPresent(jid)) {
            int buttonCount;
            const unsigned char* buttons = glfwGetJoystickButtons(jid, &buttonCount);
            
            for (int i = 0; i < buttonCount; i++) {
                auto& buttonStates = gamepadButtonStates_[jid];
                ButtonState prevState = ButtonState::RELEASED;
                
                if (buttonStates.find(i) != buttonStates.end()) {
                    prevState = buttonStates[i];
                }
                
                ButtonState newState = ButtonState::RELEASED;
                
                if (buttons[i] == GLFW_PRESS) {
                    if (prevState == ButtonState::PRESSED || 
                        prevState == ButtonState::HELD || 
                        prevState == ButtonState::PRESSED_THIS_FRAME) {
                        newState = ButtonState::HELD;
                    } else {
                        newState = ButtonState::PRESSED_THIS_FRAME;
                        
                        InputEvent event;
                        event.type = EventType::GAMEPAD_BUTTON;
                        event.code = i;
                        event.state = newState;
                        event.gamepadId = jid;
                        
                        processInputEvent(event);
                    }
                } else if (buttons[i] == GLFW_RELEASE) {
                    if (prevState == ButtonState::PRESSED || 
                        prevState == ButtonState::HELD || 
                        prevState == ButtonState::PRESSED_THIS_FRAME) {
                        newState = ButtonState::RELEASED_THIS_FRAME;
                        
                        InputEvent event;
                        event.type = EventType::GAMEPAD_BUTTON;
                        event.code = i;
                        event.state = newState;
                        event.gamepadId = jid;
                        
                        processInputEvent(event);
                    } else {
                        newState = ButtonState::RELEASED;
                    }
                }
                
                buttonStates[i] = newState;
            }
            
            int axisCount;
            const float* axes = glfwGetJoystickAxes(jid, &axisCount);
            
            for (int i = 0; i < axisCount; i++) {
                float value = axes[i];
                
                const float deadzone = 0.15f;
                if (std::abs(value) < deadzone) {
                    value = 0.0f;
                }
                
                gamepadAxisValues_[jid][i] = value;
                
                float prevValue = 0.0f;
                auto& axisValues = gamepadAxisValues_[jid];
                
                if (axisValues.find(i) != axisValues.end()) {
                    prevValue = axisValues[i];
                }
                
                if (std::abs(value - prevValue) > 0.05f) {
                    InputEvent event;
                    event.type = EventType::GAMEPAD_AXIS;
                    event.code = i;
                    event.position = glm::vec2(value, 0.0f);
                    event.gamepadId = jid;
                    
                    processInputEvent(event);
                }
            }
        }
    }
    
    for (auto& pair : keyStates_) {
        if (pair.second == ButtonState::PRESSED_THIS_FRAME) {
            pair.second = ButtonState::HELD;
        } else if (pair.second == ButtonState::RELEASED_THIS_FRAME) {
            pair.second = ButtonState::RELEASED;
        }
    }
    
    for (auto& pair : mouseButtonStates_) {
        if (pair.second == ButtonState::PRESSED_THIS_FRAME) {
            pair.second = ButtonState::HELD;
        } else if (pair.second == ButtonState::RELEASED_THIS_FRAME) {
            pair.second = ButtonState::RELEASED;
        }
    }
    
    for (auto& gamepadPair : gamepadButtonStates_) {
        for (auto& buttonPair : gamepadPair.second) {
            if (buttonPair.second == ButtonState::PRESSED_THIS_FRAME) {
                buttonPair.second = ButtonState::HELD;
            } else if (buttonPair.second == ButtonState::RELEASED_THIS_FRAME) {
                buttonPair.second = ButtonState::RELEASED;
            }
        }
    }
    
    scrollDelta_ = glm::vec2(0.0f);
}

int InputSystem::addCallback(EventType type, const InputCallback& callback) {
    int id = nextCallbackId_++;
    callbacks_.push_back(std::make_pair(type, callback));
    return id;
}

void InputSystem::removeCallback(int callbackId) {
        if (!callbacks_.empty()) {
        callbacks_.pop_back();
    }
}

ButtonState InputSystem::getKeyState(int keyCode) const {
    auto it = keyStates_.find(keyCode);
    if (it != keyStates_.end()) {
        return it->second;
    }
    return ButtonState::RELEASED;
}

ButtonState InputSystem::getMouseButtonState(int button) const {
    auto it = mouseButtonStates_.find(button);
    if (it != mouseButtonStates_.end()) {
        return it->second;
    }
    return ButtonState::RELEASED;
}

ButtonState InputSystem::getGamepadButtonState(int gamepadId, int button) const {
    auto gamepadIt = gamepadButtonStates_.find(gamepadId);
    if (gamepadIt != gamepadButtonStates_.end()) {
        auto buttonIt = gamepadIt->second.find(button);
        if (buttonIt != gamepadIt->second.end()) {
            return buttonIt->second;
        }
    }
    return ButtonState::RELEASED;
}

glm::vec2 InputSystem::getMousePosition() const {
    return mousePosition_;
}

glm::vec2 InputSystem::getMouseDelta() const {
    return mouseDelta_;
}

void InputSystem::setMousePosition(const glm::vec2& position) {
    // Store previous position for delta calculation
    lastMousePosition_ = mousePosition_;
    // Update current position
    mousePosition_ = position;
}

glm::vec2 InputSystem::getGamepadAxis(int gamepadId, int axisX, int axisY) const {
    glm::vec2 result(0.0f);
    
    auto gamepadIt = gamepadAxisValues_.find(gamepadId);
    if (gamepadIt != gamepadAxisValues_.end()) {
        auto axisXIt = gamepadIt->second.find(axisX);
        if (axisXIt != gamepadIt->second.end()) {
            result.x = axisXIt->second;
        }
        
        auto axisYIt = gamepadIt->second.find(axisY);
        if (axisYIt != gamepadIt->second.end()) {
            result.y = axisYIt->second;
        }
    }
    
    return result;
}

float InputSystem::getGamepadTrigger(int gamepadId, int axis) const {
    auto gamepadIt = gamepadAxisValues_.find(gamepadId);
    if (gamepadIt != gamepadAxisValues_.end()) {
        auto axisIt = gamepadIt->second.find(axis);
        if (axisIt != gamepadIt->second.end()) {
            return (axisIt->second + 1.0f) * 0.5f;
        }
    }
    
    return 0.0f;
}

bool InputSystem::isKeyPressed(int keyCode) const {
    ButtonState state = getKeyState(keyCode);
    return state == ButtonState::PRESSED || state == ButtonState::PRESSED_THIS_FRAME || state == ButtonState::HELD;
}

bool InputSystem::isKeyReleased(int keyCode) const {
    return getKeyState(keyCode) == ButtonState::RELEASED || getKeyState(keyCode) == ButtonState::RELEASED_THIS_FRAME;
}

bool InputSystem::isMouseButtonPressed(int button) const {
    ButtonState state = getMouseButtonState(button);
    return state == ButtonState::PRESSED || state == ButtonState::PRESSED_THIS_FRAME || state == ButtonState::HELD;
}

bool InputSystem::wasKeyPressedThisFrame(int keyCode) const {
    return getKeyState(keyCode) == ButtonState::PRESSED_THIS_FRAME;
}

bool InputSystem::wasKeyReleasedThisFrame(int keyCode) const {
    return getKeyState(keyCode) == ButtonState::RELEASED_THIS_FRAME;
}

const std::string& InputSystem::getTextInput() const {
    return textInput_;
}

void InputSystem::clearTextInput() {
    textInput_.clear();
}

const char* InputSystem::getKeyName(int keyCode) {
    return glfwGetKeyName(keyCode, 0);
}

int InputSystem::getKeyScancode(int keyCode) {
    return glfwGetKeyScancode(keyCode);
}

void InputSystem::processInputEvent(const InputEvent& event) {
    for (const auto& callbackPair : callbacks_) {
        if (callbackPair.first == event.type) {
            callbackPair.second(event);
        }
    }
}

void InputSystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (!instance_) return;
    
    ButtonState newState;
    switch (action) {
        case GLFW_PRESS:
            newState = ButtonState::PRESSED_THIS_FRAME;
            break;
        case GLFW_RELEASE:
            newState = ButtonState::RELEASED_THIS_FRAME;
            break;
        case GLFW_REPEAT:
            newState = ButtonState::HELD;
            break;
        default:
            return;
    }
    
    instance_->keyStates_[key] = newState;
    
    InputEvent event;
    event.type = EventType::KEY;
    event.code = key;
    event.state = newState;
    event.mods = mods;
    
    instance_->processInputEvent(event);
}

void InputSystem::charCallback(GLFWwindow* window, unsigned int codepoint) {
    if (!instance_) return;
    
    if (codepoint < 0x80) {
        instance_->textInput_ += static_cast<char>(codepoint);
    } else if (codepoint < 0x800) {
        instance_->textInput_ += static_cast<char>(0xC0 | (codepoint >> 6));
        instance_->textInput_ += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint < 0x10000) {
        instance_->textInput_ += static_cast<char>(0xE0 | (codepoint >> 12));
        instance_->textInput_ += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        instance_->textInput_ += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else {
        instance_->textInput_ += static_cast<char>(0xF0 | (codepoint >> 18));
        instance_->textInput_ += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        instance_->textInput_ += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        instance_->textInput_ += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
}

void InputSystem::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (!instance_) return;
    
    ButtonState newState;
    switch (action) {
        case GLFW_PRESS:
            newState = ButtonState::PRESSED_THIS_FRAME;
            break;
        case GLFW_RELEASE:
            newState = ButtonState::RELEASED_THIS_FRAME;
            break;
        default:
            return;
    }
    
    instance_->mouseButtonStates_[button] = newState;
    
    InputEvent event;
    event.type = EventType::MOUSE_BUTTON;
    event.code = button;
    event.state = newState;
    event.position = instance_->mousePosition_;
    event.mods = mods;
    
    instance_->processInputEvent(event);
}

void InputSystem::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!instance_) return;
    
    glm::vec2 newPos(xpos, ypos);
    glm::vec2 delta = newPos - instance_->mousePosition_;
    
    instance_->mousePosition_ = newPos;
    
    InputEvent event;
    event.type = EventType::MOUSE_MOVE;
    event.position = instance_->mousePosition_;
    event.delta = delta;
    
    instance_->processInputEvent(event);
}

void InputSystem::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!instance_) return;
    
    instance_->scrollDelta_ = glm::vec2(xoffset, yoffset);
    
    InputEvent event;
    event.type = EventType::MOUSE_SCROLL;
    event.delta = instance_->scrollDelta_;
    
    instance_->processInputEvent(event);
}

void InputSystem::joystickCallback(int jid, int event) {
    if (!instance_) return;
    
    if (event == GLFW_CONNECTED) {
        std::cout << "Gamepad " << jid << " connected: " << glfwGetJoystickName(jid) << std::endl;
    } else if (event == GLFW_DISCONNECTED) {
        std::cout << "Gamepad " << jid << " disconnected" << std::endl;
        
        instance_->gamepadButtonStates_.erase(jid);
        instance_->gamepadAxisValues_.erase(jid);
    }
}

void InputSystem::updateKeyState(int keyCode, ButtonState state) {
    keyStates_[keyCode] = state;
}

void InputSystem::updateMouseButtonState(int button, ButtonState state) {
    mouseButtonStates_[button] = state;
}

void InputSystem::appendTextInput(unsigned int codepoint) {
    if (codepoint < 0x80) {
        textInput_ += static_cast<char>(codepoint);
    } else if (codepoint < 0x800) {
        textInput_ += static_cast<char>(0xC0 | (codepoint >> 6));
        textInput_ += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint < 0x10000) {
        textInput_ += static_cast<char>(0xE0 | (codepoint >> 12));
        textInput_ += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        textInput_ += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else {
        textInput_ += static_cast<char>(0xF0 | (codepoint >> 18));
        textInput_ += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        textInput_ += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        textInput_ += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
}

} // namespace input
} // namespace voidengine 