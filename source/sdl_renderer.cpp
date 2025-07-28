#include "maths.h"
#include "physics.h"
#include "renderer.h"
#include <SDL3/SDL.h>

using namespace coffee;

static constexpr Vector2 ship_vertices[3] = {
    // Local coordinates.
    Vector2( 50.0f,   0.0f),  // Nose (forward).
    Vector2(-20.0f,  25.0f),  // Left base.
    Vector2(-20.0f, -25.0f),  // Right base.
};

void
coffee::render(void* renderer, World& world, Renderer_Data& renderer_data)
{
    SDL_Renderer* r = reinterpret_cast<SDL_Renderer*>(renderer);

    SDL_SetRenderDrawColor(r, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(r);

    for (u64 i = 0; i < renderer_data._transforms.size(); ++i) {
        renderer_data._transforms[i].set_translation(world._positions[i]);
        renderer_data._transforms[i].set_orientation(world._orientations[i]);
    }

    for (u64 i = 0; i < renderer_data._transforms.size(); ++i) {
        SDL_Vertex vertices[3];

        for (u64 j = 0; j < 3; ++j) {
            Vector2 world_position = renderer_data._transforms[i].transform_vertex(ship_vertices[j]);
            vertices[j].position.x = world_position._x;
            vertices[j].position.y = world_position._y;
            vertices[j].color = {255, 255, 255, 255};
            vertices[j].tex_coord = {0.0f, 0.0f};
        }

        SDL_RenderGeometry(r, nullptr, vertices, 3, nullptr, 0);
    }

    SDL_RenderPresent(r);
}
