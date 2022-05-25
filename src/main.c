#include "headers.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

void pollEvents(sdl_window *window, mouse_handler *mouse, keyboard_handler *keyBoard, SDL_Event *event);

/**
 * main - entry point
 * Return: 0 for success
 */
int main(int argc, char **argv)
{
    SDL_Event event;
    SDL_Rect miniMapVP;
    SDL_Rect worldVP;

    sdl_window window;
    outer_walls oWalls;
    inner_walls iWalls;
    player firstPlayer;

    /* to store lines that where a HIT */
    line scene[MAX_SCENE_WIDTH];
    size_t scene_lines_counter = 0;

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

    /* set minimap and world ports */
    setMinimapPort(&miniMapVP, window.SCREEN_WIDTH, window.SCREEN_HEIGHT);
    setWorldPort(&worldVP, window.SCREEN_WIDTH, window.SCREEN_HEIGHT);

    /* set maps */
    setOuterWalls(&oWalls, &miniMapVP, 0.9);

    /* set wall and player */
    pos.x = 300;
    pos.y = 500;
    pos.z = 0;
    dir.x = 320;
    dir.y = 500;
    dir.z = 0;
    playerInit(&window, &firstPlayer, pos, dir, 60.0f);

    // set keyboard init pos
    keyBoard.pos = pos;

    // Init inner walls
    setInnerWalls(&iWalls, &oWalls);

    bool firstCheck = true;

    while (!window.windowShouldClose)
    {
        // handle events on queue
        pollEvents(&window, &mouse, &keyBoard, &event);

        // Clear screen
        SDL_SetRenderDrawColor(window.renderer, 0, 1, 2, 1);
        SDL_RenderClear(window.renderer);

        /* render outer walls */
        renderOuterWalls(&window, &oWalls, &miniMapVP);

        /* render wall and player*/
        renderInnerWalls(&window, &iWalls, &miniMapVP);
        playerRender(&window, &firstPlayer, &miniMapVP);

        /* update player vector */
        dir.x = mouse.position.x;
        dir.y = mouse.position.y;
        dir.z = 0;
        playerUpdateDir(&firstPlayer, dir);
        playerUpdatePos(&firstPlayer, keyBoard.pos);

        /* check if there is intersection */
        scene_lines_counter = 0;
        // loop throught all rays from player
        for (i = 0; i < firstPlayer.noRays; i++)
        {
            closestDistance = 100000.0f;
            closestVec = firstPlayer.rays[i].pos;

            // loop through all the walls to find the closest one
            for (j = 0; j < MAX_WALLS; j++)
            {
                if (rayCasting(iWalls.l[j], firstPlayer.rays[i], &intPos))
                {
                    currentDistance = distanceBtwPoints(firstPlayer.rays[i].pos, intPos);

                    // check all walls to see which is the nearest one
                    if (currentDistance < closestDistance)
                    {
                        closestVec = intPos;
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
            l.x1 = firstPlayer.pos.x;
            l.y1 = firstPlayer.pos.y;
            l.x2 = closestVec.x;
            l.y2 = closestVec.y;
            scene[scene_lines_counter] = l;
            scene_lines_counter++;
        }

        /* Render SCENE in world view port */
        renderScene(&window, &worldVP, scene, scene_lines_counter);
        
        // clear scene
        for (size_t s = 0; s < scene_lines_counter; s++)
        {
            scene[s].x1 = 0;
            scene[s].y1 = 0;
            scene[s].x2 = 0;
            scene[s].y2 = 0;
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
