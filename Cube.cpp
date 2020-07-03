#include "Cube.h"

Cube::Cube(GLfloat r, GLfloat g, GLfloat b) {
    model = glm::mat4(1.0f);
    translateX = 0.0f;
    translateY = 0.0f;
    translateZ = 0.0f;
    rotationRadians = 0.0f;
    rotationX = 1.0f;
    rotationY = 1.0f;
    rotationZ = 1.0f;
    scaleX = 1.0f;
    scaleY = 1.0f;
    scaleZ = 1.0f;

    GLfloat vertices[24] = {
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    GLuint indices[36] = {
        // Front
        1, 3, 0,
        0, 3, 2,
        // Back
        4, 6, 5,
        5, 6, 7,
        // Left
        0, 2, 4,
        4, 2, 6,
        // Right
        5, 7, 1,
        1, 7, 3,
        // Top
        5, 1, 4,
        4, 1, 0,
        // Bottom
        6, 2, 7,
        7, 2, 3
    };

    calculateNormals(vertexData, vertices, indices, 36, r, g, b);

    mesh = new Mesh();
    mesh->createMesh(vertexData, 36 * 9);
}

GLfloat *Cube::calculateNormals(GLfloat *destination, GLfloat *vertices, GLuint *indices, GLuint indexCount, GLfloat r, GLfloat g, GLfloat b) {
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

void Cube::render(GLuint uniformModel) {
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    mesh->renderRaw(36);
}

void Cube::translate(GLfloat xTranslation, GLfloat yTranslation, GLfloat zTranslation) {
    translateX = xTranslation;
    translateY = yTranslation;
    translateZ = zTranslation;
}

void Cube::rotate(GLfloat radians, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation) {
    rotationRadians = radians;
    rotationX = xRotation;
    rotationY = yRotation;
    rotationZ = zRotation;
}

void Cube::scale (GLfloat xScale, GLfloat yScale, GLfloat zScale) {
    scaleX = xScale;
    scaleY = yScale;
    scaleZ = zScale;
}

void Cube::setModel() {
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(translateX, translateY, translateZ));
    model = glm::rotate(model, rotationRadians, glm::vec3(rotationX, rotationY, rotationZ));
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
}

Cube::~Cube() {
    delete mesh;
}