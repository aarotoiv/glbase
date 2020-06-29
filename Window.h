#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

class Window {
    public:
        Window();
        Window(GLint windowWidth, GLint windowHeight);
        int initialize();
        GLfloat getBufferWidth() { return bufferWidth; }
        GLfloat getBufferHeight() { return bufferHeight; }
        bool getShouldClose() { return quit; }
        void setShouldClose(bool should) { quit = should; }

        bool* getKeys() { return keys; }

        GLfloat getMouseX() { GLfloat tempX = mouseX; mouseX = 0.0f; return tempX; }
        GLfloat getMouseY() { GLfloat tempY = mouseY; mouseY = 0.0f; return tempY; }

        void pollEvent();

        void swapBuffers() { SDL_GL_SwapWindow(window); }

        ~Window();

    private:
        void handleKey(int code, bool down);
        void handleMouse(float x, float y);

        SDL_Window *window;
        GLint width, height;
        GLint bufferWidth, bufferHeight;
        bool quit;
        SDL_GLContext glContext;
        bool keys[1024];

        GLfloat mouseX, mouseY;
};

#endif