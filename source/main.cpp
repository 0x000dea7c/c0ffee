#include "constants.h"
#include "physics.cpp"
#include "sdl_renderer.cpp"
#include "types.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <cstdlib>

using namespace coffee;

/// RAII for SDL3 resources
struct SDL_subsystem
{
    SDL_subsystem()
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
            SDL_Log("Could not initialise SDL3: %s\n", SDL_GetError());
            std::exit(1);
        }
    }

    ~SDL_subsystem()
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
struct button_state
{
    button_state() : _is_pressed{ false }, _half_transition{ 0 }
    {
    }

    b32 _is_pressed;
    u32 _half_transition;
};

struct player_input
{
    button_state _move_left;
    button_state _move_right;
    button_state _move_up;
    button_state _action_shoot;
};

struct player
{
    player(player_input const& input)
      : _input{ input }, _health{ 100 }, _thrust{ 0.0f }
    {
    }

    player_input _input;
    f32 _thrust;
    u32 _health;
};

void
handle_input(player& player, world& world, f32 delta_time)
{
    // Poll the keyboard state, it's supposed to be faster this way compared to
    // checking for events. TODO: I should benchmark this.
    bool const* keys = SDL_GetKeyboardState(nullptr);

    // Continuous actions.
    player._input._move_left._is_pressed = keys[SDL_SCANCODE_LEFT];
    player._input._move_right._is_pressed = keys[SDL_SCANCODE_RIGHT];
    player._input._move_up._is_pressed = keys[SDL_SCANCODE_UP];

    if (player._input._move_left._is_pressed) {
        // TODO:
    }

    if (player._input._move_right._is_pressed) {
        // TODO:
    }

    if (player._input._move_up._is_pressed) {
        player._thrust = player_thrust_factor;
        world._velocities[player_id]._x += cosf(world._rotations[player_id]) * player._thrust;
        world._velocities[player_id]._y += sinf(world._rotations[player_id]) * player._thrust;
    } else {
        world._velocities[player_id]._x = 0.0f;
        world._velocities[player_id]._y = 0.0f;
        player._thrust = 0.0f;
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
    char const* window_title{ "Coffee" };
    SDL_subsystem sdl_subsystem;

    sdl_subsystem.create_window(window_title, window_width, window_height);

    SDL_Event event;
    bool quit{ false };
    u64 last_time{ SDL_GetTicks() };
    player_input input;
    player player(input);
    world world;

    // Add the player at the center
    world.add_entity(Vector2{ window_width / 2.0f, window_height / 2.0f },
                     Vector2{ 0.0f, 0.0f },
                     0.0f);

    while (!quit) {
        f32 delta_time = SDL_GetTicks() - last_time;
        last_time = SDL_GetTicks();

        handle_input(player, world, delta_time);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
                break;
            }
        }

        update(delta_time, world);

        render(sdl_subsystem._renderer, world);

        SDL_Delay(16);
    }

    return 0;
}
