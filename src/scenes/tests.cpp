#include "tests.hpp"

#include <cmath>

Texture *tex;

TestsModule::TestsModule(Window *window, Mouse *mouse, Keyboard *keyboard)
    : window(window), mouse(mouse), keyboard(keyboard)
{
    this->setPort();

    this->scr_W = this->window->getWidth();
    this->scr_H = this->window->getHeight();

    this->centerPoint = vec2((float)this->scr_W / 2, (float)this->scr_H / 2);

    tex = new Texture(this->window, "../assets/Floors/floor.png", 1);

    // mGround = new Sprite(mapSize, mapSize);

    // for (int x = 0; x < mapSize; x += 32)
    // {
    //     for (int y = 0; y < mapSize; y++)
    //     {
    //         mGround->setColor(x, y, RGB::magenta);
    //         mGround->setColor(x + 1, y, RGB::crimson);
    //         mGround->setColor(x - 1, y, RGB::red);

    //         mGround->setColor(y, x, RGB::stateBlue);
    //         mGround->setColor(y, x + 1, RGB::gold);
    //         mGround->setColor(y, x - 1, RGB::white);
    //     }
    // }
}

void TestsModule::setPort()
{
    this->VPscreen.x = 0;
    this->VPscreen.y = 0;
    this->VPscreen.w = this->window->getWidth();
    this->VPscreen.h = this->window->getHeight();
}

void TestsModule::updateAngle(float dt)
{
    // pass
}

void TestsModule::updatePos(float dt)
{
    // pass
}

void TestsModule::render(float dt)
{

    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
            window->windowShouldClose = true;
    }
    const uint8_t *currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_Q])
        fNear += 0.1f * dt;
    if (currentKeyStates[SDL_SCANCODE_A])
        fNear -= 0.1f * dt;

    if (currentKeyStates[SDL_SCANCODE_W])
        fFar += 0.1f * dt;
    if (currentKeyStates[SDL_SCANCODE_S])
        fFar -= 0.1f * dt;

    if (currentKeyStates[SDL_SCANCODE_Z])
        fFovHalf += 0.1f * dt;
    if (currentKeyStates[SDL_SCANCODE_X])
        fFovHalf -= 0.1f * dt;

    if (currentKeyStates[SDL_SCANCODE_LEFT])
        fWorldA -= 5.0f * dt;
    if (currentKeyStates[SDL_SCANCODE_RIGHT])
        fWorldA += 5.0f * dt;
    if (currentKeyStates[SDL_SCANCODE_UP])
    {
        fWorldX += cosf(fWorldA) * dt;
        fWorldY += sinf(fWorldA) * dt;
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN])
    {
        fWorldX -= cosf(fWorldA) * dt;
        fWorldY -= sinf(fWorldA) * dt;
    }

    // this operations give you 4 coordinate points for the map
    /*  Far x1, Far y1 (*) ___________________ (*) Far x2, Far y2
                          |                   |
                           |                 |
                            |               |
                             |             |
         Near x1, Near y1 (*) |___________| (*) Near x2, Near y2
    */

    float fFarX1 = fWorldX + cosf(fWorldA - fFovHalf) * fFar;
    float fFarY1 = fWorldY + sinf(fWorldA - fFovHalf) * fFar;

    float fNearX1 = fWorldX + cosf(fWorldA - fFovHalf) * fNear;
    float fNearY1 = fWorldY + sinf(fWorldA - fFovHalf) * fNear;

    float fFarX2 = fWorldX + cosf(fWorldA + fFovHalf) * fFar;
    float fFarY2 = fWorldY + sinf(fWorldA + fFovHalf) * fFar;

    float fNearX2 = fWorldX + cosf(fWorldA + fFovHalf) * fNear;
    float fNearY2 = fWorldY + sinf(fWorldA + fFovHalf) * fNear;

    float scr_H = (float)this->window->getHeight();
    float scr_W = (float)this->window->getWidth();
    float floorsOffset = (int)scr_H / 4;

    int offsetY = 1;
    int offsetX = 1;

    float sampDepthDelta = 1.0f / floorsOffset;
    float sampWidthDelta = 1.0f / scr_W;

    for (int y = 1; y < floorsOffset; y++)
    {
        float fSampleDepth = sampDepthDelta * y;

        // create points representing the scanline
        float fStartX = (fFarX1 - fNearX1) / (fSampleDepth) + fNearX1;
        float fStartY = (fFarY1 - fNearY1) / (fSampleDepth) + fNearY1;

        float fEndX = (fFarX2 - fNearX2) / (fSampleDepth) + fNearX2;
        float fEndY = (fFarY2 - fNearY2) / (fSampleDepth) + fNearY2;

        for (int x = 1; x < scr_W; x += offsetX)
        {
            // sample width
            float fSampleWidth = sampWidthDelta * x;

            // find coords - interpolating
            float fSampleX = (fEndX - fStartX) * fSampleWidth + fStartX;
            float fSampleY = (fEndY - fStartY) * fSampleWidth + fStartY;

            fSampleX = fmod(fSampleX, 1.0f);
            fSampleY = fmod(fSampleY, 1.0f);

            int sx = (int)(fSampleX * (float)tex->getWidth());
            int sy = (int)(fSampleY * (float)tex->getHeight() - 1.0f);

            SDL_Rect texPart;
            texPart = {sx, sy, offsetX, offsetY};
            tex->render(this->window, x, y + (int)scr_H/2 + floorsOffset, offsetX, offsetY, &texPart);
            // RGB::rgb color = mGround->SampleRGBColor(fSampleX, fSampleY);
            // SDL_SetRenderDrawColor(this->window->renderer, color.r, color.g, color.b, 255);            
            // SDL_RenderDrawPoint(this->window->renderer, x, y + (int)(scr_H / 2));
        }
    }

    if (keyboard->printData)
    {
        this->debugging();
    }
}

void TestsModule::debugging()
{

    this->keyboard->printData = false;
}

void TestsModule::cleanup() {}
