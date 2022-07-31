//
// Created by simon on 5/23/22.
//

#define DEFINEVARS

#include "DR_RenderManager.h"

namespace DemonRender {
    DR_RenderManager::DR_RenderManager() {}

    DR_RenderManager::~DR_RenderManager() {}

    void DR_RenderManager::createRenderer(const char *title, uint32_t width, uint32_t height) {
        _drwindow = new DR_Window();
        _dropengl = new DR_OpenGL();
        _drwindow->createWindow(title, width, height);
        _dropengl->initOpenGL(width, height);

    }

    void DR_RenderManager::destroyRenderer() {
        _drwindow->destroyWindow();
        delete _drwindow;
        delete _dropengl;
    }

    void DR_RenderManager::newFrame() {
        _dropengl->clear();
    }

    void DR_RenderManager::render() {
        for (unsigned int m = 0; m < _drMeshList.size(); m++) {
            if (_drMeshList.at(m)->active) {
                _drMeshList.at(m)->bindShader();
                if (hasCamera) {
                    _drMainCamera->setShader(_drMeshList.at(m)->getShader());
                    _drMainCamera->setMatrix();
                }
                _drMeshList.at(m)->renderMeshes();
            }
            else{
                delete _drMeshList.at(m);
                _drMeshList.erase(_drMeshList.begin() + m);
            }
        }
        _drwindow->flip();
    }
} // DemonRender