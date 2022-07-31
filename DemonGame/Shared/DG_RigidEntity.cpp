//
// Created by simon on 6/7/22.
//

#include "DG_RigidEntity.h"

namespace DemonGame {
    void DG_RigidEntity::createEntityFromMesh(const char *meshFile,
                                              glm::vec3 pos,
                                              glm::vec3 rotation,
                                              glm::vec3 scale) {
        mainTransform.createTransform(pos, rotation, glm::vec3(1.0f));
        //mainTransform = new DemonWorld::DW_Transform(pos, rotation, scale);
        mainMeshRenderer = new DemonRender::DR_MeshRenderer();
        mainMeshRenderer->setShader(meshShader);


        unsigned int outLen;
        DemonBase::b_Mesh **normalMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile(meshFile, &outLen, scale);

        mainMeshRenderer->loadExistingMeshes(normalMesh, outLen);

        renderManager->addMeshGroup(mainMeshRenderer);

        std::vector<DemonBase::b_RigidMesh*> meshes;
        for (unsigned int m = 0; m < outLen; m++) {
             meshes.push_back((DemonBase::b_RigidMesh*) new DemonPhysics::DP_RigidTriangleMesh(normalMesh[m]));
            physicsManager->cookMesh(meshes.at(meshes.size()-1));
        }

        rigidActor = new DemonPhysics::DP_RigidActor(meshes);

        DemonPhysics::DP_PhysicsMaterial matgood(1.0f, 1.0f, 0.0f);
        physicsManager->cookMaterial(&matgood);
        //physicsManager->cookActor(rigidActor, &matgood);
        rigidActor->createActor(physicsManager->getPhysics(), matgood.getMaterial());
        physicsManager->addActor(rigidActor);
        rigidActor->setTransform(mainTransform);
        mainMeshRenderer->bindTransform(&mainTransform);
    }

    void DG_RigidEntity::destroyEntity() {
        mainMaterial->destroyMaterial();
        rigidMesh->destroyMesh();
        physicsManager->removeActor(rigidActor);
        rigidActor->destroyActor();
        mainMeshRenderer->destroyMeshes();
        //delete mainTransform;
        delete mainMeshRenderer;
    }

    void DG_RigidEntity::update() {
        //rigidActor->setTransform(mainTransform);
        //printf("P: %f %f %f\n", rigidActor->getTransform()->getPosition().x, rigidActor->getTransform()->getPosition().y, rigidActor->getTransform()->getPosition().z);

        rigidActor->updateActor();
        mainTransform.setPosition(rigidActor->getTransform()->getPosition());
        mainTransform.setRotation(rigidActor->getTransform()->getRotation());

        //mainTransform = *rigidActor->getTransform();
        //mainTransform = *rigidActor->getTransform();

    }
} // DemonGame