#include "game/tweaks.h"

#include "game/config.h"
#include "plib/gnw/debug.h"

namespace fallout {

static bool tweaks_initialized = false;
static bool tweak_auto_mouse_mode = false;

bool tweaks_init()
{
    if (tweaks_initialized) {
        return true;
    }

    Config tweaksConfig;
    if (config_init(&tweaksConfig)) {
        if (config_load(&tweaksConfig, "tweaks.ini", false)) {
            int value;
            if (config_get_value(&tweaksConfig, "Mouse", "AutoMode", &value)) {
                tweak_auto_mouse_mode = (value != 0);
            }

            debug_printf("Tweaks loaded from tweaks.ini\n");
            if (tweak_auto_mouse_mode) {
                debug_printf("  Mouse.AutoMode = 1\n");
            }
        }
        config_exit(&tweaksConfig);
    }

    tweaks_initialized = true;
    return true;
}

void tweaks_exit()
{
    if (!tweaks_initialized) {
        return;
    }

    tweak_auto_mouse_mode = false;
    tweaks_initialized = false;
}

bool tweaks_auto_mouse_mode()
{
    return tweak_auto_mouse_mode;
}

} // namespace fallout
