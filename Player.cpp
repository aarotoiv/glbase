#include "Player.h"

Player::Player() {
    head = new Cube(1.0f, 1.0f, 1.0f);
    body = new Cube(1.0f, 0.0f, 0.0f);
    head->translate(0.0f, 0.3f, 0.02f);
    head->scale(0.1f, 0.1f, 0.1f);
    body->scale(0.06f, 0.2f, 0.06f);
    head->setModel();
    body->setModel();
}

Player::~Player() {

}

void Player::render(GLuint uniformModel) {
    head->render(uniformModel);
    body->render(uniformModel);
}