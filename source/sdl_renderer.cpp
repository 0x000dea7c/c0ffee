#include "maths.h"
#include "physics.h"
#include "renderer.h"
#include "entity_manager.h"
#include <SDL3/SDL.h>

using namespace coffee;

static constexpr Vector2 ship_vertices[3] = {
    // Local coordinates, a triangle.
    Vector2( 50.0f,   0.0f), // Nose (forward).
    Vector2(-20.0f,  25.0f), // Left base.
    Vector2(-20.0f, -25.0f), // Right base.
};

static constexpr Vector2 rock_vertices[5] = { // @TODO: Have random values here?
    // Local coordinates, a pentagon.
    Vector2( 30.0f,  0.0f), // Top.
    Vector2( 0.0f, -25.0f), // Top-left.
    Vector2(-25.0f, -5.0f), // Bottom-left.
    Vector2(-10.0f, 20.0f), // Bottom-right.
    Vector2( 20.0f, 25.0f), // Top-right.
};

static constexpr s32 rock_indices[9] = {
    0, 1, 2, // Top, Top-right, Bottom-right.
    0, 2, 3, // Top, Bottom-right, Bottom-left.
    0, 3, 4  // Top, Bottom-left, Top-left.
};

void
coffee::render(void* renderer, World& world, Renderer_Data& renderer_data, Entity_Manager& entity_manager)
{
    SDL_Renderer* r = reinterpret_cast<SDL_Renderer*>(renderer);
    static u64 const player_id = entity_manager.get_player_id();

    SDL_SetRenderDrawColor(r, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(r);

    {
        // Render the rocks.
        SDL_Vertex vertices[5];

        // @HACK: we're starting at 1 because the player id is 0. This is hacky.
        for (u64 i = 1; i < renderer_data._transforms.size(); ++i) {
            renderer_data._transforms[i].set_translation(world._positions[i]);
            renderer_data._transforms[i].set_orientation(world._orientations[i]);

            for (u64 j = 0; j < 5; ++j) {
                Vector2 const world_position = renderer_data._transforms[i].transform_vertex(rock_vertices[j]);
                vertices[j].position.x = world_position._x;
                vertices[j].position.y = world_position._y;
                vertices[j].color = { 200, 200, 200, 255 };
                vertices[j].tex_coord = { 0.0f, 0.0f };
            }

            SDL_RenderGeometry(r, nullptr, vertices, 5, rock_indices, 9);
        }
    }

    {
        // Render Player's spaceship.
        renderer_data._transforms[player_id].set_translation(world._positions[player_id]);
        renderer_data._transforms[player_id].set_orientation(world._orientations[player_id]);

        SDL_Vertex vertices[3];

        for (u64 j = 0; j < 3; ++j) {
            Vector2 const world_position = renderer_data._transforms[player_id].transform_vertex(ship_vertices[j]);
            vertices[j].position.x = world_position._x;
            vertices[j].position.y = world_position._y;
            vertices[j].color = {255, 255, 255, 255};
            vertices[j].tex_coord = {0.0f, 0.0f};
        }

        SDL_RenderGeometry(r, nullptr, vertices, 3, nullptr, 0);
    }

    SDL_RenderPresent(r);
}
