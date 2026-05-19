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

} // namespace fallout

#endif /* FALLOUT_GAME_TWEAKS_H_ */
