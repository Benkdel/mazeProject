#ifndef TESTS_HPP
#define TESTS_HPP

#include "../sdl_engine/window.hpp"
#include "../math/myMathLib.hpp"
#include "../worlds/map.hpp"
#include "../io/keyboard.hpp"
#include "../io/mouse.hpp"
#include "../shapes/circle.hpp"

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

    line l1;
    vec2 leftLeg;
    vec2 rightLeg;

    Circle circle;
    float radius;

private:
    Window *window;
    Mouse *mouse;
    Keyboard *keyboard;

    Map map;

    SDL_Rect VPscreen;

    int scr_W, scr_H;

    float angle;
    float angleGotFromVec;
};



#endif