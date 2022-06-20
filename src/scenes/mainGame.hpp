#ifndef MAINGAME_HPP
#define MAINGAME_HPP

#include "../macros.hpp"
#include "../sdl_engine/window.hpp"
#include "../sdl_engine/texture.hpp"
#include "../io/keyboard.hpp"
#include "../io/mouse.hpp"
#include "../entities/player.hpp"
#include "../worlds/map.hpp"

#include <map>

class MainGame {
public:

    MainGame(Window *window, Map *map, Mouse *mouse, Keyboard *keyboard);

    void setMinimapPort();
    void setWorldPort();

    void renderMinimap(float dt);
    void renderWorld(float dt);

    void debugging();

    void cleanup();

    inline float getP2Sdistance() { return this->playerToScreenDistance; }

    int mNumRays;

private:
    Window *window;
    Mouse *mouse;
    Keyboard *keyboard;
    
    Texture *background;
    Texture *wall;
    Texture *gun1;
    Texture *floor1;

    float playerToScreenDistance;

    Map *map;
    Player player;

    SDL_Rect VPfiller;
    SDL_Rect VPminimap;
    SDL_Rect VPworld;

    SDL_Rect tempSquare;

};


#endif