#include "mainGame.hpp"

#include <cmath>

/**
 * draw triangle
 *
 *
 */
void drawTriangle(Window *w, Triangle t)
{
    SDL_RenderDrawLineF(w->renderer, t.p2.x, t.p2.y, t.p3.x, t.p3.y); // nose to right leg
    SDL_RenderDrawLineF(w->renderer, t.p3.x, t.p3.y, t.p1.x, t.p1.y); // right to left leg
    SDL_RenderDrawLineF(w->renderer, t.p1.x, t.p1.y, t.p2.x, t.p2.y); // left to nose
}

MainGame::MainGame(Window *window, Mouse *mouse, Keyboard *keyboard)
    : window(window), mouse(mouse), keyboard(keyboard)
{

    // initialization
    this->setMap();
    this->setMinimapPort();
    this->setWorldPort();

    // Init texture obj and load textures
    this->texture = new Texture();
    this->texture->load(this->window, "../assets/images/space_1.png");
}

void MainGame::setMinimapPort()
{
    this->VPminimap.x = 0;
    this->VPminimap.y = 0;
    this->VPminimap.w = CELL_SIZE * 8;
    this->VPminimap.h = CELL_SIZE * 8;
}
void MainGame::setWorldPort()
{
    this->VPworld.x = 0;
    this->VPworld.y = 0;
    this->VPworld.w = this->window->getWidth();
    this->VPworld.h = this->window->getHeight();
}

void MainGame::setMap()
{
    this->map.setWalls();
}

void MainGame::initPlayers()
{
    this->player.init(&this->map);
}

void MainGame::initEnemies()
{
    // nothing for now
}

void MainGame::renderMinimap(float dt)
{
    // render mini map stuff
    SDL_RenderSetViewport(this->window->renderer, &this->VPminimap);
    SDL_SetRenderDrawColor(this->window->renderer, 0, 0, 0, 255);

    // render background for minimap
    SDL_Rect r = {0, 0, this->VPminimap.w, this->VPminimap.h};
    SDL_RenderFillRect(this->window->renderer, &r);

    // render minimap to port
    this->map.renderMiniMap(this->window, &this->VPminimap, &this->VPworld);

    // render player position and rays to port
    float xConvRatio = ((float)this->VPminimap.w / (float)this->VPworld.w);
    float yConvRatio = ((float)this->VPminimap.h / (float)this->VPworld.h);

    Triangle minimapTriangle;
    minimapTriangle.p1.x = this->player.transfTriangle.p1.x * xConvRatio;
    minimapTriangle.p1.y = this->player.transfTriangle.p1.y * yConvRatio;

    minimapTriangle.p2.x = this->player.transfTriangle.p2.x * xConvRatio;
    minimapTriangle.p2.y = this->player.transfTriangle.p2.y * yConvRatio;

    minimapTriangle.p3.x = this->player.transfTriangle.p3.x * xConvRatio;
    minimapTriangle.p3.y = this->player.transfTriangle.p3.y * yConvRatio;

    drawTriangle(this->window, minimapTriangle);

    // draw rays
    for (int i = 0; i < MAX_RAYS; i++)
    {
        if (this->player.rays[i].results.hit)
            SDL_SetRenderDrawColor(window->renderer, 153, 0, 76, 255);
        else
            SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);

        float xPos = this->player.pos.x * xConvRatio;
        float yPos = this->player.pos.y * yConvRatio;
        float xInt = this->player.rays[i].results.intersection.x * xConvRatio;
        float yInt = this->player.rays[i].results.intersection.y * yConvRatio;

        SDL_RenderDrawLineF(this->window->renderer, xPos, yPos, xInt, yInt);
    }
}

void MainGame::renderWorld(float dt)
{
    // player updates and translations
    this->player.updatePos(this->keyboard, dt);
    this->player.updateCurrentAngle(this->keyboard, dt);
    this->player.translate();

    // raycasting
    this->player.rayCastDDD(&this->map);

    // render world:
    SDL_RenderSetViewport(this->window->renderer, &this->VPworld);

    // render background
    this->texture->render(this->window, 0, 0);

    // on top of that load floor textures
    SDL_SetRenderDrawColor(this->window->renderer, 138, 127, 74, 255);
    SDL_Rect floor = {0, this->VPworld.h / 2, this->VPworld.w, this->VPworld.h / 2};
    SDL_RenderFillRect(this->window->renderer, &floor);

    // render walls based on rays length
    int nGallGroups = this->VPworld.w / MAX_RAYS;
    int wallWidth = 3;
    int wallsPerRay = nGallGroups / wallWidth;
    float rayOffset = 0.0f;

    SDL_FRect wall;
    vec2 fWall = vec2((float)(CELL_SIZE * 3), (float)(this->VPworld.h - CELL_SIZE));

    for (int i = 0; i < MAX_RAYS; i++)
    {
        // set current wall

        // fix fish eye
        float aDist = this->player.rays[i].angle - this->player.angle;
        aDist = (aDist < 0.0f) ? 360.0f + aDist : aDist;
        aDist = (aDist > 360.0f) ? aDist - 360.0f + 0.0001f : aDist;

        float modDistance = this->player.rays[i].distance * cos(deg2rad(aDist));

        float wallHeight = (float)(this->VPworld.h * CELL_SIZE) / modDistance;
        if (wallHeight > this->VPworld.h - 2 * CELL_SIZE)
            wallHeight = this->VPworld.h - 2 * CELL_SIZE;

        if (this->player.rays[i].results.HitDir == 0)
            SDL_SetRenderDrawColor(this->window->renderer, 0, 128, 255, 255);
        else
            SDL_SetRenderDrawColor(this->window->renderer, 0, 51, 102, 255);            

        // draw multiple walls per ray
        for (int j = 0; j < wallsPerRay; j++)
        {
            wall.x = i * wallWidth + (j * wallWidth) + rayOffset;
            wall.y = this->VPworld.h / 2 - wallHeight / 2;
            wall.w = wallWidth;
            wall.h = wallHeight;

            // render current wall
            SDL_RenderFillRectF(this->window->renderer, &wall);
        }
        rayOffset += (wallsPerRay - 1) * wallWidth;
    }

    if (this->keyboard->printData)
        this->debugging();
}

void MainGame::debugging()
{
    std::cout << "===============================================================================================\n";
    std::cout << "Player pos => x: " << this->player.pos.x << " y: " << this->player.pos.y << "\n";
    std::cout << "Top poing => x: " << this->player.triangle.p2.x << " y: " << this->player.triangle.p2.y << "\n";
    std::cout << "Player angle: " << this->player.angle << "\n";
    std::cout << "Player Acceleration: " << this->keyboard->acceleration << "\n";

    std::cout << "================< FoV Vectors - mid quartil >========================\n";
    std::cout << " --  X  -- || --  Y  --\n";
    for (int i = (MAX_RAYS / 4) * 2; i < (MAX_RAYS / 4) * 3; i++)
    {
        std::cout << " " << this->player.rays[i].rayDir.x << "     " << this->player.rays[i].rayDir.y << "\n";
    }

    std::cout << "================< distances mid quartil >========================\n";
    for (int i = (MAX_RAYS / 4) * 2; i < (MAX_RAYS / 4) * 3; i++)
    {
        std::cout << this->player.rays[i].distance << "\n";
    }

    this->keyboard->printData = false;
}

void MainGame::cleanup()
{
    this->player.clearRays();
}
