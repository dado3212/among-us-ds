#ifndef PLAYER_H
#define PLAYER_H
class Player {
    private:
        float y = 50;
        float x = 80;
        bool alive = true;

    public:
        float getY();
        bool isAlive();
        void setY(float y);
        void setAlive(bool alive);
        float getX();
        void setX(float x);
};
#endif