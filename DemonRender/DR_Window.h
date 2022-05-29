//
// Created by simon on 5/23/22.
//

#ifndef DEMONENGINE_DR_WINDOW_H
#define DEMONENGINE_DR_WINDOW_H
#include "DR_STD.h"

#include <SDL2/SDL.h>
#include <functional>
#include <string>


namespace DemonRender {
    class DR_Window {
    public:
        DR_Window();
        void createWindow(const char *name, uint32_t width, uint32_t height);

        void destroyWindow();

        void flip();

        // Lua Functions
        void moveWindow(int newX, int newY);
    private:
        SDL_Window *_window = nullptr;
        SDL_GLContext _context = nullptr;
    };
} // DemonRender

#endif //DEMONENGINE_DR_WINDOW_H
