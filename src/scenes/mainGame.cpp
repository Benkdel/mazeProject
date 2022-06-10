#include "mainGame.hpp"
#include <iomanip>

MainGame::MainGame(Window *window, Mouse *mouse, Keyboard *keyboard)
: window(window), mouse(mouse), keyboard(keyboard) {
    
    // initialization
    this->setMinimapPort();
    this->setWorldPort();
    this->setMapPerimeter();
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

void MainGame::render(float dt)
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
    
    this->keyboard->printData = false;
}

void MainGame::cleanup()
{
    this->player.clearRays();
}
