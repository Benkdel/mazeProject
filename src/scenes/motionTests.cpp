#include "motionTests.hpp"

#include <cmath>

MotionTesting::MotionTesting(Window *window, Mouse *mouse, Keyboard *keyboard)
    : window(window), mouse(mouse), keyboard(keyboard)
{
    this->setPort();
    this->setMapPerimeter();

    this->scr_W = this->window->getWidth();
    this->scr_H = this->window->getHeight();

    this->centerPoint = vec2((float)this->scr_W / 2, (float)this->scr_H / 2);

    // start line in 0 degrees, along the x axis with size 5
    this->l1 = line(vec2(-0.5f, 0.0f), vec2(0.5f, 0.0f));

    // set up legs of triangle
    this->leftLeg = vec2(-0.5f, 0.5f);
    this->rightLeg = vec2(-0.5f, -0.5f);

    // init angle should be 0, line is parallel to the X-axis
    this->angle = 0.0f;

    this->radius = 2.0f * CONST_PI;
}

void MotionTesting::setPort()
{
    this->VPscreen.x = 0;
    this->VPscreen.y = 0;
    this->VPscreen.w = this->window->getWidth();
    this->VPscreen.h = this->window->getHeight();
}

void MotionTesting::setMapPerimeter()
{
    this->map.setPerimeter(&this->VPscreen);
}

void MotionTesting::updateAngle(float dt)
{
    // update angle correctly
    float rotation = 50.0f * this->keyboard->angle * dt;

    // reset keyboard angle
    this->keyboard->angle = 0.0f;

    this->angle += rotation;

    if (this->angle < 0.0f)
        this->angle = 360.0f + this->angle;
    if (this->angle > 360.0f)
        this->angle = this->angle - 360.0f + 0.00000001f;
}

void MotionTesting::updatePos(float dt)
{
    // apply motion
    this->centerPoint.x += cosf(deg2rad(this->angle)) * keyboard->acceleration * 5.0f * dt;
    this->centerPoint.y += -sinf(deg2rad(this->angle)) * keyboard->acceleration * 5.0f * dt;

    this->keyboard->acceleration = 0.0f;
}

void MotionTesting::render(float dt)
{
    // execute testing
    this->map.renderGrid(this->window, &this->VPscreen);

    // update angle
    this->updateAngle(dt);
    
    line tl1;
    vec2 lLeg;
    vec2 rLeg;

    // scale line by size
    float size = 200.0f;
    tl1.p1 = scale2Dvec(this->l1.p1, size);
    tl1.p2 = scale2Dvec(this->l1.p2, size);

    // scale legs by same size
    lLeg = scale2Dvec(this->leftLeg, size);
    rLeg = scale2Dvec(this->rightLeg, size);
    

    // rotate line towards angle
    tl1.p1 = rotate2Dvec(tl1.p1, -this->angle);
    tl1.p2 = rotate2Dvec(tl1.p2, -this->angle);

    // rotate legs towards angle
    lLeg = rotate2Dvec(lLeg, -this->angle);
    rLeg = rotate2Dvec(rLeg, -this->angle);

    // transform to center position
    tl1.p1 = tl1.p1 + this->centerPoint;
    tl1.p2 = tl1.p2 + this->centerPoint;

    // transform legs to center position
    lLeg = lLeg + this->centerPoint; 
    rLeg = rLeg + this->centerPoint; 

    float radians;
    float posX = tl1.p2.x - this->scr_W / 2;
    float posY = tl1.p2.y - this->scr_H / 2;
    radians = atan(posY / posX);
    this->angleGotFromVec = rad2deg(radians);

    SDL_SetRenderDrawColor(this->window->renderer, 247, 0, 49, 255);
    SDL_RenderDrawLineF(this->window->renderer, tl1.p1.x, tl1.p1.y, tl1.p2.x, tl1.p2.y);

    // draw legs of triangle
    SDL_RenderDrawLineF(this->window->renderer, lLeg.x, lLeg.y, tl1.p2.x, tl1.p2.y);
    SDL_RenderDrawLineF(this->window->renderer, rLeg.x, rLeg.y, tl1.p2.x, tl1.p2.y);

    // draw base
    SDL_RenderDrawLineF(this->window->renderer, lLeg.x, lLeg.y, rLeg.x, rLeg.y);

    // draw circle at center of the triangle
    this->circle.drawCircle(this->window->renderer, (int32_t)this->centerPoint.x, (int32_t)this->centerPoint.y, (int32_t)this->radius);

    // draw circles at the 3 points of the triangle
    this->circle.drawCircle(this->window->renderer, (int32_t)lLeg.x, (int32_t)lLeg.y, (int32_t)this->radius);
    this->circle.drawCircle(this->window->renderer, (int32_t)rLeg.x, (int32_t)rLeg.y, (int32_t)this->radius);
    this->circle.drawCircle(this->window->renderer, (int32_t)tl1.p2.x, (int32_t)tl1.p2.y, (int32_t)this->radius);


    if (this->keyboard->printData)
    {
        std::cout << "Position of P2 of Line: x: " << posX << " y: " << posY << "\n";
        this->debugging();
    }
}

void MotionTesting::debugging()
{
    std::cout << "Line angle: " << this->angle << "\n";
    std::cout << "Keyboard angle: " << this->keyboard->angle << "\n";
    std::cout << "Angle obtained from line vectors: " << this->angleGotFromVec << "\n";

    this->keyboard->printData = false;
}

void MotionTesting::cleanup() {}
