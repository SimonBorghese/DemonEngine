//
// Created by simon on 6/10/22.
//

#include "DG_PhysicsObject.h"

namespace DemonGame {
    void DG_PhysicsObject::createEntityFromMesh(const char *meshFile,
                                                glm::vec3 pos,
                                                glm::vec3 rotation,
                                                glm::vec3 scale) {
        mainTransform.createTransform(pos, rotation, scale);
        //mainTransform = new DemonWorld::DW_Transform(pos, rotation, scale);
        mainMeshRenderer = new DemonRender::DR_MeshRenderer();
        mainMeshRenderer->setShader(meshShader);


        unsigned int outLen;
        DemonBase::b_Mesh **normalMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile(meshFile, &outLen);

        mainMeshRenderer->loadExistingMeshes(normalMesh, outLen);

        renderManager->addMeshGroup(mainMeshRenderer);

        rigidMesh = new DemonPhysics::DP_RigidMesh(normalMesh[0]);
        rigidActor = new DemonPhysics::DP_RigidPhysicsActor(rigidMesh);
        DemonPhysics::DP_PhysicsMaterial matgood;
        physicsManager->cookMesh(rigidMesh);
        physicsManager->cookMaterial(&matgood);
        //physicsManager->cookActor(rigidActor, &matgood);
        rigidActor->createActor(physicsManager->getPhysics(), matgood.getMaterial());
        physicsManager->addActor(rigidActor);
        rigidActor->setTransform(mainTransform);
        mainMeshRenderer->bindTransform(&mainTransform);


        rigidActor->setEmbedData(&objDesc);
    }

    void DG_PhysicsObject::destroyEntity() {
        mainMaterial->destroyMaterial();
        rigidMesh->destroyMesh();
        physicsManager->removeActor(rigidActor);
        rigidActor->destroyActor();
        mainMeshRenderer->destroyMeshes();
        //delete mainTransform;
        delete mainMeshRenderer;
    }

    void DG_PhysicsObject::update() {
        //rigidActor->setTransform(mainTransform);
        //printf("P: %f %f %f\n", rigidActor->getTransform()->getPosition().x, rigidActor->getTransform()->getPosition().y, rigidActor->getTransform()->getPosition().z);

        //rigidActor->setTransform(*this);
        rigidActor->updateActor();
        mainTransform.setPosition(rigidActor->getTransform()->getPosition());
        mainTransform.setRotation(rigidActor->getTransform()->getRotation());
        //mainTransform = *rigidActor->getTransform();
        //mainTransform = *rigidActor->getTransform();

    }
} // DemonGame