#ifndef PLAYER_H
#define PLAYER_H
class Player {
    private:
        float y = 50;
        float x = 80;
        bool alive = true;
        int animFrame = 0;
        int progressFrame = 0;

    public:
        float getY();
        bool isAlive();
        void setY(float y);
        void setAlive(bool alive);
        float getX();
        void setX(float x);
        int getAnimFrame();
        void nextFrame(bool moving);
};
#endif