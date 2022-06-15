#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "../entities/player.hpp"
#include "../worlds/map.hpp"

vec2 map_collision(vec2 pos, Map *map);
vec2 map_collision_2(vec2 pos, std::vector<vec2> box, Map *map);

#endif
