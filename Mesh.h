#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <iostream>

class Mesh
{
public:
	Mesh();

	void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void createMesh(GLfloat *vertices, unsigned int numOfVertices);
	void renderMesh();
	void renderRaw(GLuint verticeCount);
	void clearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

#endif

