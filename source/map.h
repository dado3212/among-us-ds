#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

class Map {
    private:
        float x = 896;
        float y = 384;
        std::string mapName = "skeld";

    public:
        float getY();
        void setY(float y);

        float getX();
        void setX(float x);
        std::vector<std::string> getBackgrounds();
};
#endif