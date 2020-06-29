#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap {
    public:
        ShadowMap();
        virtual bool init(GLuint width, GLuint height);
        virtual void write();
        virtual void read(GLenum textureUnit);

        GLuint getShadowWidth() { return shadowWidth; }
        GLuint getShadowHeight() { return shadowHeight; }

        ~ShadowMap();

    protected:
        GLuint FBO, shadowMap;
        GLuint shadowWidth, shadowHeight;

};



#endif