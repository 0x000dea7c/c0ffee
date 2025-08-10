#include <SDL3/SDL.h>
#include <cstdlib>
#include <vector>
#include "types.h"
#include "maths.h"
#include "constants.h"

#include "game.cpp"
#include "random.cpp"

void log(char const* fmt, ...) {
    std::vector<char> buffer(512);

    va_list args;
    va_start(args, fmt);

    s32 result = vsnprintf(buffer.data(), buffer.size(), fmt, args);

    if(result >= static_cast<s32>(buffer.size())) {
        return;
    }

    va_end(args);

    SDL_Log("%s", buffer.data());
}

void initialise() {
    initialise_random_system(SDL_GetTicks());
    positions.reserve(game_maximum_entities);
    accelerations.reserve(game_maximum_entities);
    velocities.reserve(game_maximum_entities);
    orientations.reserve(game_maximum_entities);
    sizes.reserve(game_maximum_entities);
    transforms.reserve(game_maximum_entities);
}

void handle_input(f32 delta_time, Player& player) {
    bool const* keys = SDL_GetKeyboardState(nullptr);

    player.input.move_left.is_pressed = keys[SDL_SCANCODE_LEFT];
    player.input.move_right.is_pressed = keys[SDL_SCANCODE_RIGHT];
    player.input.move_up.is_pressed = keys[SDL_SCANCODE_UP];

    if(player.input.move_left.is_pressed) {
        orientations[0] = orientations[0] + player_rotation_factor * delta_time;
    }

    if(player.input.move_right.is_pressed) {
        orientations[0] = orientations[0] - player_rotation_factor * delta_time;
    }

    if(player.input.move_up.is_pressed) {
        Vector2 forward(1.0f, 0.0f);
        forward.rotate(-orientations[0]); // @NOTE: in SDL3, +y is down, so we need to invert the y coordinate!
        accelerations[0].x = (player_thrust_force * forward.x) / player_mass_kg;
        accelerations[0].y = (player_thrust_force * forward.y) / player_mass_kg;
    } else {
        accelerations[0].x = accelerations[0].y = 0.0f;
    }
}

static void update_player(f32 delta_time) {
    velocities[0].x *= 1.0 - damping * delta_time;
    velocities[0].y *= 1.0 - damping * delta_time;

    velocities[0].x += accelerations[0].x * delta_time;
    velocities[0].y += accelerations[0].y * delta_time;

    positions[0].x += velocities[0].x * delta_time;
    positions[0].y += velocities[0].y * delta_time;

    if(positions[0].x - sizes[0].width < 0.0f) {
        positions[0].x = sizes[0].width;
    } else if(positions[0].x + sizes[0].width > window_width) {
        positions[0].x = window_width - sizes[0].width;
    }

    if(positions[0].y - sizes[0].height < 0.0f) {
        positions[0].y = sizes[0].height;
    } else if(positions[0].y + sizes[0].height > window_height) {
        positions[0].y = window_height - sizes[0].height;
    }
}

static void update_rocks(f32 delta_time) {
    for(u64 i = 1; i < positions.size(); ++i) {
        velocities[i].x *= 1.0 - damping * delta_time;
        velocities[i].y *= 1.0 - damping * delta_time;

        velocities[i].x += accelerations[i].x * delta_time;
        velocities[i].y += accelerations[i].y * delta_time;

        positions[i].x += velocities[i].x * delta_time;
        positions[i].y += velocities[i].y * delta_time;

        // If the rock is out of bounds, we need to remove it from the
        // system.
        if(positions[i].x - sizes[i].width < 0.0f || positions[i].x + sizes[i].width > window_width) {
            remove_entity(i);
        }

        if(positions[i].y - sizes[i].height < 0.0f || positions[i].y + sizes[i].height > window_height) {
            remove_entity(i);
        }
    }
}

void update(f32 delta_time) {
    update_player(delta_time);
    update_rocks(delta_time);
}

