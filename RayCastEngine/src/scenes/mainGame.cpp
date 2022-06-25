#include "mainGame.hpp"

#include <cmath>

/**
 * draw triangle
 */
void drawTriangle(Window *w, Triangle t)
{
    SDL_RenderDrawLineF(w->renderer, t.p2.x, t.p2.y, t.p3.x, t.p3.y); // nose to right leg
    SDL_RenderDrawLineF(w->renderer, t.p3.x, t.p3.y, t.p1.x, t.p1.y); // right to left leg
    SDL_RenderDrawLineF(w->renderer, t.p1.x, t.p1.y, t.p2.x, t.p2.y); // left to nose
}

MainGame::MainGame(Window *window, Map *map, Mouse *mouse, Keyboard *keyboard)
    : window(window), map(map), mouse(mouse), keyboard(keyboard)
{

    // initialization
    this->player.init(this->map);

    this->setMinimapPort();
    this->setWorldPort();

    this->mouse->firstMouse = true;

    mNumRays = map->getWidth() * map->getCellSize();

    // Load gun1
    // this->gun1 = new Texture(this->window, "assets/weapons/mp4.png", 1);

    // create wall sprite
    this->wallSprite = Sprite(new Texture(this->window, "assets/Walls/scify_walls_1.png", 0));

    // create floor sprite
    this->floorSprite = Sprite(new Texture(this->window, "assets/Floors/scify_floor_3.png", 0));
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
    this->map->renderMiniMap(this->window, &this->VPminimap, &this->VPworld);

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
    for (int i = 0; i < mNumRays; i++)
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

    /*
    ====================================
    Player stuff
    ====================================
    */

    // player updates and translations
    this->player.updatePos(this->keyboard, dt, this->map);
    this->player.updateCurrentAngle(this->mouse, dt);
    this->player.translate(map->getPlayerSize(), mNumRays);

    // raycasting
    this->player.rayCastDDD(this->map);

    // render world:
    SDL_RenderSetViewport(this->window->renderer, &this->VPworld);

    SDL_Rect texturePart;

    float scrHeigth = this->VPworld.h;
    float scrWidht = this->VPworld.w;
    int halfScreen = (int)scrHeigth >> 1;

    /*
    =====================================================
    render Walls, Floors and Ceilings to texture buffer and then render to screen
    =====================================================
    */

    // create target texture
    Texture texTarget = Texture(this->window, "assets/BufferTextures/space960x640.png", 0);

    int texWidth = texTarget.getWidth(); // should be pitch / 4
    int texHeight = texTarget.getHeight();
    texTarget.lockTexture();
    Uint32 *buffer = (Uint32 *)texTarget.getPixels();
    float pDistToScreen = map->getPlayerDistToScr();

    /*
    ====================================
    render walls based on rays length
    ====================================
    */

    float wallWidth = (float)(map->getWidth() * map->getCellSize()) / (float)mNumRays;
    int midRow = halfScreen / map->getCellSize();

    SDL_FRect wall;
    for (int x = 0; x < mNumRays; x++)
    {
        if (this->player.rays[x].results.hit)
        {
            // get offset for rendering texture
            int cellX = this->player.rays[x].results.intersection.x;
            int cellY = this->player.rays[x].results.intersection.y;
            SDL_Rect cell = this->map->getCell(vec2(cellX, cellY)).rect;
            
            float tx;
            if (this->player.rays[x].results.HitDir == 0) // horizontal hit
                tx = this->player.rays[x].results.intersection.y - cell.y;
            else // vertical hit
                tx = this->player.rays[x].results.intersection.x - cell.x;
            
            tx /= map->getCellSize();
            tx *= this->wallSprite.nWidth;

            // fix fish eye
            float aDist = clampAngle(player.rays[x].angle - player.angle);
            float modDistance = player.rays[x].distance * cosf(deg2rad(aDist));

            // compute wall height
            float wallHeight = ((float)map->getCellSize() * map->getPlayerDistToScr()) / modDistance;
            float yStep = this->wallSprite.nHeight / wallHeight;
            float offY = 0;
            if (wallHeight >= scrHeigth)
            {
                offY = (wallHeight - scrHeigth) / 2;
                wallHeight = scrHeigth;
            }
            float wallOffset = halfScreen - wallHeight / 2;
            
            float ty = offY * yStep;
            
            // SAMPLE FROM WALL TEXTURES
            for (int y = wallOffset; y < wallOffset + wallHeight; y++)
            {
                Uint32 sampleColor = SDL_MapRGB(texTarget.getPixFormat(), wallSprite.GetRGBColor(tx, ty).r, wallSprite.GetRGBColor(tx, ty).g, wallSprite.GetRGBColor(tx, ty).b);
                buffer[y * texWidth + x] = sampleColor;
                ty += yStep;
            }

            // SAMPLE FROM FLOOR TEXTURE
            float deg = deg2rad(this->player.rays[x].angle);
            float raFix = cosf(deg2rad(clampAngle(this->player.rays[x].angle - this->player.angle)));
            for (int y = wallOffset + wallHeight; y < scrHeigth; y++)
            {
                // i dont know why this numbers: 158 * 2 * 32 work with 128 by 128 textures, 
                // ill leave this for now and come back to find out
                float dy = y - (halfScreen);    
                tx = this->player.pos.x + cosf(deg) * 158 * 2 * 32/** floorSprite.nWidth*/ / dy / raFix;
                ty = this->player.pos.y - sinf(deg) * 158 * 2 * 32/** floorSprite.nHeight*/ / dy / raFix;
                tx = (int)(tx) & (floorSprite.nWidth - 1);
                ty = (int)(ty) & (floorSprite.nHeight - 1);

                Uint32 sampleColor = SDL_MapRGB(texTarget.getPixFormat(), floorSprite.GetRGBColor(tx, ty).r, floorSprite.GetRGBColor(tx, ty).g, floorSprite.GetRGBColor(tx, ty).b);
                buffer[y * texWidth + x] = sampleColor;
            }
        }
    }

    texTarget.unlockTexture();
    texTarget.render(this->window, 0, 0);

    // render gun
    // this->gun1->render(this->window, this->window->getWidth() - this->gun1->getWidth(), this->window->getHeight() - this->gun1->getHeight());

    // render croshair
    SDL_SetRenderDrawColor(this->window->renderer, 150, 255, 51, 255);
    vec2 P1 = vec2(this->window->getWidth() / 2 - 5.0f, this->window->getHeight() / 2);
    vec2 P2 = vec2(this->window->getWidth() / 2 + 5.0f, this->window->getHeight() / 2);
    SDL_RenderDrawLineF(this->window->renderer, P1.x, P1.y, P2.x, P2.y);
    P1 = vec2(this->window->getWidth() / 2, this->window->getHeight() / 2 + 5.0f);
    P2 = vec2(this->window->getWidth() / 2, this->window->getHeight() / 2 - 5.0f);
    SDL_RenderDrawLineF(this->window->renderer, P1.x, P1.y, P2.x, P2.y);

    if (this->keyboard->printData)
    {
        this->debugging();
        this->mouse->printData = true;
    }
}

void MainGame::debugging()
{
    for (int y = 0; y < map->getHeight(); y++)
    {
        for (int x = 0; x < map->getHeight(); x++)
        {
            std::cout << map->mMapCells[y * map->getWidth() + x].value << " ";
        }
        std::cout << "\n";
    }

    this->keyboard->printData = false;
}

void MainGame::setMinimapPort()
{
    this->VPminimap.x = 0;
    this->VPminimap.y = 0;
    this->VPminimap.w = map->getCellSize() * map->getMinMapSize();
    this->VPminimap.h = map->getCellSize() * map->getMinMapSize();
}
void MainGame::setWorldPort()
{
    this->VPworld.x = 0;
    this->VPworld.y = 0;
    this->VPworld.w = this->window->getWidth();
    this->VPworld.h = this->window->getHeight();
}

void MainGame::cleanup()
{
    this->player.clearRays();
}
