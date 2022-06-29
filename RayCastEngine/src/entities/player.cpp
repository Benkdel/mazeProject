#include "player.hpp"

#include "../shapes/circle.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cmath>

#include "../physics/collision.hpp"

Player::Player() {}

void Player::init(Map *m)
{
    this->pos = m->getPlayerPos();

    this->angle = 0.0f; // because we start with the triangle pointing to the right

    // left leg, nose, and right leg (points pointing to the right at 0 degrees) // y is inverted
    this->triangle = Triangle(vec2(0.0f, -0.5f), vec2(0.5f, 0.0f), vec2(0.0f, 0.5f));
}

void Player::updateCurrentAngle(Mouse *m, float dt)
{
    float deltaX = m->position.x - m->lastPos.x;
    m->lastPos.x = m->position.x;
    this->angle += deltaX * 0.5f;
    this->angle = clampAngle(this->angle);
}

void Player::updatePos(Keyboard *kb, float dt, Map *map)
{
    vec2 velocity;
    vec2 newPlayerPos;

    velocity.x = cosf(deg2rad(this->angle)) * kb->velocity * dt;
    velocity.y = -sinf(deg2rad(this->angle)) * kb->velocity * dt;

    newPlayerPos = this->pos + velocity;

    vec2 collDist = map_collision(this->pos, newPlayerPos, velocity, map);

    if (collDist.x != -1)
    {
        this->pos.x = collDist.x;
        velocity.x = 0.0f;
    }

    if (collDist.y != -1)
    {
        this->pos.y = collDist.y;
        velocity.y = 0.0f;
    }

    this->pos.x += velocity.x;
    this->pos.y += velocity.y;

    kb->velocity = 0.0f;
}

void Player::translate(int size, int numRays)
{
    // scale
    this->transfTriangle.p1 = scale2Dvec(this->triangle.p1, size);
    this->transfTriangle.p2 = scale2Dvec(this->triangle.p2, size);
    this->transfTriangle.p3 = scale2Dvec(this->triangle.p3, size);

    // rotate
    // im negating the angles to flip directions correclty
    this->transfTriangle.p1 = rotate2Dvec(this->transfTriangle.p1, -this->angle);
    this->transfTriangle.p2 = rotate2Dvec(this->transfTriangle.p2, -this->angle);
    this->transfTriangle.p3 = rotate2Dvec(this->transfTriangle.p3, -this->angle);

    // translate
    this->transfTriangle.p1 = this->transfTriangle.p1 + this->pos;
    this->transfTriangle.p2 = this->transfTriangle.p2 + this->pos;
    this->transfTriangle.p3 = this->transfTriangle.p3 + this->pos;

    // set FoV vectors or Rays
    this->lookAt = this->transfTriangle.p2;
}

