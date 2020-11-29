#include "player.h"

#define WALK_ANIM_RATE 3 // Update the walk animation every 3 movements
#define WALK_FRAME_COUNT 12 // Number of walk frames

int Player::getAnimFrame() {
    return this->animFrame;
}

void Player::nextFrame(bool moving) {
    if (moving) {
        if (this->progressFrame % WALK_ANIM_RATE == 0) {
            this->animFrame = (this->animFrame + 1) % WALK_FRAME_COUNT;
        }
        this->progressFrame += 1;
    } else {
        // Most stationary frame, this should probably be frame 12 or something with a real stand still image
        this->progressFrame = 0;
        this->animFrame = 11;
    }
}

 bool Player::isFacingRight() {
    return this->facingRight;
 }

void Player::setFacingRight(bool facingRight) {
    this->facingRight = facingRight;
}