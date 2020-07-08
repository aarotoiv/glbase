#ifndef PLAYER_H
#define PLAYER_H

#include "Cube.h"


class Player {
    public:
        Player();
        void render(GLuint uniformModel);
        ~Player();

    private:
        Cube *head;
        Cube *body;
};

#endif
