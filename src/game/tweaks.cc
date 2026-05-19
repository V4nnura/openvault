#include "game/tweaks.h"

#include "game/config.h"
#include "plib/gnw/debug.h"

namespace fallout {

static bool tweaks_initialized = false;
static bool tweak_auto_mouse_mode = false;
static bool tweak_hover_hide_roof = false;
static bool tweak_object_tooltip = false;
static int tweak_highlight_objects_key = 0;

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
            if (config_get_value(&tweaksConfig, "Roof", "HoverHide", &value)) {
                tweak_hover_hide_roof = (value != 0);
            }
            if (config_get_value(&tweaksConfig, "Mouse", "ObjectTooltip", &value)) {
                tweak_object_tooltip = (value != 0);
            }
            if (config_get_value(&tweaksConfig, "Accessibility", "HighlightKey", &value)) {
                tweak_highlight_objects_key = value;
            }

            debug_printf("Tweaks loaded from tweaks.ini\n");
            if (tweak_auto_mouse_mode) {
                debug_printf("  Mouse.AutoMode = 1\n");
            }
            if (tweak_hover_hide_roof) {
                debug_printf("  Roof.HoverHide = 1\n");
            }
            if (tweak_object_tooltip) {
                debug_printf("  Mouse.ObjectTooltip = 1\n");
            }
            if (tweak_highlight_objects_key != 0) {
                debug_printf("  Accessibility.HighlightKey = %d\n", tweak_highlight_objects_key);
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
    tweak_hover_hide_roof = false;
    tweak_object_tooltip = false;
    tweak_highlight_objects_key = 0;
    tweaks_initialized = false;
}

bool tweaks_auto_mouse_mode()
{
    return tweak_auto_mouse_mode;
}

bool tweaks_hover_hide_roof()
{
    return tweak_hover_hide_roof;
}

bool tweaks_object_tooltip()
{
    return tweak_object_tooltip;
}

int tweaks_highlight_objects_key()
{
    return tweak_highlight_objects_key;
}

} // namespace fallout
