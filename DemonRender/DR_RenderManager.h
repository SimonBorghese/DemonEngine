//
// Created by simon on 5/23/22.
//

#ifndef DEMONENGINE_DR_RENDERMANAGER_H
#define DEMONENGINE_DR_RENDERMANAGER_H
#include "DR_Window.h"
#include "DR_OpenGL.h"
#include "DR_Shader.h"
// Purpose:
// Render Manager should oversee & wrap most elements of the rendering process to the programmer
// - Manage current rendering meshes?
// -

namespace DemonRender {

    class DR_RenderManager {
    public:
        DR_RenderManager();
        virtual ~DR_RenderManager();

        void createRenderer(const char * title, uint32_t width, uint32_t height);
        void setDefaultShader(DR_Shader *targetShader);

        void destroyRenderer();

        //  Lua Functions
        void newFrame();
        void render();
    private:
        DR_Window *_drwindow = nullptr;
        DR_OpenGL *_dropengl = nullptr;
        DR_Shader *_drDefaultShader = nullptr;
    };

} // DemonRender

#endif //DEMONENGINE_DR_RENDERMANAGER_H
