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

    MainGame(Window *window, Mouse *mouse, Keyboard *keyboard);

    void setMinimapPort();
    void setWorldPort();

    void setMap();
    void initPlayers();
    void initEnemies();

    void renderMinimap(float dt);
    void renderWorld(float dt);

    void debugging();

    void cleanup();


private:
    Window *window;
    Mouse *mouse;
    Keyboard *keyboard;
    
    Texture *background;
    Texture *wall;

    
    Map map;
    Player player;

    SDL_Rect VPfiller;
    SDL_Rect VPminimap;
    SDL_Rect VPworld;

    SDL_Rect tempSquare;

};


#endif