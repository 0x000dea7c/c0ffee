#include "game.h"

void add_entity(Add_Entity_Arguments const& args) {
    positions.emplace_back(args.position);
    accelerations.emplace_back(args.acceleration);
    velocities.emplace_back(args.velocity);
    orientations.emplace_back(args.orientation);
    sizes.emplace_back(args.size);
    transforms.emplace_back(args.transform);
}

void remove_entity(u64 entity_id) {
    if(positions.size() == 2) {
        // There's only the rock that was deleted and the player. In
        // this case we only need to pop.
        positions.pop_back();
        accelerations.pop_back();
        velocities.pop_back();
        orientations.pop_back();
        sizes.pop_back();
        transforms.pop_back();
        return;
    }

    std::swap(positions[entity_id], positions[positions.size() - 1]);
    std::swap(accelerations[entity_id], accelerations[accelerations.size() - 1]);
    std::swap(velocities[entity_id], velocities[velocities.size() - 1]);
    std::swap(orientations[entity_id], orientations[orientations.size() - 1]);
    std::swap(sizes[entity_id], sizes[sizes.size() - 1]);
    std::swap(transforms[entity_id], transforms[transforms.size() - 1]);

    positions.pop_back();
    accelerations.pop_back();
    velocities.pop_back();
    orientations.pop_back();
    sizes.pop_back();
    transforms.pop_back();
}
