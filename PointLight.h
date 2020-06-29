#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light {
    public:
        PointLight();
        PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
            GLfloat xPos, GLfloat yPos, GLfloat zPos,
            GLfloat con, GLfloat lin, GLfloat exp);

        void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
            GLfloat diffuseIntensityLocation, GLfloat positionLocation,
            GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

        ~PointLight();
    protected:
        glm::vec3 position;

        GLfloat constant, linear, exponent;
};

#endif