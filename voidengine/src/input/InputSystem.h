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

// Key and button states
enum class ButtonState {
    RELEASED,
    PRESSED,
    HELD,
    RELEASED_THIS_FRAME,
    PRESSED_THIS_FRAME
};

// Input event types
enum class EventType {
    KEY,
    MOUSE_BUTTON,
    MOUSE_MOVE,
    MOUSE_SCROLL,
    GAMEPAD_BUTTON,
    GAMEPAD_AXIS
};

// Event data structure
struct InputEvent {
    EventType type;
    int code;                // Key code, mouse button, gamepad button
    ButtonState state;       // Button state
    glm::vec2 position;      // Mouse position or axis value
    glm::vec2 delta;         // Change in position or scroll amount
    int mods;                // Modifier keys (shift, ctrl, alt)
    int gamepadId;           // Gamepad ID
};

// Input callback type
using InputCallback = std::function<void(const InputEvent&)>;

class InputSystem {
public:
    InputSystem();
    ~InputSystem();

    // Initialization and shutdown
    void initialize(GLFWwindow* window);
    void shutdown();

    // Update function to be called each frame
    void update();

    // Register/unregister callbacks for specific input events
    int addCallback(EventType type, const InputCallback& callback);
    void removeCallback(int callbackId);

    // Key/button state checking
    ButtonState getKeyState(int keyCode) const;
    ButtonState getMouseButtonState(int button) const;
    ButtonState getGamepadButtonState(int gamepadId, int button) const;

    // Mouse position and delta
    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseDelta() const;

    // Gamepad axis values
    glm::vec2 getGamepadAxis(int gamepadId, int axisX, int axisY) const;
    float getGamepadTrigger(int gamepadId, int axis) const;

    // Helper methods
    bool isKeyPressed(int keyCode) const;
    bool isKeyReleased(int keyCode) const;
    bool isMouseButtonPressed(int button) const;
    bool wasKeyPressedThisFrame(int keyCode) const;
    bool wasKeyReleasedThisFrame(int keyCode) const;

    // Text input
    const std::string& getTextInput() const;
    void clearTextInput();

    // Utility functions
    static const char* getKeyName(int keyCode);
    static int getKeyScancode(int keyCode);

private:
    // Process input callbacks
    void processInputEvent(const InputEvent& event);

    // GLFW callback handling
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void joystickCallback(int jid, int event);

    // Data members
    GLFWwindow* window_;
    bool initialized_;

    // Button states
    std::unordered_map<int, ButtonState> keyStates_;
    std::unordered_map<int, ButtonState> mouseButtonStates_;
    std::unordered_map<int, std::unordered_map<int, ButtonState>> gamepadButtonStates_;

    // Mouse state
    glm::vec2 mousePosition_;
    glm::vec2 lastMousePosition_;
    glm::vec2 mouseDelta_;
    glm::vec2 scrollDelta_;

    // Gamepad state
    std::unordered_map<int, std::unordered_map<int, float>> gamepadAxisValues_;

    // Text input
    std::string textInput_;

    // Callbacks
    std::vector<std::pair<EventType, InputCallback>> callbacks_;
    int nextCallbackId_;

    // Static instance for callbacks
    static InputSystem* instance_;
};

// Global input system instance
extern std::unique_ptr<InputSystem> gInputSystem;

// Initialize global input system
void initializeInputSystem(GLFWwindow* window);

// Shutdown global input system
void shutdownInputSystem();

} // namespace input
} // namespace voidengine 