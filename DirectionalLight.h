#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, 
					GLfloat red, GLfloat green, GLfloat blue,
					GLfloat aIntensity, GLfloat dIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	glm::mat4 calculateLightTransform();

	~DirectionalLight();

private:
	glm::vec3 direction;
};

#endif