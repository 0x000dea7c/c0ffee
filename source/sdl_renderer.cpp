#include "physics.h"
#include "renderer.h"
#include <SDL3/SDL.h>

void
coffee::render(void* renderer, World& world)
{
    SDL_Renderer* r = reinterpret_cast<SDL_Renderer*>(renderer);

    SDL_SetRenderDrawColor(r, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(r);

    SDL_RenderPresent(r);
}
