#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"

#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
    public:
        Cube();
        Cube(GLfloat r, GLfloat g, GLfloat b);

        void render(GLuint uniformModel);
        void updateMesh();
    
        void translate(GLfloat xTranslation, GLfloat yTranslation, GLfloat zTranslation);
        void rotate(GLfloat radians, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
        void scale (GLfloat xScale, GLfloat yScale, GLfloat zScale);

        void setModel();
        
        ~Cube();

    private:
        GLfloat *calculateNormals(GLfloat *destination, GLfloat *vertices, GLuint *indices, GLuint indexCount, GLfloat r, GLfloat g, GLfloat b);
        GLfloat vertexData[36 * 9];
        Mesh *mesh;
        GLfloat rotationRadians, rotationX, rotationY, rotationZ, scaleX, scaleY, scaleZ, translateX, translateY, translateZ;
        glm::mat4 model;
};


#endif