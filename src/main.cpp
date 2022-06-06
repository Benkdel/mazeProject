#include <iostream>
#include <vector>

#include "macros.hpp"
#include "window.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "player.hpp"
#include "map.hpp"
#include "scene.hpp"
#include "collision.hpp"

void pollEvents(Window *window, Mouse *mouse, Keyboard *keyBoard, SDL_Event *event, float deltaTime);
void setMinimapPort(SDL_Rect *vp, unsigned int scr_W, unsigned int scr_H);
void setWorldPort(SDL_Rect *vp, unsigned int scr_W, unsigned int scr_H);

/**
 * main - entry point
 * Return: 0 for success
 */
int main(int argc, char **argv)
{
    SDL_Event event;
    SDL_Rect miniMapVP;
    SDL_Rect worldVP;
    SDL_Rect hoverGrid;

    // +1 so that the last grid lines fit in the 
    const int SCREEN_WIDTH = (CELL_SIZE * GRID_WIDTH) + 1;
    const int SCREEN_HEIGHT = (CELL_SIZE * GRID_HEIGHT) + 1;
    
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
    Map map;
    std::vector<line> iWalls;
    Player firstPlayer;

    std::vector<line> scenePoints;

    vec2 pos, dir;

    unsigned int i, j;

    unsigned int lastTime = 0;
    unsigned int currentTime = 0;
    float deltaTime = 0;

    /* IO */
    Mouse mouse;
    Keyboard keyboard;

    if (!window.status)
    {
        window.cleanup();
        std::cout << "Exiting program as a result of failing to initialice SDL window\n";
        return (1);
    }

    /* set minimap and world ports */
    setMinimapPort(&miniMapVP, window.getWidth(), window.getHeight());
    setWorldPort(&worldVP, window.getWidth(), window.getHeight());

    // set outer walls
    map.setPerimeter(&miniMapVP);

    // init player
    firstPlayer.init(&map);
    keyboard.pos = firstPlayer.pos;
    keyboard.angle = firstPlayer.angle;

    while (!window.windowShouldClose)
    {
        currentTime = SDL_GetTicks();
        deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        // handle events on queue
        pollEvents(&window, &mouse, &keyboard, &event, deltaTime);

        // Clear screen
        SDL_SetRenderDrawColor(window.renderer, 0, 1, 2, 1);
        SDL_RenderClear(window.renderer);
        
        map.renderGrid(&window, &miniMapVP);
        
        map.renderInnerWalls(&window, &miniMapVP);

        firstPlayer.render(&window, &miniMapVP);

        // update player pos
        vec2 mousePos = vec2(mouse.position.x, mouse.position.y);
        
        if (keyboard.called)
        {
            firstPlayer.updatePos(keyboard.acceleration, mousePos, deltaTime, &miniMapVP);
            keyboard.called = false;
        } 

        firstPlayer.updateCurrentAngle(keyboard.angle);
        
        // reset velocity
        keyboard.velocity = 0.0f;
        
        vec2 collBoxpos = { (float)firstPlayer.box_collider.x, (float)firstPlayer.box_collider.y };
        /*if (map_collision(firstPlayer.pos, &map, &miniMapVP, &window))
        {
            //std::cout << "collision detected!\n";
        }*/

        if (keyboard.printData)
        {
            std::cout << "Player pos == x: " << firstPlayer.pos.x << " y: " << firstPlayer.pos.y << "\n";
            std::cout << "Player velocity == x: " << firstPlayer.lastVelocity.x << " y: " << firstPlayer.lastVelocity.y << "\n";
            std::cout << "Player Top Point == x: " << firstPlayer.translTriangle.p2.x << " y: " << firstPlayer.translTriangle.p2.y << "\n";
            std::cout << "Player angle: " << firstPlayer.angle << "\n";
            std::cout << "Player Acceleration: " << keyboard.acceleration << "\n";
            std::cout << "Player current velocity: " << firstPlayer.velocity_mag << "\n";
            std::cout << "Player lookAt norm vector == x: " << firstPlayer.lookAt.x << " y: " << firstPlayer.lookAt.y << "\n"; 
        }

        hoverGrid = {(mouse.position.x / CELL_SIZE) * CELL_SIZE, (mouse.position.y / CELL_SIZE) * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderSetViewport(window.renderer, &miniMapVP);
        SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 1);
        SDL_RenderFillRect(window.renderer, &hoverGrid);

        // loop throught all rays from player
        firstPlayer.setUpRays();
        HitResult hitResult;
        for (i = 0; i < firstPlayer.rays.size(); i++)
        {
            hitResult = firstPlayer.rays[i].castDDD(firstPlayer.pos, firstPlayer.lookAt, &map);

            // then draw closest hit
            SDL_RenderSetViewport(window.renderer, &miniMapVP);
            if (hitResult.hit) SDL_SetRenderDrawColor(window.renderer, 153, 0, 76, 1);
            else SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 1);
            SDL_RenderDrawLine(window.renderer, firstPlayer.pos.x, firstPlayer.pos.y, hitResult.intersection.x, hitResult.intersection.y);
            if (keyboard.printData)
            {
                std::cout << "RayCast draw from player to  x: " << hitResult.intersection.x << " y: " << hitResult.intersection.y << "\n";
                std::cout << "RayCast hit: " << hitResult.hit << "\n";
                keyboard.printData = false;
            }

            // store hit to draw it in world view port
            line l;
            l.p1.x = firstPlayer.pos.x;
            l.p1.y = firstPlayer.pos.y;
            l.p2.x = hitResult.intersection.x;
            l.p2.y = hitResult.intersection.y;
            scenePoints.push_back(l);
        }

        /* Render SCENE in world view port */
        /*Scene scene;

        scene.render(&window, &worldVP, scenePoints);
        scenePoints.clear();*/

        /* Update the surface */
        SDL_RenderPresent(window.renderer);
    }

    /* cleanup SDL */
    window.cleanup();
    firstPlayer.clearRays();

    return 0;
}

