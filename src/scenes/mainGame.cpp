#include "mainGame.hpp"


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

    // update player position
    this->player.updatePos(this->keyboard->acceleration, this->mouse->position, dt, &this->VPminimap);
    
    this->player.updateCurrentAngle(this->keyboard->angle);
    this->keyboard->angle = 0.0f; // reset after applied

    // set up rays
    this->player.setUpRays();

    this->player.rayCastDDD(this->window, &this->VPminimap, &this->map);


    if (this->keyboard->printData)
        this->debugging();
}

void MainGame::debugging()
{
    std::cout << "Player pos == x: " << this->player.pos.x << " y: " << this->player.pos.y << "\n";
    std::cout << "Player velocity == x: " << this->player.lastVelocity.x << " y: " << this->player.lastVelocity.y << "\n";
    std::cout << "Player Top Point == x: " << this->player.translTriangle.p2.x << " y: " << this->player.translTriangle.p2.y << "\n";
    std::cout << "Player angle: " << this->player.angle << "\n";
    std::cout << "Player Acceleration: " << this->keyboard->acceleration << "\n";
    std::cout << "Player current velocity: " << this->player.velocity_mag << "\n";
    std::cout << "Player lookAt norm vector == x: " << this->player.lookAt.x << " y: " << this->player.lookAt.y << "\n"; 
   

    this->keyboard->printData = false;
}

void MainGame::cleanup()
{
    this->player.clearRays();
}
