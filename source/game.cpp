#include "game.h"

#include <cassert>

void add_entity(Add_Entity_Arguments const& args) {
    positions.emplace_back(args.position);
    accelerations.emplace_back(args.acceleration);
    velocities.emplace_back(args.velocity);
    orientations.emplace_back(args.orientation);
    sizes.emplace_back(args.size);
    transforms.emplace_back(args.transform);
    types.emplace_back(args.entity_type);
}

void remove_entity(u64 entity_id) {
    if (entity_id >= positions.size()) {
        log("Invalid entity_id: %u, size: %u\n", entity_id, positions.size());
        assert(false && "How did this happen?");
    }

    u64 last_entity_id = positions.size() - 1;

    if (entity_id == last_entity_id) {
        positions.pop_back();
        accelerations.pop_back();
        velocities.pop_back();
        orientations.pop_back();
        sizes.pop_back();
        transforms.pop_back();
        types.pop_back();
        return;
    }

    // log("Removing entity: %u and swapping it with %u\n", entity_id, last_entity_id);

    std::swap(positions[entity_id], positions[last_entity_id]);
    std::swap(accelerations[entity_id], accelerations[last_entity_id]);
    std::swap(velocities[entity_id], velocities[last_entity_id]);
    std::swap(orientations[entity_id], orientations[last_entity_id]);
    std::swap(sizes[entity_id], sizes[last_entity_id]);
    std::swap(transforms[entity_id], transforms[last_entity_id]);
    std::swap(types[entity_id], types[last_entity_id]);

    positions.pop_back();
    accelerations.pop_back();
    velocities.pop_back();
    orientations.pop_back();
    sizes.pop_back();
    transforms.pop_back();
    types.pop_back();
}
