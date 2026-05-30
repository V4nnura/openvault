#include "fps_limiter.h"

#include <SDL.h>

namespace fallout {

FpsLimiter::FpsLimiter(unsigned int fps)
    : _fps(fps)
    , _ticks(0)
    , _enabled(true)
{
}

void FpsLimiter::mark()
{
    _ticks = SDL_GetTicks();
}

void FpsLimiter::throttle() const
{
    if (!_enabled) {
        return;
    }

    if (1000 / _fps > SDL_GetTicks() - _ticks) {
        SDL_Delay(1000 / _fps - (SDL_GetTicks() - _ticks));
    }
}

void FpsLimiter::setFps(unsigned int fps)
{
    _fps = fps > 0 ? fps : 60;
}

void FpsLimiter::setEnabled(bool enabled)
{
    _enabled = enabled;
}

} // namespace fallout
