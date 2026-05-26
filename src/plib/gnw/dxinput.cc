#include "plib/gnw/dxinput.h"

#include "plib/gnw/debug.h"
#include "plib/gnw/mouse.h"
#include "plib/gnw/svga.h"

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

// 0x4E0400
bool dxinput_init()
{
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
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
    SDL_ShowCursor(SDL_DISABLE);
    return true;
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

} // namespace fallout
