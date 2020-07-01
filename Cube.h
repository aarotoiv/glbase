#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"

#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube {
    public:
        Cube(GLfloat r, GLfloat g, GLfloat b);

        void render();
        
        ~Cube();

    private:
        GLfloat *calculateNormals(GLfloat *destination, GLfloat *vertices, GLuint *indices, GLuint indexCount, GLfloat r, GLfloat g, GLfloat b);
        GLfloat vertexData[36 * 9];
        Mesh *mesh;
};


#endif