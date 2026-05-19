#ifndef FALLOUT_GAME_TWEAKS_H_
#define FALLOUT_GAME_TWEAKS_H_

namespace fallout {

// Initialize tweaks system by loading tweaks.ini from base directory.
// Should be called early during game initialization.
bool tweaks_init();

// Shutdown tweaks system.
void tweaks_exit();

// Returns true if auto mouse mode switching is enabled.
// When enabled, the game automatically switches between MOVE and ARROW
// mouse modes based on what's under the cursor.
bool tweaks_auto_mouse_mode();

// Returns true if hover-to-hide roof is enabled.
// When enabled, roofs are hidden when the mouse cursor hovers over them,
// allowing the player to see inside buildings.
bool tweaks_hover_hide_roof();

// Returns true if object name tooltip is enabled.
// When enabled, hovering over objects displays their name as a tooltip
// near the mouse cursor.
bool tweaks_object_tooltip();

// Returns the SDL scancode for the accessibility highlight key.
// When held, all interactable objects (NPCs, items, exits, doors) are outlined.
// Returns 0 if disabled (not configured in tweaks.ini).
int tweaks_highlight_objects_key();

} // namespace fallout

#endif /* FALLOUT_GAME_TWEAKS_H_ */
