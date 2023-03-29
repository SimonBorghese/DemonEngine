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
        DemonBase::b_Mesh **normalMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile(
                DFS::FileSystem::getFS()->getModelPath(meshFile).c_str(), &outLen, scale);

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

        rigidActor->setEmbedData(&clientInfo);
    }

    void DG_Trigger::createEntityFromExistingMesh(DemonBase::b_Mesh **meshes, uint32_t numMesh,
                                      glm::vec3 pos,
                                      glm::vec3 rotation){
        mainTransform = new DemonWorld::DW_Transform();
        enableRender = 0;

        //mainMeshRenderer = new DemonRender::DR_MeshRenderer();
        //mainMeshRenderer->setShader(meshShader);

        mainTransform->createTransform(pos, rotation, glm::vec3(1.0f));

        //mainMeshRenderer->loadExistingMeshes(normalMesh, outLen);

        //renderManager->addMeshGroup(mainMeshRenderer);

        std::vector<DemonBase::b_RigidMesh*> arr_meshes;
        for (unsigned int m = 0; m < numMesh; m++) {
            arr_meshes.push_back((DemonBase::b_RigidMesh*) new DemonPhysics::DP_RigidConvexMesh(meshes[m]));
            physicsManager->cookMesh(arr_meshes.at(arr_meshes.size()-1));
        }

        rigidActor = new DemonPhysics::DP_RigidTriggerActor(arr_meshes);

        DemonPhysics::DP_PhysicsMaterial matgood(1.0f, 1.0f, 0.0f);
        physicsManager->cookMaterial(&matgood);

        rigidActor->createActor(physicsManager->getPhysics(), matgood.getMaterial());
        physicsManager->addActor(rigidActor);
        rigidActor->setTransform(*mainTransform);
        //mainMeshRenderer->bindTransform(&mainTransform);

        rigidActor->setEmbedData(&clientInfo);
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