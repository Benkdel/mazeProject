#include "map.hpp"

#include <string>
#include <vector>
#include <algorithm>

#include <fstream>
#include <sstream>
#include <iostream>

#include <cmath>

Map::Map() {}

void Map::renderMiniMap(Window *w, SDL_Rect *VPminimap, SDL_Rect *VPworld)
{
    SDL_RenderSetViewport(w->renderer, VPminimap);

    float xConvRatio = ((float)VPminimap->w / (float)VPworld->w);
    float yConvratio = ((float)VPminimap->h / (float)VPworld->h);

    float cellWidth = (float)mCellSize * xConvRatio;
    float cellHeight = (float)mCellSize * yConvratio;

    // render walls
    SDL_Rect worldCell;
    for (int r = 0; r < mHeight; r++)
    {
        for (int c = 0; c < mWidth; c++)
        {
            worldCell = mMapCells[r * mWidth + c].rect;
            worldCell.x *= xConvRatio;
            worldCell.y *= yConvratio;
            worldCell.w = cellWidth;
            worldCell.h = cellHeight;

            if (mMapCells[r * mWidth + c].value == '#')
            {
                SDL_SetRenderDrawColor(w->renderer, 0, 204, 204, 255);
                SDL_RenderFillRect(w->renderer, &worldCell);
            }
            else
            {
                SDL_SetRenderDrawColor(w->renderer, 0, 204, 204, 85);
                SDL_RenderDrawRect(w->renderer, &worldCell);
            }
        }
    }
}

bool Map::loadMap(const char *filePath)
{
    /*
    =========================
    Load Map file
    =========================
    */
    std::vector<int> map;

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        std::cout << "Could not open file -- " << filePath << "\n";
        return false;
    }

    std::string line;
    std::string::iterator c;
    bool witdhSaved = false;
    while (std::getline(inputFile, line))
    {
        char id = line[0];

        switch (id)
        {
        case 'M':
            for (c = line.begin() + 2; c < line.end(); c++)
            {
                if (*c != ' ') map.push_back((int)*c - 48);
            }
            if (!witdhSaved)
            {
                mWidth = map.size();
                witdhSaved = true;
            }
            break;
        case 'C':
            mCellSize = stoi((line.substr(2, line.size() - 2)));
            break;
        case 'P':
            mInitPlayerPos.x = (int)line[2] - 48;
            mInitPlayerPos.y = (int)line[4] - 48;
            break;
        case 'S':
            mPlayerSize = stoi((line.substr(2, line.size() - 2)));
            break;
        case 'N':
            mMinMapSize = (int)line[2] - 48;
            break;
        default:
            break;
        }
    }
    inputFile.close();

    mHeight = map.size() / mWidth;

    mMaxRayLength = sqrt(mWidth * mWidth + mHeight * mHeight) * mCellSize;

    // convert player position from cells to screen coordinates
    mInitPlayerPos.x *= mCellSize;
    mInitPlayerPos.y *= mCellSize;

    mPlayerDistToScreen = (mWidth * mCellSize) / (atan(FOV / 2));

    /*
    =========================
    Populate CELLs array
    =========================
    */
   mMapCells.clear();
    for (int r = 0; r < mHeight; r++)
    {
        for (int c = 0; c < mWidth; c++)
        {
            int x1 = c * mCellSize;
            int y1 = r * mCellSize;

            mMapCells.push_back(Cell());
            mMapCells[r * mWidth + c].rect = {x1, y1, mCellSize, mCellSize};

            if (map[r * mWidth + c] == 1)
                mMapCells[r * mWidth + c].value = '#';
            else
                mMapCells[r * mWidth + c].value = ' ';
        }
    }
    return true;
}

Cell Map::getCell(vec2 vec)
{
    int cell_X = vec.x / mCellSize;
    int cell_Y = vec.y / mCellSize;

    return this->mMapCells[cell_Y * mWidth + cell_X];
}