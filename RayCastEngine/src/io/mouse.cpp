#include "mouse.hpp"

Mouse::Mouse(double scr_W, double scr_H) 
{
    // set initial position to center of the screen
    this->scr_W = scr_W;
    this->scr_H = scr_H;

    this->firstMouse = true;

    this->setPosition(scr_W / 2.0f, scr_H / 2.0f);
    this->centerX = scr_W / 2.0f;
    this->centerY = scr_H / 2.0f;
}

void Mouse::setPosition(int x, int y)
{
    this->position.x = (double)x;
    this->position.y = (double)y;

    if (this->firstMouse)
    {
        this->lastPos.x = this->position.x;
        this->lastPos.y = this->position.y;
        this->firstMouse = false;
    }
}
