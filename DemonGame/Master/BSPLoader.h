//
// Created by simon on 7/11/22.
//

#ifndef DEMONENGINE_BSPLOADER_H
#define DEMONENGINE_BSPLOADER_H
#include "Engine.h"
#include <glm/glm.hpp>
#include <string>
#include <functional>
#include <utility>
#include <DemonGame/Shared/DG_BSPMap.h>
#include <DemonIO/DI_BSPLoader.h>

namespace DemonEngine {
    typedef struct{
        char name[64];
        glm::vec3 pos;

        std::pair<const char *, const char *> *attributes;
        unsigned int numAttributes;
    } BSP_EntityCreateInfo;

    class BSPLoader {
    public:
        BSPLoader(DemonEngine::Engine *engine) : _targetEngine(engine) {}

        DemonGame::DG_BSPMap* loadBSP(const char *map);

        void setBSPCreationCallback(std::function<void(BSP_EntityCreateInfo)> callback) { bspEntityCallback = callback;}

    private:
        DemonEngine::Engine *_targetEngine;
        std::function<void(BSP_EntityCreateInfo)> bspEntityCallback;

    };

} // BSP

#endif //DEMONENGINE_BSPLOADER_H
