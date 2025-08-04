#include "entity_manager.h"
#include "physics.h"
#include "renderer.h"
#include "constants.h"
#include <cassert>

using namespace coffee;

Entity_Manager::Entity_Manager(World& w, Renderer_Data& r)
    : world(w), renderer_data(r) {
    entities.reserve(game_maximum_entities);
    free_ids.reserve(game_maximum_entities);

    for (u64 i = 0; i < game_maximum_entities; ++i) {
        free_ids.push_back(i);
    }
}

void Entity_Manager::add_player(Add_Entity_World_Arguments const& arguments) {
    u64 const player_id = free_ids.back();

    free_ids.pop_back();

    entities.push_back(player_id);

    world.add_entity(arguments);

    renderer_data.add_entity();
}

void Entity_Manager::add_rock(Add_Entity_World_Arguments const& arguments) {
    assert(!free_ids.empty());

    u64 const rock_id = free_ids.back();

    free_ids.pop_back();

    entities.push_back(rock_id);

    world.add_entity(arguments);

    renderer_data.add_entity();
}
