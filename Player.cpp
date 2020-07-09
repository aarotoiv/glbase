#include "Player.h"

Player::Player() {
    head = new Cube(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    body = new Cube(1.0f, 0.0f, 0.0f);
    leftHand = new Cube(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    rightHand = new Cube(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    leftLeg = new Cube(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    rightLeg = new Cube(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    head->translate(0.0f, 0.3f, 0.02f);
    head->scale(0.1f, 0.1f, 0.1f);
    body->scale(0.06f, 0.2f, 0.06f);

    rightHand->translate(-0.09f, 0.04f, 0.0f);
    leftHand->translate(0.09f, 0.04f, 0.0f);
    rightHand->scale(0.03f, 0.12f, 0.03f);
    leftHand->scale(0.03f, 0.12f, 0.03f);

    leftLeg->translate(0.045f, -0.26f, 0.0f);
    rightLeg->translate(-0.045f, -0.26f, 0.0f);
    leftLeg->scale(0.015f, 0.06f, 0.015f);
    rightLeg->scale(0.015f, 0.06f, 0.015f);

    head->setModel();
    body->setModel();

    leftHand->setModel();
    rightHand->setModel();

    leftLeg->setModel();
    rightLeg->setModel();

    GLfloat vertices[42] = {
        // Shoulder topright 0
        0.0f, 0.0f, 0.0f,
        // Shoulder topleft 1
        0.0f, 0.0f, -1.0f,
        // Shoulder botleft 2
        0.0f, -1.0f, -1.0f,
        // Shoulder botright 3
        0.0f, -1.0f, 0.0f,
        // elbow backtop 4
        -2.0f, -2.0f, -1.0f,
        // elbow fronttop 5
        -2.0f, -2.0f, 0.0f,
        // elbow backright 6
        -1.0f, -3.0f, -1.0f,
        // elbow frontright 7
        -1.0f, -3.0f, 0.0f,
        //elbow backbottom 8
        -2.0f, -4.0f, -1.0f,
        //elbow backleft 9
        -3.0f, -3.0f, -1.0f,
        // arm left 10
        -3.0f, -3.0f, 1.0f,
        // arm top 11
        -2.0f, -2.0f, 1.0f, 
        // arm right 12
        -1.0f, -3.0f, 1.0f,
        // arm bot 13
        -2.0f, -4.0f, 1.0f,    
    };

    GLuint indices[60] = {
        //elbowconnection right
        0, 3, 7,
        7, 5, 0,
        // elbowconnection top
        1, 0, 5,
        5, 4, 1,
        //elbowconnection left
        1, 6, 2,
        4, 6, 1,
        // elbowconnection bottom
        3, 2, 6,
        6, 7, 3,

        5, 7, 12,
        12, 11, 5,

        9, 5, 10,
        11, 10, 5,

        13, 9, 8,
        13, 9, 10,

        7, 8, 13, 
        13, 12, 7,

        4, 8, 6,
        9, 8, 4,

        9, 4, 5,

        8, 7, 6


    };
    generateHands(playerHand.vertexData, vertices, indices, 60, 1.0f, 1.0f, 1.0f);
    playerHand.mesh = new Mesh();
    playerHand.mesh->createMesh(playerHand.vertexData, 60 * 9);
    playerHand.model = glm::mat4(1.0f);
    playerHand.model = glm::translate(playerHand.model, glm::vec3(0.0f, 2.0f, 0.0f));
    playerHand.model = glm::scale(playerHand.model, glm::vec3(0.5f, 0.5f, 0.5f));
}

Player::~Player() {

}

void Player::generateHands(GLfloat *destination, GLfloat *vertices, GLuint *indices, GLuint indexCount, GLfloat r, GLfloat g, GLfloat b) {
    int offset = 0;
    for (size_t i = 0; i < indexCount; i += 3) {
        GLfloat x1 = vertices[indices[i] * 3]; GLfloat y1 = vertices[indices[i] * 3 + 1]; GLfloat z1 = vertices[indices[i] * 3 + 2];
        GLfloat x2 = vertices[indices[i + 1] * 3]; GLfloat y2 = vertices[indices[i + 1] * 3 + 1]; GLfloat z2 = vertices[indices[i + 1] * 3 + 2];
        GLfloat x3 = vertices[indices[i + 2] * 3]; GLfloat y3 = vertices[indices[i + 2] * 3 + 1]; GLfloat z3 = vertices[indices[i + 2] * 3 + 2];

        glm::vec3 v1(x2 - x1, y2 - y1, z2 - z1);
		glm::vec3 v2(x3 - x1, x3 - x1, x3 - x1);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
        
        for (size_t j = 0; j < 3; j++) {
            destination[offset++] = vertices[indices[i+j] * 3];
            destination[offset++] = vertices[indices[i+j] * 3 + 1];
            destination[offset++] = vertices[indices[i+j] * 3 + 2];
            destination[offset++] = r;
            destination[offset++] = g;
            destination[offset++] = b;
            destination[offset++] = -normal.x;
            destination[offset++] = -normal.y;
            destination[offset++] = -normal.z;
        }
    } 
}

void Player::render(GLuint uniformModel) {
    head->render(uniformModel);
    body->render(uniformModel);
    leftHand->render(uniformModel);
    rightHand->render(uniformModel);
    leftLeg->render(uniformModel);
    rightLeg->render(uniformModel);

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(playerHand.model));
    playerHand.mesh->renderRaw(60);
}