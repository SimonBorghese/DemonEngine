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
#include <DemonIO/DI_BSPLoader.h>
#include <DemonIO/DI_SceneLoader.h>
#include <DemonBase/b_Mesh.h>
#include <fmt/format.h>
#include <DFileSystem/FileSystem.h>

#define BSP_SCALE glm::vec3(1.0f/8.0f)

namespace DemonEngine {
    typedef struct {
        char name[64];
        glm::vec3 pos;

        std::pair<const char *, const char *> *attributes;
        unsigned int numAttributes;

        CBSP_Entity *currentEntity;
        uint32_t entityNumber;

        DemonGame::DG_RigidEntity *worldBrush; // ONLY FOR WORLD SPAWM!!!!

        DemonBase::b_Mesh **brushMeshes;
        uint32_t numBrushMesh;
        glm::vec3 origin;
        float angle;
    } BSP_EntityCreateInfo;

    class BSPLoader {
    public:
        BSPLoader(DemonEngine::Engine *engine) : _targetEngine(engine) {}

        DemonGame::DG_RigidEntity* loadBSP(const char *map);

        void setBSPCreationCallback(std::function<void(BSP_EntityCreateInfo)> callback) { bspEntityCallback = callback;}

    private:
        DemonEngine::Engine *_targetEngine;
        std::function<void(BSP_EntityCreateInfo)> bspEntityCallback;

    };

} // BSP

#endif //DEMONENGINE_BSPLOADER_H
