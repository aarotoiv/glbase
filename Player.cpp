#include "Player.h"

Player::Player() {
    movingDown = true;

    head = new Cube(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    body = new Cube(1.0f, 0.0f, 0.0f);
    leftLeg = new Cube(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    rightLeg = new Cube(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    head->translate(0.0f, 0.25f, 0.02f);
    head->scale(0.1f, 0.1f, 0.1f);
    body->scale(0.06f, 0.15f, 0.06f);

    leftLeg->translate(0.045f, -0.21f, 0.0f);
    rightLeg->translate(-0.045f, -0.21f, 0.0f);
    leftLeg->scale(0.015f, 0.06f, 0.015f);
    rightLeg->scale(0.015f, 0.06f, 0.015f);

    head->setModel();
    body->setModel();

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
        -1.5f, -2.5f, -1.0f,
        // elbow fronttop 5
        -1.5f, -2.5f, 0.0f,
        // elbow backright 6
        -1.0f, -3.0f, -1.0f,
        // elbow frontright 7
        -1.0f, -3.0f, 0.0f,
        //elbow backbottom 8
        -1.5f, -3.5f, -1.0f,
        //elbow backleft 9
        -2.0f, -3.0f, -1.0f,
        // arm left 10
        -2.0f, -3.0f, 2.0f,
        // arm top 11
        -1.5f, -2.5f, 2.0f, 
        // arm right 12
        -1.0f, -3.0f, 2.0f,
        // arm bot 13
        -1.5f, -3.5f, 2.0f,    
    };

    GLuint indices[66] = {
        //elbowconnection right
        3, 0, 7,
        7, 0, 5,
        // elbowconnection top
        0, 1, 5,
        4, 5, 1,
        //elbowconnection left
        1, 2, 6,
        6, 4, 1,
        // elbowconnection bottom
        6, 2, 3,
        3, 7, 6,

        12, 7, 5,
        5, 11, 12,

        9, 10, 5,
        11, 5, 10,

        13, 9, 8,
        10, 9, 13,

        13, 8, 7, 
        7, 12, 13,

        6, 8, 4,
        4, 8, 9,

        5, 4, 9,
        6, 7, 8,

        // hand tips
        10, 13, 11,
        11, 13, 12
    };

    generateHands(rightHand.vertexData, vertices, indices, 66, 241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    rightHand.mesh = new Mesh();
    rightHand.mesh->createMesh(rightHand.vertexData, 66 * 9);
    rightHand.model = glm::mat4(1.0f);
    rightHand.model = glm::translate(rightHand.model, glm::vec3(-0.06f, 0.12f, 0.02f));
    rightHand.model = glm::scale(rightHand.model, glm::vec3(0.05f, 0.05f, 0.05f));

    rightHand.animDir = -1;
    rightHand.animCap = 2.0f;
    rightHand.xAnimCurrent = 0.0f;
    rightHand.yAnimCurrent = 0.0f;
    rightHand.zAnimCurrent = 0.0f;

    std::copy(std::begin(indices), std::end(indices), std::begin(rightHand.indices));

    // reverse indices for lefthand generation. the normals get fucked
    // TODO: fix normal calculations
    std::reverse(indices, indices + 66);

    generateHands(leftHand.vertexData, vertices, indices, 66, 241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
    leftHand.mesh = new Mesh();
    leftHand.mesh->createMesh(leftHand.vertexData, 66 * 9);
    leftHand.model = glm::mat4(1.0f);
    leftHand.model = glm::translate(leftHand.model, glm::vec3(0.06f, 0.12f, 0.02f));
    leftHand.model = glm::scale(leftHand.model, glm::vec3(-0.05f, 0.05f, 0.05f));

    std::copy(std::begin(indices), std::end(indices), std::begin(leftHand.indices));
}

Player::~Player() {
    delete head;
    delete body;
    delete leftLeg;
    delete rightLeg;
}

void Player::generateHands(GLfloat *destination, GLfloat *vertices, GLuint *indices, GLuint indexCount, GLfloat r, GLfloat g, GLfloat b) {
    int offset = 0;
    for (size_t i = 0; i < indexCount; i += 6) {
        GLfloat x1 = vertices[indices[i] * 3]; GLfloat y1 = vertices[indices[i] * 3 + 1]; GLfloat z1 = vertices[indices[i] * 3 + 2];
        GLfloat x2 = vertices[indices[i + 1] * 3]; GLfloat y2 = vertices[indices[i + 1] * 3 + 1]; GLfloat z2 = vertices[indices[i + 1] * 3 + 2];
        GLfloat x3 = vertices[indices[i + 2] * 3]; GLfloat y3 = vertices[indices[i + 2] * 3 + 1]; GLfloat z3 = vertices[indices[i + 2] * 3 + 2];
        
        glm::vec3 v1(x2 - x1, y2 - y1, z2 - z1);
		glm::vec3 v2(x3 - x1, x3 - x1, x3 - x1);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
        
        for (size_t j = 0; j < 6; j++) {
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
    leftLeg->render(uniformModel);
    rightLeg->render(uniformModel);

    if (movingDown) {
        moveHands();
    }

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(leftHand.model));
    leftHand.mesh->renderRaw(66);

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rightHand.model));
    rightHand.mesh->renderRaw(66);

}

void Player::moveHands() {
    int indicePoints[4] = {10, 11, 12, 13};
    rightHand.yAnimCurrent += 0.0005f * rightHand.animDir;
    if (rightHand.animDir == -1 && rightHand.yAnimCurrent < -rightHand.animCap) {
        rightHand.yAnimCurrent = -rightHand.animCap;
        rightHand.animDir *= -1;
    }

    if (rightHand.animDir == 1 && rightHand.yAnimCurrent > rightHand.animCap) {
        rightHand.yAnimCurrent = rightHand.animCap;
        rightHand.animDir *= -1;
    }

    for (int i = 0; i < 66; i++) {
        for (int j = 0; j < 4; j++) {
            if (rightHand.indices[i] == indicePoints[j]) {
                // Match found, modify 
                rightHand.vertexData[i * 9 + 1] += 0.0005f * rightHand.animDir;
                rightHand.vertexData[i * 9 + 2] += 0.0001f * rightHand.animDir;
            }
        }
    }

    rightHand.mesh->updateMesh(rightHand.vertexData, 66 * 9);
}