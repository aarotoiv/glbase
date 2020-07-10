#ifndef PLAYER_H
#define PLAYER_H

#include "Cube.h"
#include <algorithm>


class Player {
    public:
        Player();
        void render(GLuint uniformModel);
        void generateHands(GLfloat *destination, GLfloat *vertices, GLuint *indices, GLuint indexCount, GLfloat r, GLfloat g, GLfloat b, bool inverted);
        ~Player();

    private:
        Cube *head;
        Cube *body;
        Cube *leftLeg;
        Cube *rightLeg;
        struct {
            GLfloat vertices[36];
            GLfloat indices[48];
            GLfloat vertexData[60 * 9];
            Mesh *mesh;
            glm::mat4 model;
        } leftHand, rightHand;
};

#endif
