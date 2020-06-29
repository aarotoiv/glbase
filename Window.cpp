#include "Window.h";

Window::Window() {
    width = 800;
    height = 600;
    bufferWidth = 0;
    bufferHeight = 0;
    quit = false;
    mouseX = 0.0f;
    mouseY = 0.0f;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
    std::cout << "Created window" << std::endl;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;
    bufferWidth = 0;
    bufferHeight = 0;
    quit = false;
    mouseX = 0.0f;
    mouseY = 0.0f;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }

    std::cout << "Created window with params" << std::endl;
}

Window::~Window() {
    SDL_GL_DeleteContext(glContext);
}

int Window::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL could not initialize\n");
        return 1;
    }
    // Create a window
    window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating window.\n");
        return 2;
    }

    // Lock mouse in window
    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GetWindowSize(window, &bufferWidth, &bufferHeight);

    // SDL OpenGL settings
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    glewExperimental = GL_TRUE;

    // SDL OpenGL context
    glContext = SDL_GL_CreateContext(window);
    // SDL v-sync
    SDL_GL_SetSwapInterval(1);

    // GLEW
    glewInit();
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);
}

void Window::pollEvent() {
    SDL_Event windowEvent;
    while (SDL_PollEvent(&windowEvent)) {
        if (windowEvent.type == SDL_QUIT) {
            setShouldClose(true);
            break;
        }

        if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym > -1 && windowEvent.key.keysym.sym < 1025) {
            handleKey(windowEvent.key.keysym.sym, true);
        }

        if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym > -1 && windowEvent.key.keysym.sym < 1025) {
            handleKey(windowEvent.key.keysym.sym, false);
        }

        if (windowEvent.type == SDL_MOUSEMOTION) {
            handleMouse(windowEvent.motion.xrel, -windowEvent.motion.yrel);
        }
    }
}

void Window::handleKey(int code, bool down) {
    keys[code] = down;
    
    // Debug
    for (int i = 0; i < 1024; i++) {
        if (keys[i])
            std::cout << i << std::endl;
    }
}

void Window::handleMouse(float x, float y) {
    mouseX = x;
    mouseY = y;
}