#ifndef MAP_HPP
#define MAP_HPP

#include <SDL.h>
#include <vector>

#include "../macros.hpp"
#include "../sdl_engine/window.hpp"
#include "../math/myMathLib.hpp"

class Cell {
public:
    SDL_Rect rect;
    char value;
};

class Map {
public:
    
    // Empty constructor
    Map();
    
    bool loadMap(const char *filePath);
    
    void renderMiniMap(Window *w, SDL_Rect *VPminimap, SDL_Rect *VPworld);

    std::vector<Cell> mMapCells;
    Cell getCell(vec2 vec);
    
    inline unsigned int getWidth() { return mWidth; }
    inline unsigned int getHeight() { return mHeight; }
    inline vec2 getPlayerPos() { return mInitPlayerPos; }
    inline float getMaxRayL() { return mMaxRayLength; }
    inline int getCellSize() { return mCellSize; }
    inline int getPlayerSize() { return mPlayerSize; }
    inline int getMinMapSize() { return mMinMapSize; }

private:
    unsigned int mWidth;
    unsigned int mHeight;
    int mCellSize;
    int mMinMapSize;
    vec2 mInitPlayerPos;
    int mPlayerSize;
    float mMaxRayLength;
};

#endif