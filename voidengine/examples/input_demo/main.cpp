#include "window/Window.h"
#include "input/Input.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace voidengine;

class InputDemo {
public:
    InputDemo() : window_(800, 600, "VoidEngine Input Demo"), quitRequested_(false) {
        // Setup input mappings
        setupInputMappings();
    }
    
    void run() {
        while (!window_.shouldClose() && !quitRequested_) {
            // Clear screen
            window_.clear(0.1f, 0.1f, 0.2f, 1.0f);
            
            // Print input statuses to console
            printInputStatus();
            
            // Swap buffers and poll events
            window_.swapBuffers();
            window_.pollEvents();
        }
    }

private:
    void setupInputMappings() {
        // Movement controls
        gInputMapping->bindKeyToAction("move_up", Keys::W);
        gInputMapping->bindKeyToAction("move_up", Keys::UP);
        gInputMapping->bindGamepadButtonToAction("move_up", GamepadButtons::DPAD_UP);
        
        gInputMapping->bindKeyToAction("move_down", Keys::S);
        gInputMapping->bindKeyToAction("move_down", Keys::DOWN);
        gInputMapping->bindGamepadButtonToAction("move_down", GamepadButtons::DPAD_DOWN);
        
        gInputMapping->bindKeyToAction("move_left", Keys::A);
        gInputMapping->bindKeyToAction("move_left", Keys::LEFT);
        gInputMapping->bindGamepadButtonToAction("move_left", GamepadButtons::DPAD_LEFT);
        
        gInputMapping->bindKeyToAction("move_right", Keys::D);
        gInputMapping->bindKeyToAction("move_right", Keys::RIGHT);
        gInputMapping->bindGamepadButtonToAction("move_right", GamepadButtons::DPAD_RIGHT);
        
        // Action controls
        gInputMapping->bindKeyToAction("jump", Keys::SPACE);
        gInputMapping->bindGamepadButtonToAction("jump", GamepadButtons::A);
        
        gInputMapping->bindKeyToAction("attack", Keys::LEFT_CONTROL);
        gInputMapping->bindMouseButtonToAction("attack", MouseButtons::LEFT);
        gInputMapping->bindGamepadButtonToAction("attack", GamepadButtons::X);
        
        gInputMapping->bindKeyToAction("defend", Keys::LEFT_SHIFT);
        gInputMapping->bindMouseButtonToAction("defend", MouseButtons::RIGHT);
        gInputMapping->bindGamepadButtonToAction("defend", GamepadButtons::B);
        
        // Analog movement - gamepad left stick
        gInputMapping->bindGamepadAxisToAction("move", GamepadAxes::LEFT_X, GamepadAxes::LEFT_Y);
        
        // Look controls - mouse movement
        gInputMapping->bindMouseAxisToAction("look", MouseAxes::X, MouseAxes::Y);
        
        // Gamepad right stick
        gInputMapping->bindGamepadAxisToAction("look", GamepadAxes::RIGHT_X, GamepadAxes::RIGHT_Y);
        
        // Quit
        gInputMapping->bindKeyToAction("quit", Keys::ESCAPE);
        gInputMapping->bindKeyToAction("quit", Keys::Q);
        
        // Register callback for quit action
        gInputMapping->addActionCallback("quit", [this](const std::string& action, const ActionState& state) {
            if (state.justActivated) {
                quitRequested_ = true;
            }
        });
        
        // Register callbacks to print when actions are activated
        const std::vector<std::string> actionsToPrint = {
            "move_up", "move_down", "move_left", "move_right", 
            "jump", "attack", "defend"
        };
        
        for (const auto& action : actionsToPrint) {
            gInputMapping->addActionCallback(action, [action](const std::string& actionName, const ActionState& state) {
                if (state.justActivated) {
                    std::cout << "Action activated: " << actionName << std::endl;
                } else if (state.justDeactivated) {
                    std::cout << "Action deactivated: " << actionName << std::endl;
                }
            });
        }
        
        // Print instructions
        std::cout << "\n===== INPUT DEMO INSTRUCTIONS =====\n";
        std::cout << "WASD/Arrows: Move\n";
        std::cout << "Mouse/Right Stick: Look\n";
        std::cout << "Space/A: Jump\n";
        std::cout << "LCtrl/LMB/X: Attack\n";
        std::cout << "LShift/RMB/B: Defend\n";
        std::cout << "Esc/Q: Quit\n";
        std::cout << "================================\n\n";
    }
    
