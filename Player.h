#ifndef PLAYER_H
#define PLAYER_H

#include "Cube.h"
#include <algorithm>


class Player {
    public:
        Player();
        void render(GLuint uniformModel);
        void generateHands(GLfloat *destination, GLfloat *vertices, GLuint *indices, GLuint indexCount, GLfloat r, GLfloat g, GLfloat b);
        void moveHands();
        ~Player();

    private:
        Cube *head;
        Cube *body;
        Cube *leftLeg;
        Cube *rightLeg;
        struct {
            GLfloat vertices[36];
            GLfloat indices[66];
            GLfloat vertexData[66 * 9];
            Mesh *mesh;
            glm::mat4 model;
            int animDir;
            float xAnimCurrent, yAnimCurrent, zAnimCurrent;
            float animCap;
        } leftHand, rightHand;

        bool movingDown;
};

#endif
