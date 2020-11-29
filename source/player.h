#ifndef PLAYER_H
#define PLAYER_H
class Player {
    private:
        float y = 50;
        float x = 80;
        int animFrame = 0;
        int progressFrame = 0;
        bool facingRight = true;

    public:
        float getY();
        void setY(float y);

        float getX();
        void setX(float x);

        int getAnimFrame();
        void nextFrame(bool moving);

        bool isFacingRight();
        void setFacingRight(bool facingRight);
};
#endif