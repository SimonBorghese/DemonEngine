//
// Created by simon on 1/9/23.
//

#ifndef DEMONENGINE_WINDOW_H
#define DEMONENGINE_WINDOW_H
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <DemonMacro/DemonLog.h>

namespace DGL {

    class Window {
    public:
        Window(const char *windowTitle, uint32_t width, uint32_t height, int vsync = 1);
        virtual ~Window();

        void flipWindow();
        void clearWindow(float clearR = 0.0f, float clearG = 0.0f, float clearB = 0.0f);

        void setVsync(int vsync);
        void setMouseGrab(int enable);

        uint32_t getWidth() { return _width; }
        uint32_t getHeight() { return _height; }

        SDL_Window *getWindow();
        SDL_GLContext getContext();
    private:
        std::string _windowTitle;
        uint32_t _width, _height;

        SDL_Window *_window = nullptr;
        SDL_GLContext _context = nullptr;

    };

} // DGL

#endif //DEMONENGINE_WINDOW_H
