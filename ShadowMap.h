#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool init(unsigned int width, unsigned int height);

	virtual void write();

	virtual void read(GLenum TextureUnit);

	GLuint getShadowWidth() { return shadowWidth; }
	GLuint getShadowHeight() { return shadowHeight; }

	~ShadowMap();
protected:
	GLuint FBO, shadowMap;
	GLuint shadowWidth, shadowHeight;
};

#endif