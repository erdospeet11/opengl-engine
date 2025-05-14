#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace voidengine {
namespace input {

enum class ButtonState {
    RELEASED,
    PRESSED,
    HELD,
    RELEASED_THIS_FRAME,
    PRESSED_THIS_FRAME
};

enum class EventType {
    KEY,
    MOUSE_BUTTON,
    MOUSE_MOVE,
    MOUSE_SCROLL,
    GAMEPAD_BUTTON,
    GAMEPAD_AXIS
};

struct InputEvent {
    EventType type;
    int code;
    ButtonState state;
    glm::vec2 position;
    glm::vec2 delta;
    int mods;
    int gamepadId;
};

using InputCallback = std::function<void(const InputEvent&)>;

class InputSystem {
public:
    InputSystem();
    ~InputSystem();

    void initialize(GLFWwindow* window);
    void shutdown();

    void update();

    int addCallback(EventType type, const InputCallback& callback);
    void removeCallback(int callbackId);

    ButtonState getKeyState(int keyCode) const;
    ButtonState getMouseButtonState(int button) const;
    ButtonState getGamepadButtonState(int gamepadId, int button) const;

    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseDelta() const;
    void setMousePosition(const glm::vec2& position);

    glm::vec2 getGamepadAxis(int gamepadId, int axisX, int axisY) const;
    float getGamepadTrigger(int gamepadId, int axis) const;

    bool isKeyPressed(int keyCode) const;
    bool isKeyReleased(int keyCode) const;
    bool isMouseButtonPressed(int button) const;
    bool wasKeyPressedThisFrame(int keyCode) const;
    bool wasKeyReleasedThisFrame(int keyCode) const;

    const std::string& getTextInput() const;
    void clearTextInput();

    static const char* getKeyName(int keyCode);
    static int getKeyScancode(int keyCode);
    
    void processInputEvent(const InputEvent& event);
    
    void updateKeyState(int keyCode, ButtonState state);
    void updateMouseButtonState(int button, ButtonState state);
    void appendTextInput(unsigned int codepoint);

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void joystickCallback(int jid, int event);

    GLFWwindow* window_;
    bool initialized_;

    std::unordered_map<int, ButtonState> keyStates_;
    std::unordered_map<int, ButtonState> mouseButtonStates_;
    std::unordered_map<int, std::unordered_map<int, ButtonState>> gamepadButtonStates_;

    glm::vec2 mousePosition_;
    glm::vec2 lastMousePosition_;
    glm::vec2 mouseDelta_;
    glm::vec2 scrollDelta_;

    std::unordered_map<int, std::unordered_map<int, float>> gamepadAxisValues_;

    std::string textInput_;

    std::vector<std::pair<EventType, InputCallback>> callbacks_;
    int nextCallbackId_;

    static InputSystem* instance_;
};

extern std::unique_ptr<InputSystem> gInputSystem;


void initializeInputSystem(GLFWwindow* window);

void shutdownInputSystem();

} // namespace input
} // namespace voidengine 