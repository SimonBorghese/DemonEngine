//
// Created by simon on 6/1/22.
//

#include "DG_Entity.h"

namespace DemonGame {

    void DG_Entity::createEntityFromMesh(const char *meshFile,
                              glm::vec3 pos,
                              glm::vec3 rotation,
                              glm::vec3 scale){
        createTransform(pos, rotation, scale);
        //mainTransform = new DemonWorld::DW_Transform(pos, rotation, scale);
        mainMeshRenderer = new DemonRender::DR_MeshRenderer();
        mainMeshRenderer->setShader(meshShader);
        mainMeshRenderer->bindTransform(this);
        mainMeshRenderer->loadMeshFromFile(meshFile);

        renderManager->addMeshGroup(mainMeshRenderer);
    }

    void DG_Entity::destroyEntity(){
        mainMeshRenderer->destroyMeshes();
        //delete mainTransform;
        delete mainMeshRenderer;
    }
} // DemonGame