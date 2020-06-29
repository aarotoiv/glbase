#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader {
    public:
        Shader();

        void createFromString(const char* vertexCode, const char* fragmentCode);
        void createFromFiles(const char* vertexLocation, const char* fragmentLocation);

        std::string readFile(const char* fileLocation);

        GLuint getProjectionLocation();
        GLuint getModelLocation();
        GLuint getViewLocation();
        GLuint getAmbientIntensityLocation();
        GLuint getAmbientColorLocation();
        GLuint getDiffuseIntensityLocation();
        GLuint getDirectionLocation();
        GLuint getSpecularIntensityLocation();
        GLuint getShininessLocation();
        GLuint getEyePositionLocation();

        void setDirectionalLight(DirectionalLight * dLight);
        void setPointLights(PointLight * pLight, unsigned int lightCount);
        void setSpotLights(SpotLight * sLight, unsigned int lightCount);
        void setTexture(GLuint textureUnit);
        void setDirectionalShadowMap(GLuint textureUnit);
        void setDirectionalLightTransform(glm::mat4 *lTransform);

        void useShader();
        void clearShader();

        ~Shader();

    private:
        int pointLightCount;
        int spotLightCount;

        GLuint shaderId, uniformProjection, uniformModel, uniformView, uniformEyePosition,
        uniformSpecularIntensity, uniformShininess,
        uniformTexture,
        uniformDirectionalLightTransform, uniformDirectionalShadowMap;

        struct {
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformDirection;  
        } uniformDirectionalLight;

        GLuint uniformPointLightCount;
        struct UniformPointLight {
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformPosition;  
            GLuint uniformConstant;
            GLuint uniformLinear;
            GLuint uniformExponent;
        };
        UniformPointLight uniformPointLight[MAX_POINT_LIGHTS];

        GLuint uniformSpotLightCount;
         struct UniformSpotLight {
            GLuint uniformColor;
            GLuint uniformAmbientIntensity;
            GLuint uniformDiffuseIntensity;

            GLuint uniformPosition;  
            GLuint uniformConstant;
            GLuint uniformLinear;
            GLuint uniformExponent;

            GLuint uniformDirection;
            GLuint uniformEdge;
        };
        UniformSpotLight uniformSpotLight[MAX_SPOT_LIGHTS]; 

        

        void compileShader(const char* vertexCode, const char* fragmentCode);
        void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};


#endif