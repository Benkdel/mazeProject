#include "player.hpp"

#include "../shapes/circle.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cmath>



Player::Player() {}

void Player::init(Map *w)
{
    this->pos = vec2(400.0f, 400.0f);

    this->angle = 0.0f; // because we start with the triangle pointing to the left

    this->radius = 6.0f * CONST_PI;

    // left leg, nose, and right leg (points pointing to the right at 0 degrees) // y is inverted
    this->triangle = Triangle(vec2(0.0f, -0.5f), vec2(0.5f, 0.0f), vec2(0.0f, 0.5f));
}

void Player::updateCurrentAngle(Keyboard *kb, float dt)
{
    // update angle correctly
    float rotation = 50.0f * kb->angle * dt;

    // reset keyboard angle
    kb->angle = 0.0f;

    this->angle += rotation;

    if (this->angle < 0.0f)
        this->angle = 360.0f + this->angle;
    if (this->angle > 360.0f)
        this->angle = this->angle - 360.0f + 0.00000001f;
}

void Player::updatePos(Keyboard *kb, float dt)
{
    this->pos.x += cosf(deg2rad(this->angle)) * kb->acceleration * 5.0f * dt;
    this->pos.y += -sinf(deg2rad(this->angle)) * kb->acceleration * 5.0f * dt;

    kb->acceleration = 0.0f;

    // update box collider
    this->box_collider.x = this->pos.x - this->radius * 7 / 10;
    this->box_collider.y = this->pos.y - this->radius * 7 / 10;
    this->box_collider.w = this->radius * 14 / 10;
    this->box_collider.h = this->radius * 14 / 10;
}

void Player::translate()
{
    // scale
    float size = 40.0f;
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

    // reset rays
    for (int i = 0; i < MAX_RAYS; i++)
        this->rays[i].rayDir = vec2(0.0f, 0.0f);

    // set FoV vectors or Rays
    this->lookAt = this->transfTriangle.p2;

    float fAngle = this->angle - (FOV / 2); // substract 30 degrees from current view angle
    float colAngleDelta = FOV / (float)MAX_RAYS; // calculate delta angle for each column
    
    if (fAngle < 0.0f)
        fAngle = 360.0f + fAngle;
    if (fAngle > 360.0f)
        fAngle = fAngle - 360.0f + 0.0001f;

    // store subsequent angles to the rigth starting from p angle - 30 deg
    for (int i = 0; i < MAX_RAYS; i++)
    {
        this->rays[i].angle = fAngle + colAngleDelta * i; // store angle to use later
        this->rays[i].rayDir = getVecFromAngle(1.0f, this->rays[i].angle);
    }
}

void Player::rayCastDDD(Map *map)
{
    for (int i = 0; i < MAX_RAYS; i++)
    {
        this->rays[i].castDDD(this->pos, this->rays[i].rayDir, map);
    }
}

void Player::clearRays()
{
    //
}
