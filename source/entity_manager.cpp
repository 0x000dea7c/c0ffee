#include "entity_manager.h"
#include "physics.h"
#include "renderer.h"
#include <cassert>

using namespace coffee;

Entity_Manager::Entity_Manager(World& world, Renderer_Data& renderer_data)
  : _world(world)
  , _renderer_data(renderer_data)
  , _current_entity_id(0)
  , _player_id(-1)
{
    _entities.reserve(1000);
}

void
Entity_Manager::add_player(Add_Entity_World_Arguments const& arguments)
{
    // We blow up if we try adding two players.
    assert(_player_id == -1 && _player_id < 999);

    _player_id = _current_entity_id++;

    _entities.push_back(_player_id);

    _world.add_entity(arguments);

    _renderer_data.add_entity();
}

void
Entity_Manager::add_rock(Add_Entity_World_Arguments const& arguments)
{
    assert(_current_entity_id < 999);

    _entities.push_back(_current_entity_id++);

    _world.add_entity(arguments);

    _renderer_data.add_entity();
}
