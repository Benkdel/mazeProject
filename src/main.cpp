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

    vec2 pos, dir, intPos, closestVec;

    unsigned int i, j;

    float wallX, wallY, currentDistance, closestDistance;

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
        firstPlayer.updatePos(keyboard.acceleration, mousePos, deltaTime, &miniMapVP);
        firstPlayer.angle = keyboard.angle;
        // reset velocity
        keyboard.velocity = 0.0f;
        
        vec2 collBoxpos = { (float)firstPlayer.box_collider.x, (float)firstPlayer.box_collider.y };
        /*if (map_collision(firstPlayer.pos, &map, &miniMapVP, &window))
        {
            //std::cout << "collision detected!\n";
        }*/

        if (keyboard.printData)
        {
            std::cout << "Player pos == x: " << firstPlayer.pos.y << " y: " << firstPlayer.pos.y << "\n";
            std::cout << "Player velocity == x: " << firstPlayer.lastVelocity.x << " y: " << firstPlayer.lastVelocity.y << "\n";
            std::cout << "Player Top Point == x: " << firstPlayer.translTriangle.p2.x << " y: " << firstPlayer.translTriangle.p2.y << "\n";
            std::cout << "Player angle: " << firstPlayer.angle << "\n";
            std::cout << "Player Acceleration: " << keyboard.acceleration << "\n";
            std::cout << "Player current velocity: " << firstPlayer.velocity_mag << "\n";
            keyboard.printData = false;
        }

        hoverGrid = {(mouse.position.x / CELL_SIZE) * CELL_SIZE, (mouse.position.y / CELL_SIZE) * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderSetViewport(window.renderer, &miniMapVP);
        SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 1);
        SDL_RenderFillRect(window.renderer, &hoverGrid);

        // loop throught all rays from player
        firstPlayer.setUpRays();
        for (i = 0; i < firstPlayer.rays.size(); i++)
        {
            closestDistance = 100000.0f;
            closestVec = firstPlayer.rays[i].pos;

            // loop through all the walls to find the closest one
            for (j = 0; j < map.innerWalls.size(); j++)
            {
                if (firstPlayer.rays[i].cast(map.innerWalls[j]))
                {
                    currentDistance = distanceBtwPoints(firstPlayer.rays[i].pos, firstPlayer.rays[i].hit);

                    // check all walls to see which is the nearest one
                    if (currentDistance < closestDistance)
                    {
                        closestVec = firstPlayer.rays[i].hit;
                        closestDistance = currentDistance;
                    }
                }
            }
            // then draw closest hit
            SDL_RenderSetViewport(window.renderer, &miniMapVP);
            SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 1);
            //SDL_RenderDrawLine(window.renderer, firstPlayer.pos.x, firstPlayer.pos.y, closestVec.x, closestVec.y);

            // store hit to draw it in world view port
            line l;
            l.p1.x = firstPlayer.pos.x;
            l.p1.y = firstPlayer.pos.y;
            l.p2.x = closestVec.x;
            l.p2.y = closestVec.y;
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
        keyBoard->acceleration = 0.0001f;
    }
    if (currentKeyStates[SDL_SCANCODE_S])
    {
        keyBoard->acceleration = -0.0001f;
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        keyBoard->angle -= 5.0f;
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        keyBoard->angle += 5.0f;
    }
}
