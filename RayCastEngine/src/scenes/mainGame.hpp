#ifndef MAINGAME_HPP
#define MAINGAME_HPP

#include "../macros.hpp"
#include "../sdl_engine/window.hpp"
#include "../sdl_engine/sprite.hpp"
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

    int mNumRays;

private:
    Window *window;
    Mouse *mouse;
    Keyboard *keyboard;
    
    Texture *gun1;
    //Texture* texTarget;

    Sprite floorSprite;
    Sprite wallSprite;

    Map *map;
    Player player;

    SDL_Rect VPminimap;
    SDL_Rect VPworld;

    SDL_Rect tempSquare;

    std::vector<vec2> mHitCoords;

};


#endif