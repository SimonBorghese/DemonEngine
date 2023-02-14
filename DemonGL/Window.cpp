#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
//
// Created by simon on 1/9/23.
//

#include "Window.h"

#define VERBOSE 0

namespace DGL {
    // Shove this in he
    void APIENTRY glDebugOutput(GLenum source,
                                GLenum type,
                                unsigned int id,
                                GLenum severity,
                                GLsizei length,
                                const char *message,
                                const void *userParam) {
        if (VERBOSE) {
            // ignore non-significant error/warning codes
            if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
            std::cout << "[OPENGL] Length: " << length << " User Param: " << userParam << std::endl;

            std::cout << "[OPENGL] ---------------" << std::endl;
            std::cout << "[OPENGL] Debug message (" << id << "): " << message << std::endl;


            std::cout << "[OPENGL] ";
            switch (source) {
                case GL_DEBUG_SOURCE_API:
                    std::cout << "Source: API";
                    break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                    std::cout << "Source: Window System";
                    break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER:
                    std::cout << "Source: Shader Compiler";
                    break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:
                    std::cout << "Source: Third Party";
                    break;
                case GL_DEBUG_SOURCE_APPLICATION:
                    std::cout << "Source: Application";
                    break;
                case GL_DEBUG_SOURCE_OTHER:
                    std::cout << "Source: Other";
                    break;
                default:
                    std::cout << "Source: Unknown";
                    break;
            }
            std::cout << std::endl;

            std::cout << "[OPENGL] ";

            switch (type) {
                case GL_DEBUG_TYPE_ERROR:
                    std::cout << "Type: Error";
                    break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    std::cout << "Type: Deprecated Behaviour";
                    break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    std::cout << "Type: Undefined Behaviour";
                    break;
                case GL_DEBUG_TYPE_PORTABILITY:
                    std::cout << "Type: Portability";
                    break;
                case GL_DEBUG_TYPE_PERFORMANCE:
                    std::cout << "Type: Performance";
                    break;
                case GL_DEBUG_TYPE_MARKER:
                    std::cout << "Type: Marker";
                    break;
                case GL_DEBUG_TYPE_PUSH_GROUP:
                    std::cout << "Type: Push Group";
                    break;
                case GL_DEBUG_TYPE_POP_GROUP:
                    std::cout << "Type: Pop Group";
                    break;
                case GL_DEBUG_TYPE_OTHER:
                    std::cout << "Type: Other";
                    break;
                default:
                    std::cout << "Type: Unknown";
                    break;
            }
            std::cout << std::endl;

            std::cout << "[OPENGL] ";

            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:
                    std::cout << "Severity: high";
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    std::cout << "Severity: medium";
                    break;
                case GL_DEBUG_SEVERITY_LOW:
                    std::cout << "Severity: low";
                    break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    std::cout << "Severity: notification";
                    break;
                default:
                    std::cout << "Severity: Unknown";
                    break;
            }
            std::cout << std::endl;
            std::cout << std::endl;
        } else{
            //if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
            std::cout << "[OPENGL] Debug message (" << id << "): " << message << std::endl;

        }
    }

    Window::Window(const char *windowTitle, uint32_t width, uint32_t height, int vsync) : _windowTitle(windowTitle),
    _width(width), _height(height){
        // Init SDL with *all* of the works
        LOG("SDL_INIT FAILED", !SDL_Init(SDL_INIT_EVERYTHING));

        // Create the window
        _window = SDL_CreateWindow(_windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                   (int) _width,(int) _height,
                                   SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        LOG("SDL WINDOW CREATION FAILED!", _window);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

        _context = SDL_GL_CreateContext(_window);
        LOG("SDL CONTEXT CREATION FAILED", _context);

        SDL_GL_MakeCurrent(_window, _context);
        SDL_GL_SetSwapInterval(vsync);

        gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
        glViewport(0,0, (int) _width, (int) _height);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glFrontFace(GL_CW);

#ifndef __WIN32__
//For some reason, this doesn't work in my Windows VM, so keeping it disabled for now
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            printf("Successfully enabled debug output\n");
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);

#ifndef NDEBUG
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#else
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif // NDEBUG
        }
#endif // __WIN32__



    }
    Window::~Window(){
        SDL_GL_DeleteContext(_context);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    void Window::flipWindow(){
        SDL_GL_SwapWindow(_window);
        SDL_UpdateWindowSurface(_window);
    }
    void Window::clearWindow(float clearR, float clearG, float clearB){
        glClearColor(clearR, clearG, clearB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    void Window::setVsync(int vsync){
        SDL_GL_SetSwapInterval(vsync);
    }
    SDL_Window *Window::getWindow(){
        return _window;
    }
    SDL_GLContext Window::getContext(){
        return _context;
    }

    void Window::setMouseGrab(int enable){
        if (enable >= 0) {
            SDL_SetWindowGrab(_window, (SDL_bool)(SDL_FALSE + enable));
            SDL_SetRelativeMouseMode((SDL_bool) (SDL_FALSE + enable));
        }
        else{
            SDL_SetWindowGrab(_window, (SDL_bool) !SDL_GetWindowGrab(_window));
            SDL_SetRelativeMouseMode((SDL_bool) !SDL_GetRelativeMouseMode());
        }
    }
} // DGL
#pragma clang diagnostic pop