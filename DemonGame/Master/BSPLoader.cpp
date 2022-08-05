//
// Created by simon on 7/11/22.
//
#include "BSPLoader.h"

namespace DemonEngine {
    DemonGame::DG_BSPMap* BSPLoader::loadBSP(const char *map){
        auto bspMap = _targetEngine->createStaticWorld();
        auto bspFile = DemonIO::DI_BSPLoader::createBSP(map);
        bspMap->createEntityFromCBSP(bspFile, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f/4.0f));

        for (int e = 0; e < bspFile->nEntityCount; e++){
            BSP_EntityCreateInfo targetEntityCreationInfo;
            // Assign name
            strcpy(&targetEntityCreationInfo.name[0], bspFile->mEntity[e].classname);
            // Get position
            targetEntityCreationInfo.pos = DemonIO::DI_BSPLoader::getPosFromEntity(&bspFile->mEntity[e]);
            if (bspEntityCallback != nullptr){
                bspEntityCallback(targetEntityCreationInfo);
            }
        }
        return bspMap;
    }
} // BSP