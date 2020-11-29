#ifndef PLAYER_H
#define PLAYER_H
class Player {
    private:
        int animFrame = 0;
        int progressFrame = 0;
        bool facingRight = true;

    public:
        int getAnimFrame();
        void nextFrame(bool moving);

        bool isFacingRight();
        void setFacingRight(bool facingRight);
};
#endif