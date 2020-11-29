#ifndef MAP_H
#define MAP_H
class Map {
    private:
        float y = 0;
        float x = 0;

    public:
        float getY();
        void setY(float y);

        float getX();
        void setX(float x);
};
#endif