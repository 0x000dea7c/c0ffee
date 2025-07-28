#include "constants.h"
#include "physics.cpp"
#include "renderer.h"
#include "sdl_renderer.cpp"
#include "types.h"
#include "maths.h"
#include <SDL3/SDL.h>
#include <cstdlib>

using namespace coffee;

/// RAII for SDL3 resources
struct SDL_Subsystem
{
    SDL_Subsystem()
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
            SDL_Log("Could not initialise SDL3: %s\n", SDL_GetError());
            std::exit(1);
        }
    }

    ~SDL_Subsystem()
    {
        SDL_Log("Releasing SDL3 resources\n");
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    void create_window(char const* title, s32 width, s32 height)
    {
        _window = SDL_CreateWindow(title, width, height, 0);

        if (!_window) {
            SDL_Log("Could not create SDL window: %s\n", SDL_GetError());
        }

        _renderer = SDL_CreateRenderer(_window, nullptr);
    }

    SDL_Window* _window;
    SDL_Renderer* _renderer;
};

/// The game checks _is_pressed for continuous actions and _transitions for
/// discrete actions. Two half transitions mean a single press in a frame.
struct Button_State
{
    Button_State() : _is_pressed{ false }, _half_transition{ 0 }
    {
    }

    b32 _is_pressed;
    u32 _half_transition;
};

struct Player_Input
{
    Button_State _move_left;
    Button_State _move_right;
    Button_State _move_up;
    Button_State _action_shoot;
};

struct Player
{
    Player(Player_Input const& input) : _input(input), _health(100)
    {
    }

    Player_Input _input;
    u32 _health;
};

void
handle_input(Player& player, World& world, f32 delta_time)
{
    // Poll the keyboard state, it's supposed to be faster this way compared to
    // checking for events. TODO: I should benchmark this.
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
        Vector2 const forward = rotate_vector({1.0f, 0.0f}, -world._orientations[player_id]);
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

int
main()
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

    // Add the player at the center.
    world.add_entity(Vector2(window_width / 2.0f, window_height / 2.0f),
                     Vector2(0.0f, 0.0f),
                     Vector2(0.0f, 0.0f),
                     0.0f);

    // Add the player to the renderer.
    renderer_data.add_entity();

    u64 last_time = SDL_GetTicks();
    f32 const fixed_delta_time = 1.0f / 60.0f;
    f32 accumulator = 0.0f;

    while (!quit) {
        u64 current_time = SDL_GetTicks();
        f32 delta_time = static_cast<f32>(current_time - last_time) / 1000.0f;
        last_time = current_time;

        if (delta_time > 0.25f) {
            // To avoid the spiral of death. @TODO: Research.
            delta_time = 0.25f;
        }

        accumulator += delta_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
                break;
            }
        }

        while (accumulator >= fixed_delta_time) {
            // Fixed time step physics updates.
            handle_input(player, world, delta_time);
            update(delta_time, world);
            accumulator -= fixed_delta_time;
        }

        render(sdl_subsystem._renderer, world, renderer_data);

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
