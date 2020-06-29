#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include "stb_image.h"

class Texture {
    public:
        Texture();
        Texture(const char* fileLoc);
        ~Texture();

        bool loadTexture();
        bool loadTextureA();
        void useTexture();
        void clearTexture();

    private:
        GLuint textureId;
        int width, height, bitDepth;

        const char* fileLocation;
};


#endif