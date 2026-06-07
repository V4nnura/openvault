#ifndef FALLOUT_PLIB_GNW_GAMEPAD_H_
#define FALLOUT_PLIB_GNW_GAMEPAD_H_

#include <SDL.h>

#include "plib/gnw/rect.h"
#include "plib/gnw/dxinput.h"

namespace fallout {

extern float leftStickX, leftStickY, rightStickX, rightStickY;

bool GetGamepadMouseState(MouseData* mouseState);
void HandleJoystickDeviceAdded(const SDL_Event& event);
void HandleJoystickDeviceRemoved(const SDL_Event& event);
void HandleControllerDeviceAdded(const SDL_Event& event);
void HandleControllerDeviceRemoved(const SDL_Event& event);
void HandleControllerAxisMotion(const SDL_Event& event);
void ProcessLeftStick();
void ProcessRightStick();

} // namespace fallout

#endif /* FALLOUT_PLIB_GNW_GAMEPAD_H_ */
