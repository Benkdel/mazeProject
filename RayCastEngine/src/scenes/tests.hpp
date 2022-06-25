#ifndef TESTS_HPP
#define TESTS_HPP

#include "../sdl_engine/window.hpp"
#include "../math/myMathLib.hpp"
#include "../worlds/map.hpp"
#include "../io/keyboard.hpp"
#include "../io/mouse.hpp"
#include "../shapes/circle.hpp"
#include "../sdl_engine/sprite.hpp"

#include <stdio.h>
#include <string.h>

class TestsModule {
public:

    TestsModule(Window *window, Mouse *mouse, Keyboard *keyboard);

    void setPort();

    void setMapPerimeter();
    
    void updatePos(float dt);
    void updateAngle(float dt);
    void render(float dt);

    void debugging();

    void cleanup();

    vec2 centerPoint;

    Sprite *mGround;

private:
    Window *window;
    Mouse *mouse;
    Keyboard *keyboard;

    SDL_Rect VPscreen;

    int scr_W, scr_H;

    int pixelW = 4;
    int pixelH = 4;

    float fWorldX = 1000.0f;
    float fWorldY = 1000.0f;
    float fWorldA = 0.0f;

    float fNear = 0.01f;
    float fFar = 0.1f;

    float fFovHalf = CONST_PI / 4.0f;
};



#endif