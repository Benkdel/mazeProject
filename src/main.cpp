#include <iostream>
#include <vector>

#include "macros.hpp"
#include "sdl_engine/window.hpp"
#include "io/keyboard.hpp"
#include "io/mouse.hpp"
#include "entities/player.hpp"
#include "worlds/map.hpp"
#include "scenes/scene.hpp"
#include "physics/collision.hpp"

#include "scenes/mainGame.hpp"
#include "scenes/motionTests.hpp"

void pollEvents(Window *window, Mouse *mouse, Keyboard *keyBoard, SDL_Event *event, float deltaTime);

enum Application
{
    Game = 0,
    TestMotion
};

/**
 * main - entry point
 * Return: 0 for success
 */
int main(int argc, char **argv)
{
    SDL_Event event;

// =========== set what application to run =============
    const int app_choosed = 1;
// =====================================================

    // +1 so that the last grid lines fit in the
    const int SCREEN_WIDTH = (CELL_SIZE * GRID_WIDTH) + 1;
    const int SCREEN_HEIGHT = (CELL_SIZE * GRID_HEIGHT) + 1;

    Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
    Map map;

    unsigned int lastTime = 0.0f;
    unsigned int currentTime = 0.0f;
    float deltaTime = 0.0f;

    /* IO */
    Mouse mouse;
    Keyboard keyboard;

    if (!window.status)
    {
        window.cleanup();
        std::cout << "Exiting program as a result of failing to initialice SDL window\n";
        return (1);
    }

    /* ================================== */
    /* ===== RUN CHOOSEN APPLICATION ==== */
    /* ================================== */
    
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);

    if (app_choosed == Application::Game)
    {
        MainGame mainGame(&window, &mouse, &keyboard);
        mainGame.initPlayers();

        while (!window.windowShouldClose)
        {
            currentTime = SDL_GetTicks();
            deltaTime = (float)(currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // handle events on queue
            pollEvents(&window, &mouse, &keyboard, &event, deltaTime);

            // Clear screen
            SDL_SetRenderDrawColor(window.renderer, 0, 1, 2, 1);
            SDL_RenderClear(window.renderer);

            mainGame.render(deltaTime);

            // reset velocity
            keyboard.velocity = 0.0f;

            /* Update the surface */
            SDL_RenderPresent(window.renderer);
        }

        mainGame.cleanup();
    }

    if (app_choosed == Application::TestMotion)
    {
        MotionTesting motionTesting(&window, &mouse, &keyboard);

        while (!window.windowShouldClose)
        {
            currentTime = SDL_GetTicks();
            deltaTime = (float)(currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // handle events on queue
            pollEvents(&window, &mouse, &keyboard, &event, deltaTime);

            // Clear screen
            SDL_SetRenderDrawColor(window.renderer, 0, 1, 2, 1);
            SDL_RenderClear(window.renderer);

            motionTesting.render(deltaTime);
            motionTesting.updatePos(deltaTime);

            // reset velocity
            keyboard.velocity = 0.0f;

            /* Update the surface */
            SDL_RenderPresent(window.renderer);
        }

        motionTesting.cleanup();
    }

    window.cleanup();
    return 0;
}





/* =============================================================================================== */
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
            case SDLK_f:
                keyBoard->angle = 1.0f;
                break;
            case SDLK_g:
                keyBoard->angle = -1.0f;
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
        keyBoard->acceleration = 50.0f;
    }
    if (currentKeyStates[SDL_SCANCODE_S])
    {
        keyBoard->acceleration = -50.0f;
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        keyBoard->angle = -5.0f;
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        keyBoard->angle = 5.0f;
    }
}
