#include "map.h"
#include "constants.h"
#include "tiledbg.h"

float Map::getY() {
    return this->y;
}

float Map::getX() {
    return this->x;
}

void Map::setY(float y) {
    this->y = y;
}

void Map::setX(float x) {
    this->x = x;
}

void Map::setMap(std::string map) {
    if (this->currentMap.compare(map) != 0) {
        this->currentMap = map;
        CreateTiledBg(0, 0, map.c_str());
    }
}

/**
 * Takes in the current position of the map.
 * Then it looks to the four corners of what is visible.
 * It then converts those to the corresponding tiles, and returns the list of tiles that need to be loaded.
 * 
 * NOTE: This probably mishandles edge cases around boundaries.
 */ 
std::vector<std::string> Map::getBackgrounds() {
    std::vector<std::string> backgrounds;
    float tl_x = this->x - (SCREEN_WIDTH / 2);
    float tl_y = this->y - (SCREEN_HEIGHT / 2);

    float bl_x = this->x - (SCREEN_WIDTH / 2);
    float bl_y = this->y + (SCREEN_HEIGHT / 2);

    float tr_x = this->x + (SCREEN_WIDTH / 2);
    float tr_y = this->y - (SCREEN_HEIGHT / 2);

    float br_x = this->x + (SCREEN_WIDTH / 2);
    float br_y = this->y + (SCREEN_HEIGHT / 2);

    backgrounds.push_back(this->mapName + "_" + std::to_string((int)(tl_y/MAP_SIZE)) + std::to_string((int)(tl_x/MAP_SIZE)));
    backgrounds.push_back(this->mapName + "_" + std::to_string((int)(bl_y/MAP_SIZE)) + std::to_string((int)(bl_x/MAP_SIZE)));
    backgrounds.push_back(this->mapName + "_" + std::to_string((int)(tr_y/MAP_SIZE)) + std::to_string((int)(tr_x/MAP_SIZE)));
    backgrounds.push_back(this->mapName + "_" + std::to_string((int)(br_y/MAP_SIZE)) + std::to_string((int)(br_x/MAP_SIZE)));
    return backgrounds;
}