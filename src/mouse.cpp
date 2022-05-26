#include "mouse.hpp"

Mouse::Mouse() {}

void Mouse::setPosition(int x, int y)
{
    this->position.x = x;
    this->position.y = y;
}