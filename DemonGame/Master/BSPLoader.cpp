//
// Created by simon on 7/11/22.
//
#include "BSPLoader.h"

namespace DemonEngine {
    DemonGame::DG_RigidEntity* BSPLoader::loadBSP(const char *map){
        //auto bspMap = _targetEngine->createWorldObject();
        //std::string bsp = fmt::format("{}.bsp", map);
        //std::string obj = fmt::format("{}.obj", map);
        auto bspFile = DemonIO::DI_BSPLoader::createBSP(DFS::FileSystem::getFS()->getMapBSPPath(map).c_str());

        unsigned int numMesh;
        DemonBase::b_Mesh **objMeshes = DemonIO::DI_SceneLoader::loadMeshesFromFile(
                DFS::FileSystem::getFS()->getMapGeometryPath(map).c_str(), &numMesh, BSP_SCALE);

        std::vector<std::vector<DemonBase::b_Mesh *>> entityBrushes(bspFile->nEntityCount);
        //std::fill(entityBrushes.begin(), entityBrushes.end() + bspFile->nEntityCount, 0);
        for (uint m = 0; m < numMesh; m++) {
            DemonBase::b_Mesh *mesh = objMeshes[m];
            int entityNum = -1;
            sscanf(mesh->getModelName(), "entity%d", &entityNum);
            assert(entityNum >= 0);

            entityBrushes.at(entityNum).push_back(mesh);
            //printf("Pushing back mesh: %s at point: %d\n", mesh->getModelName(), entityNum);
        }

        //bspMap->createEntityFromMesh(obj.c_str(), glm::vec3(0.0f), glm::vec3(0.0f), BSP_SCALE);
        //printf("Got: %d models!\n", bspFile->nModels);

        for (int e = 0; e < bspFile->nEntityCount; e++){
            BSP_EntityCreateInfo targetEntityCreationInfo;
            targetEntityCreationInfo.entityNumber = e;
            targetEntityCreationInfo.brushMeshes = nullptr;
            targetEntityCreationInfo.numBrushMesh = 0;
            targetEntityCreationInfo.origin = glm::vec3(0.0f);
            
            if (entityBrushes.at(e).size() > 0) {
                float degreeAngle = 0.0f;
                const char *angle = CBSP_getKeyFromEntity(&bspFile->mEntity[e], "angle");
                if (strcmp(angle, CBSP_getKeyFromEntity_FAILURE)){
                    degreeAngle = atof(angle);
                }
                targetEntityCreationInfo.angle = degreeAngle;



                if (e) {
                    glm::vec3 origin;
                    auto normalizedMesh = DemonIO::DI_SceneLoader::normalizeAroundOrigin(entityBrushes.at(e).data(),
                                                                                         entityBrushes.at(e).size(), &origin);

                    targetEntityCreationInfo.brushMeshes = normalizedMesh;
                    targetEntityCreationInfo.numBrushMesh = entityBrushes.at(e).size();
                    targetEntityCreationInfo.origin = origin;
                } else{
                    targetEntityCreationInfo.worldBrush = _targetEngine->createWorldObject();
                    targetEntityCreationInfo.worldBrush->createEntityFromExistingMesh(
                            entityBrushes.at(e).data(),
                            entityBrushes.at(e).size(),
                            glm::vec3(0.0f),
                            glm::vec3(0.0f, glm::radians(degreeAngle), 0.0f));
                    targetEntityCreationInfo.worldBrush->setName("WORLD BRUSH");
                }
            }

            // Assign name
            strcpy(&targetEntityCreationInfo.name[0], bspFile->mEntity[e].classname);
            // Get position
            targetEntityCreationInfo.pos = DemonIO::DI_BSPLoader::getPosFromEntity(&bspFile->mEntity[e]);

            targetEntityCreationInfo.currentEntity = &bspFile->mEntity[e];
            if (bspEntityCallback != nullptr){
                bspEntityCallback(targetEntityCreationInfo);
            }
        }
        return nullptr;
    }
} // BSP