    void printInputStatus() {
        static int frameCounter = 0;
        frameCounter++;
        
        // Only print every 30 frames to avoid console spam
        if (frameCounter % 30 != 0) {
            return;
        }
        
        // Clear console (platform-specific)
        std::cout << "\033[2J\033[1;1H";  // ANSI escape sequence to clear screen
        
        std::cout << "===== INPUT DEMO STATUS =====\n";
        
        // Keyboard status
        std::cout << "Keyboard:\n";
        std::cout << "  Up: " << (gInputMapping->isActionActive("move_up") ? "PRESSED" : "released") << "\n";
        std::cout << "  Down: " << (gInputMapping->isActionActive("move_down") ? "PRESSED" : "released") << "\n";
        std::cout << "  Left: " << (gInputMapping->isActionActive("move_left") ? "PRESSED" : "released") << "\n";
        std::cout << "  Right: " << (gInputMapping->isActionActive("move_right") ? "PRESSED" : "released") << "\n";
        std::cout << "  Jump: " << (gInputMapping->isActionActive("jump") ? "PRESSED" : "released") << "\n";
        std::cout << "  Attack: " << (gInputMapping->isActionActive("attack") ? "PRESSED" : "released") << "\n";
        std::cout << "  Defend: " << (gInputMapping->isActionActive("defend") ? "PRESSED" : "released") << "\n";
        
        // Mouse status
        std::cout << "\nMouse:\n";
        std::cout << "  Position: " << std::fixed << std::setprecision(1) 
                 << gInputSystem->getMousePosition().x << ", " 
                 << gInputSystem->getMousePosition().y << "\n";
        std::cout << "  Delta: " << std::fixed << std::setprecision(1) 
                 << gInputSystem->getMouseDelta().x << ", " 
                 << gInputSystem->getMouseDelta().y << "\n";
        std::cout << "  Left: " << (gInputSystem->isMouseButtonPressed(MouseButtons::LEFT) ? "PRESSED" : "released") << "\n";
        std::cout << "  Right: " << (gInputSystem->isMouseButtonPressed(MouseButtons::RIGHT) ? "PRESSED" : "released") << "\n";
        
        // Gamepad status
        // To detect gamepads, check if we can get any gamepad axis values
        bool gamepadConnected = false;
        for (int i = 0; i < 16; i++) {
            if (glm::length(gInputSystem->getGamepadAxis(i, 0, 1)) > 0.01f ||
                gInputSystem->getGamepadButtonState(i, 0) != ButtonState::RELEASED) {
                gamepadConnected = true;
                break;
            }
        }
        
        std::cout << "\nGamepad: " << (gamepadConnected ? "Connected" : "Disconnected") << "\n";
        if (gamepadConnected) {
            std::cout << "  A: " << (gInputSystem->getGamepadButtonState(0, GamepadButtons::A) != ButtonState::RELEASED ? "PRESSED" : "released") << "\n";
            std::cout << "  B: " << (gInputSystem->getGamepadButtonState(0, GamepadButtons::B) != ButtonState::RELEASED ? "PRESSED" : "released") << "\n";
            std::cout << "  X: " << (gInputSystem->getGamepadButtonState(0, GamepadButtons::X) != ButtonState::RELEASED ? "PRESSED" : "released") << "\n";
            std::cout << "  Y: " << (gInputSystem->getGamepadButtonState(0, GamepadButtons::Y) != ButtonState::RELEASED ? "PRESSED" : "released") << "\n";
            
            // Left stick
            glm::vec2 leftStick = gInputSystem->getGamepadAxis(0, GamepadAxes::LEFT_X, GamepadAxes::LEFT_Y);
            std::cout << "  Left Stick: " << std::fixed << std::setprecision(2) 
                     << leftStick.x << ", " << leftStick.y 
                     << " (Mag: " << glm::length(leftStick) << ")\n";
            
            // Right stick
            glm::vec2 rightStick = gInputSystem->getGamepadAxis(0, GamepadAxes::RIGHT_X, GamepadAxes::RIGHT_Y);
            std::cout << "  Right Stick: " << std::fixed << std::setprecision(2) 
                     << rightStick.x << ", " << rightStick.y 
                     << " (Mag: " << glm::length(rightStick) << ")\n";
        }
        
        // Calculate movement vector from individual directions or analog stick
        glm::vec2 moveVector(0.0f);
        
        // Digital movement
        if (gInputMapping->isActionActive("move_up")) moveVector.y -= 1.0f;
        if (gInputMapping->isActionActive("move_down")) moveVector.y += 1.0f;
        if (gInputMapping->isActionActive("move_left")) moveVector.x -= 1.0f;
        if (gInputMapping->isActionActive("move_right")) moveVector.x += 1.0f;
        
        // Add analog movement from action
        glm::vec2 analogMoveVector = gInputMapping->getActionVector("move");
        moveVector += analogMoveVector;
        
        // Normalize if length > 1
        if (glm::length(moveVector) > 1.0f) {
            moveVector = glm::normalize(moveVector);
        }
        
        // Movement and look vectors
        std::cout << "\nMovement Vector: " << std::fixed << std::setprecision(2) 
                 << moveVector.x << ", " << moveVector.y 
                 << " (Mag: " << glm::length(moveVector) << ")\n";
        
        glm::vec2 lookVector = gInputMapping->getActionVector("look");
        std::cout << "Look Vector: " << std::fixed << std::setprecision(2) 
                 << lookVector.x << ", " << lookVector.y 
                 << " (Mag: " << glm::length(lookVector) << ")\n";
        
        std::cout << "=============================\n";
    }
    
private:
    window::Window window_;
    bool quitRequested_;
};

int main() {
    try {
        InputDemo demo;
        demo.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 