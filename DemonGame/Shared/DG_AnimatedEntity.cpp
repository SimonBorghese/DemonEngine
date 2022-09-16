//
// Created by simon on 9/15/22.
//

#include "DG_AnimatedEntity.h"

namespace DemonGame {
    void DG_AnimatedEntity::createEntityFromMesh(const char *meshFile, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
    {
        mainTransform.createTransform(pos, rotation, glm::vec3(1.0f));
        //mainTransform = new DemonWorld::DW_Transform(pos, rotation, scale);
        mainMeshRenderer = new DemonRender::DR_MeshRenderer();
        mainMeshRenderer->setShader(meshShader);
        mainMeshRenderer->bindTransform(&mainTransform);

        animMeshes = DemonIO::DI_AnimationSceneLoader::loadMeshesFromFile(meshFile,
                                                                                     &numMeshes,
                                                                                     scale);

        mainMeshRenderer->loadExistingMeshes((DemonBase::b_Mesh**) animMeshes, numMeshes);

        renderManager->addMeshGroup(mainMeshRenderer);
    }

    void DG_AnimatedEntity::playAnimation(float currentTime){
        for (unsigned int m = 0; m < numMeshes; m++){
            animMeshes[m]->setShader(meshShader);
            animMeshes[m]->doAnimation(currentTime);
        }
    }
    void DG_AnimatedEntity::setAnimation(int index){
        for (unsigned int m = 0; m < numMeshes; m++){
            animMeshes[m]->setAnimation(index);
        }
    }

    void DG_AnimatedEntity::playOnce(float currentTime){
        for (unsigned int m = 0; m < numMeshes; m++){
            animMeshes[m]->playOnce(currentTime);
        }
    }

    int DG_AnimatedEntity::isAnimationFinished(float currentTime){
        for (unsigned int m = 0; m < numMeshes; m++){
            if (!animMeshes[m]->animationFinished(currentTime)){
                return 0;
            }
        }
        return 1;
    }
} // DemonGame