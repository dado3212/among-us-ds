#include "player.h"

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