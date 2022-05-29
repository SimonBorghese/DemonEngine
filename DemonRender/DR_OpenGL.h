//
// Created by simon on 5/23/22.
//

#ifndef DEMONENGINE_DR_OPENGL_H
#define DEMONENGINE_DR_OPENGL_H

#include <SDL2/SDL.h>
#include <iostream>

namespace DemonRender {

    class DR_OpenGL {
    public:
        DR_OpenGL();

        void initOpenGL(int viewportX, int viewportY);

        void clear();

    private:
    };

} // DemonRender

#endif //DEMONENGINE_DR_OPENGL_H
