#pragma once

#include <GLFW/glfw3.h>

namespace voidengine {
namespace input {

namespace Keys {
    constexpr int SPACE = GLFW_KEY_SPACE;
    constexpr int APOSTROPHE = GLFW_KEY_APOSTROPHE;
    constexpr int COMMA = GLFW_KEY_COMMA;
    constexpr int MINUS = GLFW_KEY_MINUS;
    constexpr int PERIOD = GLFW_KEY_PERIOD;
    constexpr int SLASH = GLFW_KEY_SLASH;
    constexpr int NUM_0 = GLFW_KEY_0;
    constexpr int NUM_1 = GLFW_KEY_1;
    constexpr int NUM_2 = GLFW_KEY_2;
    constexpr int NUM_3 = GLFW_KEY_3;
    constexpr int NUM_4 = GLFW_KEY_4;
    constexpr int NUM_5 = GLFW_KEY_5;
    constexpr int NUM_6 = GLFW_KEY_6;
    constexpr int NUM_7 = GLFW_KEY_7;
    constexpr int NUM_8 = GLFW_KEY_8;
    constexpr int NUM_9 = GLFW_KEY_9;
    constexpr int SEMICOLON = GLFW_KEY_SEMICOLON;
    constexpr int EQUAL = GLFW_KEY_EQUAL;
    constexpr int A = GLFW_KEY_A;
    constexpr int B = GLFW_KEY_B;
    constexpr int C = GLFW_KEY_C;
    constexpr int D = GLFW_KEY_D;
    constexpr int E = GLFW_KEY_E;
    constexpr int F = GLFW_KEY_F;
    constexpr int G = GLFW_KEY_G;
    constexpr int H = GLFW_KEY_H;
    constexpr int I = GLFW_KEY_I;
    constexpr int J = GLFW_KEY_J;
    constexpr int K = GLFW_KEY_K;
    constexpr int L = GLFW_KEY_L;
    constexpr int M = GLFW_KEY_M;
    constexpr int N = GLFW_KEY_N;
    constexpr int O = GLFW_KEY_O;
    constexpr int P = GLFW_KEY_P;
    constexpr int Q = GLFW_KEY_Q;
    constexpr int R = GLFW_KEY_R;
    constexpr int S = GLFW_KEY_S;
    constexpr int T = GLFW_KEY_T;
    constexpr int U = GLFW_KEY_U;
    constexpr int V = GLFW_KEY_V;
    constexpr int W = GLFW_KEY_W;
    constexpr int X = GLFW_KEY_X;
    constexpr int Y = GLFW_KEY_Y;
    constexpr int Z = GLFW_KEY_Z;
    constexpr int LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
    constexpr int BACKSLASH = GLFW_KEY_BACKSLASH;
    constexpr int RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
    constexpr int GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
    
