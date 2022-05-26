#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL.h>
#include "myMathLib.hpp"
#include "window.hpp"

#include <vector>

class Scene {
public:

    Scene();

    void render(Window *w, SDL_Rect *port, std::vector<line> scene);
    

};

#endif