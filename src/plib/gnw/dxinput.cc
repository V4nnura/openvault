#include "plib/gnw/dxinput.h"

#include "plib/gnw/debug.h"
#include "plib/gnw/mouse.h"
#include "plib/gnw/svga.h"
#include <SDL.h>
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

namespace fallout {

static bool dxinput_mouse_init();
static void dxinput_mouse_exit();
static bool dxinput_keyboard_init();
static void dxinput_keyboard_exit();

// Track previous mouse position for calculating deltas in non-relative mode
static int gMouseWheelDeltaX = 0;
static int gMouseWheelDeltaY = 0;
static int gMousePrevX = 0;
static int gMousePrevY = 0;
static bool gMousePrevInitialized = false;

#if defined(__APPLE__) && TARGET_OS_IOS
static bool last_input_was_mouse = false;
static int last_system_x = -1;
static int last_system_y = -1;
static Uint32 last_mouse_buttons = 0;
#endif

// 0x4E0400
bool dxinput_init()
{
    SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");
    if (SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) {
        return false;
    }

    if (!dxinput_mouse_init()) {
        goto err;
    }

    if (!dxinput_keyboard_init()) {
        goto err;
    }

    return true;

err:

    dxinput_mouse_exit();

    return false;
}

// 0x4E0478
void dxinput_exit()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

// 0x4E04E8
bool dxinput_acquire_mouse()
{
    return true;
}

// 0x4E0514
bool dxinput_unacquire_mouse()
{
    return true;
}

// 0x4E053C
bool dxinput_get_mouse_state(MouseData* mouseState)
{
    // CE: This function is sometimes called outside loops calling `get_input`
    // and subsequently `GNW95_process_message`, so mouse events might not be
    // handled by SDL yet.
    SDL_PumpEvents();

#if defined(__ANDROID__) && defined(__APPLE__) && TARGET_OS_IOS
    int system_x, system_y;
    Uint32 mouse_buttons = SDL_GetMouseState(&system_x, &system_y);

    bool mouse_activity = false;
    if (last_system_x == -1 && last_system_y == -1) {
        last_system_x = system_x;
        last_system_y = system_y;
        last_mouse_buttons = mouse_buttons;
    }

    if (system_x != last_system_x || system_y != last_system_y || mouse_buttons != last_mouse_buttons) {
        mouse_activity = true;
    }

    if (mouse_activity) {
        last_input_was_mouse = true;
    }

    last_system_x = system_x;
    last_system_y = system_y;
    last_mouse_buttons = mouse_buttons;

    if (last_input_was_mouse) {
        int game_x, game_y;
        mouse_get_position(&game_x, &game_y);

        float logical_x, logical_y;
        SDL_RenderWindowToLogical(gSdlRenderer, system_x, system_y, &logical_x, &logical_y);

        int mapped_x = (int)logical_x;
        int mapped_y = (int)logical_y;

        if (mapped_x < 0) mapped_x = 0;
        if (mapped_x >= screenGetWidth()) mapped_x = screenGetWidth() - 1;
        if (mapped_y < 0) mapped_y = 0;
        if (mapped_y >= screenGetHeight()) mapped_y = screenGetHeight() - 1;

        int delta_x = mapped_x - game_x;
        int delta_y = mapped_y - game_y;

        if (mapped_x >= 0 && mapped_x < screenGetWidth() && mapped_y >= 0 && mapped_y < screenGetHeight()) {
            mouseState->x = delta_x;
            mouseState->y = delta_y;
        } else {
            mouseState->x = 0;
            mouseState->y = 0;
        }
    } else {
        mouseState->x = 0;
        mouseState->y = 0;
    }

    mouseState->buttons[0] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    mouseState->buttons[1] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    mouseState->wheelX = gMouseWheelDeltaX;
    mouseState->wheelY = gMouseWheelDeltaY;
    gMouseWheelDeltaX = 0;
    gMouseWheelDeltaY = 0;
    return true;
#endif

    // Get absolute window mouse position
    Uint32 buttons = SDL_GetMouseState(&(mouseState->x), &(mouseState->y));

    // Convert window coordinates to game (logical) coordinates
    // This handles scaling automatically via SDL's renderer
    float logicalX, logicalY;
    if (gSdlRenderer != NULL) {
        SDL_RenderWindowToLogical(gSdlRenderer, (mouseState->x), (mouseState->y), &logicalX, &logicalY);
    } else {
        logicalX = (float)(mouseState->x);
        logicalY = (float)(mouseState->y);
    }

    int gameX = (int)logicalX;
    int gameY = (int)logicalY;

    // Store absolute position in game coordinates
    mouseState->absX = gameX;
    mouseState->absY = gameY;

    // Calculate deltas in game coordinates for VCR compatibility
    if (!gMousePrevInitialized) {
        gMousePrevX = gameX;
        gMousePrevY = gameY;
        gMousePrevInitialized = true;
    }

    mouseState->x = gameX - gMousePrevX;
    mouseState->y = gameY - gMousePrevY;
    gMousePrevX = gameX;
    gMousePrevY = gameY;
    mouseState->buttons[0] = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    mouseState->buttons[1] = (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    mouseState->wheelX = gMouseWheelDeltaX;
    mouseState->wheelY = gMouseWheelDeltaY;

    gMouseWheelDeltaX = 0;
    gMouseWheelDeltaY = 0;

    return true;
}

// 0x4E05A8
bool dxinput_acquire_keyboard()
{
    return true;
}

// 0x4E05D4
bool dxinput_unacquire_keyboard()
{
    return true;
}

// 0x4E05FC
bool dxinput_flush_keyboard_buffer()
{
    SDL_FlushEvents(SDL_KEYDOWN, SDL_TEXTINPUT);
    return true;
}

// 0x4E0650
bool dxinput_read_keyboard_buffer(KeyboardData* keyboardData)
{
    return true;
}

// 0x4E070C
bool dxinput_mouse_init()
{
#if defined(__APPLE__) && TARGET_OS_IOS
    return true;
#else
    SDL_ShowCursor(SDL_DISABLE);
    return true;
#endif
}

// 0x4E078C
void dxinput_mouse_exit()
{
}

// 0x4E07B8
bool dxinput_keyboard_init()
{
    return true;
}

// 0x4E0874
void dxinput_keyboard_exit()
{
}

void handleMouseEvent(SDL_Event* event)
{
    // Mouse movement and buttons are accumulated in SDL itself and will be
    // processed later in `mouseDeviceGetData` via `SDL_GetRelativeMouseState`.

    if (event->type == SDL_MOUSEWHEEL) {
        gMouseWheelDeltaX += event->wheel.x;
        gMouseWheelDeltaY += event->wheel.y;
    }
}


void dxinput_notify_mouse()
{
#if defined(__ANDROID__) && defined(__APPLE__) && TARGET_OS_IOS
    last_input_was_mouse = true;
#endif
}

void dxinput_notify_touch()
{
#if defined(__ANDROID__) && defined(__APPLE__) && TARGET_OS_IOS
    last_input_was_mouse = false;
#endif
}

} // namespace fallout
