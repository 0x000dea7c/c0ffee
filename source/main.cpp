#include "constants.h"
#include "maths.h"
#include "physics.h"
#include "renderer.h"
#include "types.h"
#include <SDL3/SDL.h>
#include <cstdlib>
#include "entity_manager.cpp"
#include "physics.cpp"
#include "sdl_renderer.cpp"

using namespace coffee;

struct SDL_Subsystem {
    SDL_Subsystem() {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
            SDL_Log("Could not initialise SDL3: %s\n", SDL_GetError());
            std::exit(1);
        }
    }

    ~SDL_Subsystem() {
        SDL_Log("Releasing SDL3 resources\n");
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    void create_window(char const* title, s32 width, s32 height) {
        _window = SDL_CreateWindow(title, width, height, 0);

        if (!_window) {
            SDL_Log("Could not create SDL window: %s\n", SDL_GetError());
            std::exit(1);
        }

        _renderer = SDL_CreateRenderer(_window, nullptr);
    }

    SDL_Window* _window;
    SDL_Renderer* _renderer;
};

/// The game checks _is_pressed for continuous actions and _transitions for
/// discrete actions. Two half transitions mean a single press in a frame.
struct Button_State {
    Button_State() : _is_pressed(false), _half_transition(0) { }

    b32 _is_pressed;
    u32 _half_transition;
};

struct Player_Input {
    Button_State _move_left;
    Button_State _move_right;
    Button_State _move_up;
    Button_State _action_shoot;
};

struct Player {
    Player(Player_Input const& input) : _input(input) { }

    Player_Input _input;
};

void handle_input(Player& player, World& world, Entity_Manager& entity_manager, Random_Generator& random_generator, f32 delta_time)
{
    static u64 const player_id = entity_manager.get_player_id();

    // Poll the keyboard state, it's supposed to be faster this way compared to
    // checking for events. @TODO: I should benchmark this.
    bool const* keys = SDL_GetKeyboardState(nullptr);

    // Continuous actions.
    player._input._move_left._is_pressed = keys[SDL_SCANCODE_LEFT];
    player._input._move_right._is_pressed = keys[SDL_SCANCODE_RIGHT];
    player._input._move_up._is_pressed = keys[SDL_SCANCODE_UP];

    if (player._input._move_left._is_pressed) {
        world._orientations[player_id] += player_rotation_factor * delta_time;
    }

    if (player._input._move_right._is_pressed) {
        world._orientations[player_id] -= player_rotation_factor * delta_time;
    }

    if (player._input._move_up._is_pressed) {
        // NOTE: in SDL3, +y is down, so we need to invert the y coordinate!
        Vector2 const forward = rotate_vector({ 1.0f, 0.0f }, -world._orientations[player_id]);
        world._accelerations[player_id]._x = (player_thrust_force * forward._x) / player_mass_kg;
        world._accelerations[player_id]._y = (player_thrust_force * forward._y) / player_mass_kg;
    } else {
        world._accelerations[player_id]._x = 0.0f;
        world._accelerations[player_id]._y = 0.0f;
    }

    // Discrete actions.
    if (keys[SDL_SCANCODE_SPACE] != player._input._action_shoot._is_pressed) {
        ++player._input._action_shoot._half_transition;
        player._input._action_shoot._is_pressed = keys[SDL_SCANCODE_SPACE];
    }

    if (player._input._action_shoot._half_transition > 0 && player._input._action_shoot._is_pressed) {
        player._input._action_shoot._half_transition = 0;
    }
}

void spawn_rock(World& world, Entity_Manager& entity_manager, Random_Generator& random_generator) {
    static u64 const player_id = entity_manager.get_player_id();

    auto const rock_spawns_in_safe_position = [](Vector2 const& from, Vector2 const& to) {
        return euclidean_distance_f32(from, to) > rock_spawn_safe_distance_to_player;
    };

    Vector2 rock_position;

    do {
        rock_position._x = random_generator.get_random_f32(0, window_width);
        rock_position._y = random_generator.get_random_f32(0, window_height);
    } while (!rock_spawns_in_safe_position(rock_position, world._positions[player_id]));

    // Make the rock point to the player's space-ship.
    Vector2 rock_direction (rock_position);
    rock_direction = world._positions[player_id] - rock_position;
    rock_direction.normalise();

    Vector2 rock_acceleration(rock_direction);
    rock_acceleration = rock_acceleration * 100.0f;

    Add_Entity_World_Arguments const rock_arguments = {
        ._position = rock_position,
        ._velocity = Vector2(),
        ._acceleration = rock_acceleration,
        ._orientation_radians = random_generator.get_random_f32(0.0f, 2.0f * pi)
    };

    entity_manager.add_rock(rock_arguments);
}

int main()
{
    char const* window_title = "Coffee";
    SDL_Subsystem sdl_subsystem;

    sdl_subsystem.create_window(window_title, window_width, window_height);

    SDL_Event event;
    bool quit = false;
    Player_Input input;
    Player player(input);
    World world;
    Renderer_Data renderer_data;
    Entity_Manager entity_manager(world, renderer_data);
    Add_Entity_World_Arguments player_spawn_arguments = {
        ._position = Vector2(window_width / 2.0f, window_height / 2.0f),
        ._velocity = {},
        ._acceleration = {},
        ._orientation_radians = 0.0f
    };
    Random_Generator random_generator(SDL_GetTicks());

    entity_manager.add_player(player_spawn_arguments);

    f32 constexpr fixed_delta_time = 1.0f / 60.0f;
    f32 accumulator = 0.0f;
    u64 last_time = SDL_GetTicks();
    u64 rock_spawn_time = SDL_GetTicks();

    while (!quit) {
        u64 current_time = SDL_GetTicks();
        f32 delta_time = static_cast<f32>(current_time - last_time) / 1000.0f;
        last_time = current_time;

        // Avoid the spiral of death. @TODO: Research.
        if (delta_time > 0.25f) {
            delta_time = 0.25f;
        }

        accumulator += delta_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
                break;
            }
        }

        // Game logic.
        if (rock_spawn_time < current_time) {
            spawn_rock(world, entity_manager, random_generator);
            rock_spawn_time = current_time + random_generator.get_random_s32(1000, 3000);
        }

        // Fixed time step physics updates.
        while (accumulator >= fixed_delta_time) {
            handle_input(player, world, entity_manager, random_generator, delta_time);
            update(delta_time, world);
            accumulator -= fixed_delta_time;
        }

        render(sdl_subsystem._renderer, world, renderer_data, entity_manager);

        // Try to avoid CPU overuse.
        u64 const end_frame_time = SDL_GetTicks();
        f32 elapsed_ms = static_cast<f32>(end_frame_time - current_time);
        f32 target_ms = 1000.0f / 60.0f;

        if (elapsed_ms < target_ms) {
            SDL_Delay(static_cast<u32>(target_ms - elapsed_ms));
        }
    }

    return 0;
}
