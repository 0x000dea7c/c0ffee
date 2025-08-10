#include "game.h"

void add_entity(Add_Entity_Arguments const& args) {
    positions.emplace_back(args.position);
    accelerations.emplace_back(args.acceleration);
    velocities.emplace_back(args.velocity);
    orientations.emplace_back(args.orientation);
    sizes.emplace_back(args.size);
    transforms.emplace_back(args.transform);
}
