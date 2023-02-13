//
// Created by simon on 9/23/22.
//

#include "DG_Trigger.h"

namespace DemonGame {
    void DG_Trigger::createEntityFromMesh(const char *meshFile,
                                              glm::vec3 pos,
                                              glm::vec3 rotation,
                                              glm::vec3 scale) {
        mainTransform = new DemonWorld::DW_Transform();
        enableRender = 0;

        //mainMeshRenderer = new DemonRender::DR_MeshRenderer();
        //mainMeshRenderer->setShader(meshShader);


        unsigned int outLen;
        DemonBase::b_Mesh **normalMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile(meshFile, &outLen, scale);

        mainTransform->createTransform(pos, rotation, glm::vec3(1.0f));

        //mainMeshRenderer->loadExistingMeshes(normalMesh, outLen);

        //renderManager->addMeshGroup(mainMeshRenderer);

        std::vector<DemonBase::b_RigidMesh*> meshes;
        for (unsigned int m = 0; m < outLen; m++) {
            meshes.push_back((DemonBase::b_RigidMesh*) new DemonPhysics::DP_RigidConvexMesh(normalMesh[m]));
            physicsManager->cookMesh(meshes.at(meshes.size()-1));
        }

        rigidActor = new DemonPhysics::DP_RigidTriggerActor(meshes);

        DemonPhysics::DP_PhysicsMaterial matgood(1.0f, 1.0f, 0.0f);
        physicsManager->cookMaterial(&matgood);

        rigidActor->createActor(physicsManager->getPhysics(), matgood.getMaterial());
        physicsManager->addActor(rigidActor);
        rigidActor->setTransform(*mainTransform);
        //mainMeshRenderer->bindTransform(&mainTransform);

        rigidActor->setEmbedData(&generalStruct);
    }

    void DG_Trigger::destroyEntity() {
        physicsManager->removeActor(rigidActor);
        rigidActor->destroyActor();
        //delete mainTransform;
    }

    void DG_Trigger::update() {

        rigidActor->updateActor();
        mainTransform->setPosition(rigidActor->getTransform()->getPosition());
        mainTransform->setRotation(rigidActor->getTransform()->getRotation());


    }
} // DemonGame