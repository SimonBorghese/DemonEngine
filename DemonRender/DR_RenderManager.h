//
// Created by simon on 5/23/22.
//

#ifndef DEMONENGINE_DR_RENDERMANAGER_H
#define DEMONENGINE_DR_RENDERMANAGER_H

#include "DR_Window.h"
#include "DR_OpenGL.h"
#include "DR_Shader.h"
#include "DR_Camera.h"
#include "DR_MeshRenderer.h"
#include <vector>
// Purpose:
// Render Manager should oversee & wrap most elements of the rendering process to the programmer
// - Manage current rendering meshes?
// -

namespace DemonRender {

    class DR_RenderManager {
    public:
        DR_RenderManager();

        virtual ~DR_RenderManager();

        void createRenderer(const char *title, uint32_t width, uint32_t height);

        void setDefaultShader(DR_Shader *targetShader);

        void addMeshGroup(DR_MeshRenderer *targetMesh) { _drMeshList.push_back(targetMesh); }

        void setCamera(DR_Camera *targetCam) {
            _drMainCamera = targetCam;
            hasCamera = true;
        }

        void destroyRenderer();

        //  Lua Functions
        void newFrame();

        void render();

    private:
        std::vector<DR_MeshRenderer *> _drMeshList;
        DR_Window *_drwindow = nullptr;
        DR_OpenGL *_dropengl = nullptr;
        DR_Shader *_drDefaultShader = nullptr;
        DR_Camera *_drMainCamera = nullptr;
        bool hasCamera = false;
    };

} // DemonRender

#endif //DEMONENGINE_DR_RENDERMANAGER_H
