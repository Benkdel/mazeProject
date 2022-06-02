#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "player.hpp"
#include "map.hpp"

bool map_collision(vec2 pos, Map *map, SDL_Rect *port, Window *w);
bool map_collision_2(vec2 pos, Map *map, SDL_Rect *port, Window *w);

#endif
