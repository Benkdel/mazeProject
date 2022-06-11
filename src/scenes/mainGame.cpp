#include "mainGame.hpp"

MainGame::MainGame(Window *window, Mouse *mouse, Keyboard *keyboard)
: window(window), mouse(mouse), keyboard(keyboard) {
    
    // initialization
    this->setMapPerimeter();
    this->setMinimapPort();
    this->setWorldPort();
}

void MainGame::setMinimapPort()
{
    this->VPminimap.x = 0;
    this->VPminimap.y = 0;
    this->VPminimap.w = (this->window->getWidth() - 1) / 2;
    this->VPminimap.h = this->window->getHeight();
}
void MainGame::setWorldPort()
{
    this->VPworld.x = (this->window->getWidth() + 1) / 2;
    this->VPworld.y = 0;
    this->VPworld.w = this->window->getWidth();
    this->VPworld.h = this->window->getHeight();
}

void MainGame::setMapPerimeter()
{
    this->map.setPerimeter(&this->VPminimap);
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
    this->map.renderGrid(this->window, &this->VPminimap);
    this->map.renderInnerWalls(this->window, &this->VPminimap);
    this->player.render(this->window, &this->VPminimap);

    // update player position and angle
    this->player.updatePos(this->keyboard, dt);
    this->player.updateCurrentAngle(this->keyboard, dt);

    this->player.rayCastDDD(this->window, &this->VPminimap, &this->map);

    if (this->keyboard->printData)
        this->debugging();
}

void MainGame::renderWorld(float dt)
{
    // render walls
    
    int wallWidth = this->VPworld.w / MAX_RAYS;
    SDL_FRect wall;
    vec2 fWall = vec2((float)(CELL_SIZE * 3), (float)(this->VPworld.h - CELL_SIZE));
    
    SDL_RenderSetViewport(this->window->renderer, &this->VPworld);
    SDL_SetRenderDrawColor(this->window->renderer, 148, 173, 201, 255);

    for (int i = 0; i < MAX_RAYS; i++)
    {
        // set current wall
        float wallHeight = (float)(this->VPworld.h * CELL_SIZE) / this->player.rays[i].distance;
        if (wallHeight > this->VPworld.h - 2 * CELL_SIZE) wallHeight = this->VPworld.h - 2 * CELL_SIZE;
        wall.x = i * wallWidth;
        wall.y = this->VPworld.h/2 - wallHeight/2;
        wall.w = fWall.x + wallWidth;
        wall.h = wallHeight;

        // transalte into player X position
        //wall.x += this->player.pos.x;
        
        // render current wall
        SDL_RenderFillRectF(this->window->renderer, &wall);
    }
}

void MainGame::debugging()
{
    std::cout << "===============================================================================================\n";
    std::cout << "Player pos => x: " << this->player.pos.x << " y: " << this->player.pos.y << "\n";
    std::cout << "Top poing => x: " << this->player.triangle.p2.x << " y: " << this->player.triangle.p2.y << "\n";
    std::cout << "Player angle: " << this->player.angle << "\n";
    std::cout << "Player Acceleration: " << this->keyboard->acceleration << "\n";

    std::cout << "================< FoV Vectors >========================\n";
    std::cout << " --  X  -- || --  Y  --\n";
    for(int i = 0; i < MAX_RAYS; i++)
    {
        std::cout << " " << this->player.rays[i].rayDir.x << "     " << this->player.rays[i].rayDir.y << "\n";
    }

    std::cout << "================< distances >========================\n";
    for(int i = 0; i < MAX_RAYS; i++)
    {
        std::cout << this->player.rays[i].distance << "\n";
    }
    
    this->keyboard->printData = false;
}

void MainGame::cleanup()
{
    this->player.clearRays();
}
