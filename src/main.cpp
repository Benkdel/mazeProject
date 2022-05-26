#include <iostream>
#include <vector>

#include "macros.hpp"
#include "window.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "player.hpp"
#include "map.hpp"
#include "scene.hpp"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

void pollEvents(Window *window, Mouse *mouse, Keyboard *keyBoard, SDL_Event *event);
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

    Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
    OuterWalls oWalls;
    std::vector<line> iWalls;
    Player firstPlayer;

    std::vector<line> scenePoints;

    vec2 pos, dir, intPos, closestVec;

    unsigned int i, j;

    float wallX, wallY, currentDistance, closestDistance;

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
    oWalls.setPerimeter(&miniMapVP, 0.9);

    // init player
    firstPlayer.init(&oWalls);
    keyboard.pos = firstPlayer.pos;

    // set inner walls
    for (i = 0; i < MAX_WALLS; i++)
    {
        line l;
        l.p1.x = (rand() % (oWalls.w - oWalls.x)) + oWalls.x,
        l.p1.y = (rand() % (oWalls.h - oWalls.y)) + oWalls.y,
        l.p2.x = (rand() % (oWalls.w - oWalls.x)) + oWalls.x,
        l.p2.y = (rand() % (oWalls.h - oWalls.y)) + oWalls.y,

        iWalls.push_back(l);
    }

    while (!window.windowShouldClose)
    {
        // handle events on queue
        pollEvents(&window, &mouse, &keyboard, &event);

        // Clear screen
        SDL_SetRenderDrawColor(window.renderer, 0, 1, 2, 1);
        SDL_RenderClear(window.renderer);

        oWalls.render(&window, &miniMapVP);

        // render inner walls
        SDL_RenderSetViewport(window.renderer, &miniMapVP);
        SDL_SetRenderDrawColor(window.renderer, 255, 0, 0, 1);

        for (i = 0; i < MAX_WALLS; i++)
        {
            SDL_RenderDrawLine(window.renderer, iWalls[i].p1.x, iWalls[i].p1.y, iWalls[i].p2.x, iWalls[i].p2.y);
        }

        firstPlayer.render(&window, &miniMapVP);
        firstPlayer.updateDir(vec2(mouse.position.x, mouse.position.y));
        firstPlayer.updatePos(keyboard.pos);

        // loop throught all rays from player
        firstPlayer.setUpRays();
        for (i = 0; i < firstPlayer.rays.size(); i++)
        {
            closestDistance = 100000.0f;
            closestVec = firstPlayer.rays[i].pos;

            // loop through all the walls to find the closest one
            for (j = 0; j < MAX_WALLS; j++)
            {
                if (firstPlayer.rays[i].cast(iWalls[j]))
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
            SDL_RenderDrawLine(window.renderer, firstPlayer.pos.x, firstPlayer.pos.y, closestVec.x, closestVec.y);

            // store hit to draw it in world view port
            line l;
            l.p1.x = firstPlayer.pos.x;
            l.p1.y = firstPlayer.pos.y;
            l.p2.x = closestVec.x;
            l.p2.y = closestVec.y;
            scenePoints.push_back(l);
        }

        /* Render SCENE in world view port */
        Scene scene;

        scene.render(&window, &worldVP, scenePoints);
        scenePoints.clear();
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
    vp->w = scr_W / 2;
    vp->h = scr_H;
}

/**
 * setWorldPort -
 *
 *
 */
void setWorldPort(SDL_Rect *vp, unsigned int scr_W, unsigned int scr_H)
{
    vp->x = scr_W / 2;
    vp->y = 0;
    vp->w = scr_W;
    vp->h = scr_H;
}

void pollEvents(Window *window, Mouse *mouse, Keyboard *keyBoard, SDL_Event *event)
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
    }

    vec2 Up = {0.0f, -5.0f};
    vec2 down = {0.0f, 5.0f};
    vec2 left = {-5.0f, 0.0f};
    vec2 right = {5.0f, 0.0f};

    const uint8_t *currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_W])
    {
        keyBoard->pos = keyBoard->pos + Up;
    }
    if (currentKeyStates[SDL_SCANCODE_S])
    {
        keyBoard->pos = keyBoard->pos + down;
    }
    if (currentKeyStates[SDL_SCANCODE_A])
    {
        keyBoard->pos = keyBoard->pos + left;
    }
    if (currentKeyStates[SDL_SCANCODE_D])
    {
        keyBoard->pos = keyBoard->pos + right;
    }
}
