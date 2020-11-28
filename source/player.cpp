#include "player.h"

#define WALK_ANIM_RATE 3 // Update the walk animation every 3 movements
#define WALK_FRAME_COUNT 12 // Number of walk frames

float Player::getY() {
    return this->y;
}

float Player::getX() {
    return this->x;
}

bool Player::isAlive() {
    return this->alive;
}

void Player::setY(float y) {
    this->y = y;
}

void Player::setX(float x) {
    this->x = x;
}

void Player::setAlive(bool alive) {
    this->alive = alive;
}

int Player::getAnimFrame() {
    return this->animFrame;
}

void Player::nextFrame() {
    if (this->progressFrame % WALK_ANIM_RATE == 0) {
        this->animFrame = (this->animFrame + 1) % WALK_FRAME_COUNT;
    }
    this->progressFrame += 1;
}