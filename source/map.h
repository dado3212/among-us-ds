#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

class Map {
    private:
        float x = 0; //896;
        float y = 0; //384;
        std::string mapName = "skeld";
        std::string currentMap = "";

    public:
        float getY();
        void setY(float y);

        float getX();
        void setX(float x);

        void setMap(std::string map);

        std::vector<std::string> getBackgrounds();
};
#endif