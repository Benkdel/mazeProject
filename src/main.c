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
    line wall;

    vec3 pos;
    vec3 dir;
    vec3 intPos;

    unsigned int i = 0;

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

    wall.x1 = 1000;
    wall.y1 = 700;
    wall.x2 = 1000;
    wall.y2 = 200;

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
        // printf("player x: %d , y: %d\n", player.pos.x, player.pos.y);

        /* render wall and player*/
        renderWall(&window, &wall);
        playerRender(&window, &firstPlayer);

        /* check if there is intersection */
        if (firstCheck)
        {
            for (i = 0; i < firstPlayer.noRays; i++)
            {
                printf("Point[%d] => x: %f  y:%f \n", i, firstPlayer.rays[i].x, firstPlayer.rays[i].y);
            }
            firstCheck = false;
        }
        
        for (i = 0; i < firstPlayer.noRays; i++)
        {
            tmp.pos = firstPlayer.pos;
            tmp.dir = firstPlayer.rays[i];

            if (rayCasting(wall, tmp, &intPos))
            {
                SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 1);
                SDL_RenderDrawLine(window.renderer, firstPlayer.pos.x, firstPlayer.pos.y, intPos.x, intPos.y);
                if (printData)
                {
                    printf("Ray[%d]\n", i);
                    printf("Hit from position: x: %f  y: %f \n", tmp.pos.x, tmp.pos.y);
                    printf("Hit with direction: x: %f  y: %f \n", tmp.dir.x, tmp.dir.y); 
                    printf("Hit wall at position: x: %f  y: %f\n", intPos.x, intPos.y);
                }
            }
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
