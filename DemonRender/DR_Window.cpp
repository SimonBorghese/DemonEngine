//
// Created by simon on 5/23/22.
//

#include "DR_Window.h"

namespace DemonRender {
    DR_Window::DR_Window() {}

    void DR_Window::createWindow(const char *name, uint32_t width, uint32_t height) {
        _window = SDL_CreateWindow(name,
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   width, height,
                                   SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        _context = SDL_GL_CreateContext(_window);
        SDL_GL_MakeCurrent(_window, _context);

        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        //SDL_ShowCursor(SDL_FALSE);
        //SDL_SetWindowGrab(_window, SDL_TRUE);
        //SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    void DR_Window::destroyWindow() {
        SDL_GL_DeleteContext(_context);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    // Lua function defs
    void DR_Window::moveWindow(int newX, int newY) {
        SDL_SetWindowPosition(_window, newX, newY);
    }

    void DR_Window::flip() {
        SDL_GL_SwapWindow(_window);
        SDL_UpdateWindowSurface(_window);
    }
} // DemonRender