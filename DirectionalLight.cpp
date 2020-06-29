#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
    lightTransform = glm::mat4(1.0f);
}

DirectionalLight::~DirectionalLight() {
    
}

DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
    GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity) {

    direction = glm::vec3(xDir, yDir, zDir);
    lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
    lightTransform = glm::mat4(1.0f);
}

void DirectionalLight::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
    GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::calculateLightTransform() {
    // Worldup can fuck up if looking straight up or straight down
    return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}