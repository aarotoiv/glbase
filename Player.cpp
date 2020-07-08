#include "Player.h"

Player::Player() {
    head = Cube(1.0f, 1.0f, 1.0f);
    body = Cube(1.0f, 0.0f, 0.0f);
}

Player::~Player() {

}

void Player::render(GLuint uniformModel) {
    head.render(uniformModel);
    body.render(uniformModel);
}