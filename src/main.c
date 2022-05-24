#include "headers.h"

/* ========================= */
/* global variables */
/* ========================= */

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

void pollEvents(sdl_window *window, mouse_handler *mouse, keyboard_handler *keyBoard, SDL_Event *event);

bool printData = false;

/**
 * main - entry point
 * Return: 0 for success
 */
int main(int argc, char **argv)
{
    sdl_window window;
    SDL_Event event;
    outer_walls oWalls;
    player firstPlayer;
    player tmp;
    line walls[MAX_WALLS];

    vec3 pos;
    vec3 dir;
    vec3 intPos;
    vec3 closestVec;

    unsigned int i, j;

    float wallX, wallY;
    float currentDistance;
    float closestDistance;

    /* IO */
    mouse_handler mouse;
    keyboard_handler keyBoard;

    if (!initWindow(&window, SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        cleanUpWindow(&window);
        printf("Exiting program as a result of failing to initialice SDL window");
        return (1);
    }

    /* set maps */
    setOuterWalls(&window, &oWalls, 0.9);

    /* set wall and player */

    pos.x = 300;
    pos.y = 500;
    pos.z = 0;
    dir.x = 320;
    dir.y = 500;
    dir.z = 0;
    playerInit(&firstPlayer, pos, dir, 60.0f);
    // set keyboard init pos
    keyBoard.pos = pos;

    // set array of walls
    for (i = 0; i < MAX_WALLS; i++)
    {
        walls[i].x1 = (rand() % (oWalls.w - oWalls.x)) + oWalls.x;
        walls[i].x2 = (rand() % (oWalls.w - oWalls.x)) + oWalls.x;

        walls[i].y1 = (rand() % (oWalls.h - oWalls.y)) + oWalls.y;
        walls[i].y2 = (rand() % (oWalls.h - oWalls.y)) + oWalls.y;
    }

    /*wall.x1 = 1000;
    wall.y1 = 700;
    wall.x2 = 1000;
    wall.y2 = 200;*/

    bool firstCheck = true;

    while (!window.windowShouldClose)
    {
        // handle events on queue
        pollEvents(&window, &mouse, &keyBoard, &event);

        // Clear screen
        SDL_SetRenderDrawColor(window.renderer, 0, 1, 2, 1);
        SDL_RenderClear(window.renderer);

        /* render outer walls */
        renderOuterWalls(&window, &oWalls);

        /* update player vector */
        dir.x = mouse.position.x;
        dir.y = mouse.position.y;
        dir.z = 0;
        playerUpdateDir(&firstPlayer, dir);

        playerUpdatePos(&firstPlayer, keyBoard.pos);

        /* render wall and player*/
        for (i = 0; i < MAX_WALLS; i++)
        {
            renderWall(&window, &walls[i]);
        }
        playerRender(&window, &firstPlayer);

        /* check if there is intersection */
        if (firstCheck)
        {
            for (i = 0; i < firstPlayer.noRays; i++)
            {
                printf("Rays Directions: \nPoint[%d] => x: %f  y:%f \n", i, firstPlayer.rays[i].x, firstPlayer.rays[i].y);
            }
            printf("========= \n");
            for (i = 0; i < MAX_WALLS; i++)
            {
                printf("Walls Pos: \nPoint[%d] => x1: %f  y1:%f \n", i, walls[i].x1, walls[i].y1);
                printf("Walls Pos: \nPoint[%d] => x2: %f  y2:%f \n", i, walls[i].x2, walls[i].y2);
            }
            firstCheck = false;
        }

        // loop throught all rays from player
        for (i = 0; i < firstPlayer.noRays; i++)
        {
            tmp.pos = firstPlayer.pos;
            tmp.dir = firstPlayer.rays[i];

            closestDistance = 100000.0f;
            closestVec = tmp.pos;

            // loop through all the walls
            for (j = 0; j < MAX_WALLS; j++)
            {
                if (rayCasting(walls[j], tmp, &intPos))
                {
                    currentDistance = distanceBtwPoints(firstPlayer.pos, intPos);
                    
                    // check all walls to see which is the nearest one
                    if (currentDistance < closestDistance)
                    {
                        closestVec = intPos;
                        closestDistance = currentDistance;
                    }

                }
            }
            // then draw closest hit
            SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 1);
            SDL_RenderDrawLine(window.renderer, firstPlayer.pos.x, firstPlayer.pos.y, closestVec.x, closestVec.y);
        }

        if (printData)
        {
            printData = false;
        }

        /* Update the surface */
        SDL_RenderPresent(window.renderer);
    }

    /* cleanup SDL */
    cleanUpWindow(&window);

    return (0);
}

void pollEvents(sdl_window *window, mouse_handler *mouse, keyboard_handler *keyBoard, SDL_Event *event)
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
            mouseSetPosition(mouse, x, y);
        }
    }

    vec3 Up = {0.0f, -5.0f, 0.0f};
    vec3 down = {0.0f, 5.0f, 0.0f};
    vec3 left = {-5.0f, 0.0f, 0.0f};
    vec3 right = {5.0f, 0.0f, 0.0f};

    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_W])
    {
        keyBoard->pos = vectorAddition(keyBoard->pos, Up);
    }
    if (currentKeyStates[SDL_SCANCODE_S])
    {
        keyBoard->pos = vectorAddition(keyBoard->pos, down);
    }
    if (currentKeyStates[SDL_SCANCODE_A])
    {
        keyBoard->pos = vectorAddition(keyBoard->pos, left);
    }
    if (currentKeyStates[SDL_SCANCODE_D])
    {
        keyBoard->pos = vectorAddition(keyBoard->pos, right);
    }

    if (currentKeyStates[SDL_SCANCODE_P])
    {
        printData = true;
    }
}
