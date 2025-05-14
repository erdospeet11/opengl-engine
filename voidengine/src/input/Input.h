#pragma once

#include "InputSystem.h"
#include "InputMapping.h"
#include "InputConstants.h"

namespace voidengine {

using input::ButtonState;
using input::EventType;
using input::InputEvent;
using input::InputCallback;
using input::InputSystem;
using input::InputType;
using input::InputBinding;
using input::ActionState;
using input::ActionCallback;
using input::InputMapping;

using input::gInputSystem;
using input::gInputMapping;

using input::initializeInputSystem;
using input::initializeInputMapping;
using input::shutdownInputSystem;

namespace Keys = input::Keys;
namespace MouseButtons = input::MouseButtons;
namespace MouseAxes = input::MouseAxes;
namespace GamepadButtons = input::GamepadButtons;
namespace GamepadAxes = input::GamepadAxes;
namespace GamepadIDs = input::GamepadIDs;
namespace KeyMods = input::KeyMods;

} // namespace voidengine 