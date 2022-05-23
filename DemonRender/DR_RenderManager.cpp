//
// Created by simon on 5/23/22.
//

#define DEFINEVARS
#include "DR_RenderManager.h"

namespace DemonRender {
    DR_RenderManager::DR_RenderManager() {}
    DR_RenderManager::~DR_RenderManager(){
        delete _drwindow;
    }

    void DR_RenderManager::createRenderer(const char * title, uint32_t width, uint32_t height){
        _drwindow = new DR_Window();
        _drwindow->createWindow(title, width, height);

        DR_registerLuaFuncs(_drwindow);
    }

    void DR_RenderManager::destroyRenderer(){
        _drwindow->destroyWindow();
    }
} // DemonRender