void render(void* renderer_ptr) {
    static constexpr Vector2 ship_vertices[3] = {
        // Local coordinates, a triangle. Are these local coordinates, though?
        Vector2( 50.0f,   0.0f), // Nose (forward).
        Vector2(-20.0f,  25.0f), // Left base.
        Vector2(-20.0f, -25.0f), // Right base.
    };

    static constexpr Vector2 rock_vertices[5] = { // @TODO: Have random values here?
        // Local coordinates, a pentagon. Are these local coordinates, though?
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

    SDL_Renderer* renderer = reinterpret_cast<SDL_Renderer*>(renderer_ptr);

    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(renderer);

    // ---------------------
    // Render the rocks.
    // --------------------
    {
        SDL_Vertex vertices[5];

        for(u64 i = 1; i < transforms.size(); ++i) {
            transforms[i].set_translation(positions[i]);
            transforms[i].set_orientation(orientations[i]);

            for(u64 j = 0; j < 5; ++j) {
                Vector2 const world_position = transforms[i].transform_vertex(rock_vertices[j]);
                vertices[j].position.x = world_position.x;
                vertices[j].position.y = world_position.y;
                vertices[j].color = {200, 200, 200, 255};
                vertices[j].tex_coord = {0.0f, 0.0f};
            }

            SDL_RenderGeometry(renderer, nullptr, vertices, 5, rock_indices, 9);
        }
    }

    // ---------------------
    // Render the player.
    // --------------------
    {
        SDL_Vertex vertices[3];

        transforms[0].set_translation(positions[0]);
        transforms[0].set_orientation(orientations[0]);

        for(u64 j = 0; j < 3; ++j) {
            Vector2 const world_position = transforms[0].transform_vertex(ship_vertices[j]);
            vertices[j].position.x = world_position.x;
            vertices[j].position.y = world_position.y;
            vertices[j].color = {255, 255, 255, 255};
            vertices[j].tex_coord = {0.0f, 0.0f};
        }

        SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
    }

    SDL_RenderPresent(renderer);
}

static void spawn_rock() {
    auto const rock_spawns_in_safe_position = [](Vector2 const& from, Vector2 const& to) {
        return euclidean_distance(from, to) > rock_spawn_safe_distance_to_player;
    };

    Vector2 rock_position;

    do {
        // @TODO: we want to spawn rocks far from the corners so that
        // they don't go out of bounds after spawning.
        rock_position.x = get_random_real_number(0, window_width - 50.0f);
        rock_position.y = get_random_real_number(0, window_height - 50.0f);
    } while(!rock_spawns_in_safe_position(rock_position, positions[0]));

    // Make the rock point to the player's space-ship.
    Vector2 rock_direction(rock_position);
    rock_direction = positions[0] - rock_position;
    rock_direction.normalise();

    Vector2 rock_acceleration(rock_direction);
    rock_acceleration = rock_acceleration * 100.0f;

    OBB rock_size(55.0f, 50.0f);

    Add_Entity_Arguments const rock_arguments = {
        .transform    = Matrix3(),
        .size         = rock_size,
        .position     = rock_position,
        .velocity     = Vector2(),
        .acceleration = rock_acceleration,
        .orientation  = get_random_real_number(0.0f, 2.0f * pi)
    };

    add_entity(rock_arguments);
}

struct SDL_Subsystem {
    SDL_Subsystem() {
        if(!SDL_Init(SDL_INIT_VIDEO)) {
            SDL_Log("Could not initialise SDL3: %s\n", SDL_GetError());
            std::exit(1);
        }
    }

    ~SDL_Subsystem() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void create_window(char const* title, s32 width, s32 height) {
        window = SDL_CreateWindow(title, width, height, 0);

        if(!window) {
            SDL_Log("Could not create SDL window: %s\n", SDL_GetError());
            std::exit(1);
        }

        renderer = SDL_CreateRenderer(window, nullptr);
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
};

int main() {
    SDL_Subsystem sdl_subsystem;

    initialise();
    sdl_subsystem.create_window(game_title, window_width, window_height);

    SDL_Event event;
    bool quit = false;
    Player_Input input;
    Player player(input);
    f32 accumulator = 0.0f;
    u64 last_time = SDL_GetTicks();
    u64 rock_spawn_time = SDL_GetTicks();

    Add_Entity_Arguments player_spawn_arguments = {
        .transform    = Matrix3(),
        .size         = OBB(player_width, player_height),
        .position     = Vector2(window_width / 2.0f, window_height / 2.0f),
        .velocity     = Vector2(),
        .acceleration = Vector2(),
        .orientation  = 0.0f,
    };

    add_entity(player_spawn_arguments);

    while(!quit) {
        u64 const current_time = SDL_GetTicks();
        f32 delta_time = static_cast<f32>(current_time - last_time) / 1000.0f;
        last_time = current_time;

        // Avoid the spiral of death.
        if(delta_time > 0.25f) {
            delta_time = 0.25f;
        }

        accumulator += delta_time;

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                quit = true;
                break;
            }
        }

        if(rock_spawn_time < current_time) {
            spawn_rock();
            rock_spawn_time = current_time + get_random_integer_number(1000, 3000);
        }

        while(accumulator >= fixed_delta_time) {
            handle_input(delta_time, player);
            update(delta_time);
            accumulator -= fixed_delta_time;
        }

        render(sdl_subsystem.renderer);

        // Avoid CPU overuse.
        u64 const end_frame_time = SDL_GetTicks();
        f32 elapsed_ms = static_cast<f32>(end_frame_time - current_time);
        f32 target_ms = 1000.0f / 60.0f;

        if(elapsed_ms < target_ms) {
            SDL_Delay(static_cast<u32>(target_ms - elapsed_ms));
        }
    }

    return 0;
}