    constexpr int ESCAPE = GLFW_KEY_ESCAPE;
    constexpr int ENTER = GLFW_KEY_ENTER;
    constexpr int TAB = GLFW_KEY_TAB;
    constexpr int BACKSPACE = GLFW_KEY_BACKSPACE;
    constexpr int INSERT = GLFW_KEY_INSERT;
    constexpr int DELETE = GLFW_KEY_DELETE;
    constexpr int RIGHT = GLFW_KEY_RIGHT;
    constexpr int LEFT = GLFW_KEY_LEFT;
    constexpr int DOWN = GLFW_KEY_DOWN;
    constexpr int UP = GLFW_KEY_UP;
    constexpr int PAGE_UP = GLFW_KEY_PAGE_UP;
    constexpr int PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
    constexpr int HOME = GLFW_KEY_HOME;
    constexpr int END = GLFW_KEY_END;
    constexpr int CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
    constexpr int SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
    constexpr int NUM_LOCK = GLFW_KEY_NUM_LOCK;
    constexpr int PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
    constexpr int PAUSE = GLFW_KEY_PAUSE;
    constexpr int F1 = GLFW_KEY_F1;
    constexpr int F2 = GLFW_KEY_F2;
    constexpr int F3 = GLFW_KEY_F3;
    constexpr int F4 = GLFW_KEY_F4;
    constexpr int F5 = GLFW_KEY_F5;
    constexpr int F6 = GLFW_KEY_F6;
    constexpr int F7 = GLFW_KEY_F7;
    constexpr int F8 = GLFW_KEY_F8;
    constexpr int F9 = GLFW_KEY_F9;
    constexpr int F10 = GLFW_KEY_F10;
    constexpr int F11 = GLFW_KEY_F11;
    constexpr int F12 = GLFW_KEY_F12;
    constexpr int KP_0 = GLFW_KEY_KP_0;
    constexpr int KP_1 = GLFW_KEY_KP_1;
    constexpr int KP_2 = GLFW_KEY_KP_2;
    constexpr int KP_3 = GLFW_KEY_KP_3;
    constexpr int KP_4 = GLFW_KEY_KP_4;
    constexpr int KP_5 = GLFW_KEY_KP_5;
    constexpr int KP_6 = GLFW_KEY_KP_6;
    constexpr int KP_7 = GLFW_KEY_KP_7;
    constexpr int KP_8 = GLFW_KEY_KP_8;
    constexpr int KP_9 = GLFW_KEY_KP_9;
    constexpr int KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
    constexpr int KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
    constexpr int KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
    constexpr int KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
    constexpr int KP_ADD = GLFW_KEY_KP_ADD;
    constexpr int KP_ENTER = GLFW_KEY_KP_ENTER;
    constexpr int KP_EQUAL = GLFW_KEY_KP_EQUAL;
    constexpr int LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
    constexpr int LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
    constexpr int LEFT_ALT = GLFW_KEY_LEFT_ALT;
    constexpr int LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
    constexpr int RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
    constexpr int RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
    constexpr int RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
    constexpr int RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
    constexpr int MENU = GLFW_KEY_MENU;
}

namespace MouseButtons {
    constexpr int LEFT = GLFW_MOUSE_BUTTON_LEFT;
    constexpr int RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
    constexpr int MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
    constexpr int BUTTON_4 = GLFW_MOUSE_BUTTON_4;
    constexpr int BUTTON_5 = GLFW_MOUSE_BUTTON_5;
    constexpr int BUTTON_6 = GLFW_MOUSE_BUTTON_6;
    constexpr int BUTTON_7 = GLFW_MOUSE_BUTTON_7;
    constexpr int BUTTON_8 = GLFW_MOUSE_BUTTON_8;
}

namespace MouseAxes {
    constexpr int X = 0;
    constexpr int Y = 1;
    constexpr int SCROLL_X = 2;
    constexpr int SCROLL_Y = 3;
}

namespace GamepadButtons {
    constexpr int A = GLFW_GAMEPAD_BUTTON_A;
    constexpr int B = GLFW_GAMEPAD_BUTTON_B;
    constexpr int X = GLFW_GAMEPAD_BUTTON_X;
    constexpr int Y = GLFW_GAMEPAD_BUTTON_Y;
    constexpr int LEFT_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
    constexpr int RIGHT_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
    constexpr int BACK = GLFW_GAMEPAD_BUTTON_BACK;
    constexpr int START = GLFW_GAMEPAD_BUTTON_START;
    constexpr int GUIDE = GLFW_GAMEPAD_BUTTON_GUIDE;
    constexpr int LEFT_THUMB = GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
    constexpr int RIGHT_THUMB = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
    constexpr int DPAD_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP;
    constexpr int DPAD_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
    constexpr int DPAD_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
    constexpr int DPAD_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
}

namespace GamepadAxes {
    constexpr int LEFT_X = GLFW_GAMEPAD_AXIS_LEFT_X;
    constexpr int LEFT_Y = GLFW_GAMEPAD_AXIS_LEFT_Y;
    constexpr int RIGHT_X = GLFW_GAMEPAD_AXIS_RIGHT_X;
    constexpr int RIGHT_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y;
    constexpr int LEFT_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER;
    constexpr int RIGHT_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;
}

namespace GamepadIDs {
    constexpr int GAMEPAD_1 = GLFW_JOYSTICK_1;
    constexpr int GAMEPAD_2 = GLFW_JOYSTICK_2;
    constexpr int GAMEPAD_3 = GLFW_JOYSTICK_3;
    constexpr int GAMEPAD_4 = GLFW_JOYSTICK_4;
    constexpr int GAMEPAD_5 = GLFW_JOYSTICK_5;
    constexpr int GAMEPAD_6 = GLFW_JOYSTICK_6;
    constexpr int GAMEPAD_7 = GLFW_JOYSTICK_7;
    constexpr int GAMEPAD_8 = GLFW_JOYSTICK_8;
    constexpr int GAMEPAD_9 = GLFW_JOYSTICK_9;
    constexpr int GAMEPAD_10 = GLFW_JOYSTICK_10;
    constexpr int GAMEPAD_11 = GLFW_JOYSTICK_11;
    constexpr int GAMEPAD_12 = GLFW_JOYSTICK_12;
    constexpr int GAMEPAD_13 = GLFW_JOYSTICK_13;
    constexpr int GAMEPAD_14 = GLFW_JOYSTICK_14;
    constexpr int GAMEPAD_15 = GLFW_JOYSTICK_15;
    constexpr int GAMEPAD_16 = GLFW_JOYSTICK_16;
}

namespace KeyMods {
    constexpr int SHIFT = GLFW_MOD_SHIFT;
    constexpr int CONTROL = GLFW_MOD_CONTROL;
    constexpr int ALT = GLFW_MOD_ALT;
    constexpr int SUPER = GLFW_MOD_SUPER;
    constexpr int CAPS_LOCK = GLFW_MOD_CAPS_LOCK;
    constexpr int NUM_LOCK = GLFW_MOD_NUM_LOCK;
}

} // namespace input
} // namespace voidengine 