/**
 * setMinimapPort -
 *
 *
 */
void setMinimapPort(SDL_Rect *vp, unsigned int scr_W, unsigned int scr_H)
{
    vp->x = 0;
    vp->y = 0;
    vp->w = (scr_W - 1) / 2;
    vp->h = scr_H;
}

/**
 * setWorldPort -
 *
 *
 */
void setWorldPort(SDL_Rect *vp, unsigned int scr_W, unsigned int scr_H)
{
    vp->x = (scr_W + 1) / 2;
    vp->y = 0;
    vp->w = scr_W;
    vp->h = scr_H;
}

void pollEvents(Window *window, Mouse *mouse, Keyboard *keyBoard, SDL_Event *event, float deltaTime)
{
    int x, y;

    // User request
    while (SDL_PollEvent(event) != 0)
    {
        if (event->type == SDL_QUIT)
        {
            window->windowShouldClose = true;
        }

        if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
        {
            SDL_GetMouseState(&x, &y);
            mouse->setPosition(x, y);
        }
    
        // handle key downs
        if (event->type == SDL_KEYDOWN)
        {
            switch (event->key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    window->windowShouldClose = true;
                    break;
                case SDLK_p:
                    keyBoard->printData = true;
                    break;
                case SDLK_1:
                    keyBoard->angle = 0.0f;
                    break;
                case SDLK_2:
                    keyBoard->angle = 90.0f;
                    break;
                case SDLK_3:
                    keyBoard->angle = 180.0f;
                    break;
                case SDLK_4:
                    keyBoard->angle = 270.0f;
                    break;
                default:
                    break;
            }
        }
    }

    // reset acceleration
    keyBoard->acceleration = 0;

    const uint8_t *currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_W])
    {
        keyBoard->called = true;
        keyBoard->acceleration = 5.0f;
    }
    if (currentKeyStates[SDL_SCANCODE_S])
    {
        keyBoard->called = true;
        keyBoard->acceleration = -5.0f;
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        keyBoard->angle += 5.0f;
        if (keyBoard->angle > 360)
            keyBoard->angle = 0.0f;
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        keyBoard->angle -= 5.0f;
        if (keyBoard->angle < 0)
            keyBoard->angle = 360.0f;
    }
}
