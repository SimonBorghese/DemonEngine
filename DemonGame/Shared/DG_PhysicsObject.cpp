//
// Created by simon on 6/10/22.
//

#include "DG_PhysicsObject.h"

namespace DemonGame {
    void DG_PhysicsObject::createEntityFromMesh(const char *meshFile,
                                                glm::vec3 pos,
                                                glm::vec3 rotation,
                                                glm::vec3 scale) {
        mainTransform.createTransform(pos, rotation, glm::vec3(1.0f));
        //mainTransform = new DemonWorld::DW_Transform(pos, rotation, scale);
        mainMeshRenderer = new DemonRender::DR_MeshRenderer();
        mainMeshRenderer->setShader(meshShader);


        unsigned int outLen;
        DemonBase::b_Mesh **normalMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile(meshFile,
                                                                                     &outLen,
                                                                                     scale);

        mainMeshRenderer->loadExistingMeshes(normalMesh, outLen);

        renderManager->addMeshGroup(mainMeshRenderer);

        rigidMesh = new DemonPhysics::DP_RigidConvexMesh(normalMesh[0]);
        rigidActor = new DemonPhysics::DP_RigidPhysicsActor((DemonBase::b_RigidMesh*) rigidMesh);

        mainMaterial = new DemonPhysics::DP_PhysicsMaterial;
        physicsManager->cookMesh(rigidMesh);
        physicsManager->cookMaterial(mainMaterial);
        //physicsManager->cookActor(rigidActor, &matgood);
        rigidActor->createActor(physicsManager->getPhysics(), mainMaterial->getMaterial());
        physicsManager->addActor(rigidActor);

        rigidActor->setTransform(mainTransform);
        mainMeshRenderer->bindTransform(&mainTransform);
        rigidActor->setEmbedData(&objDesc);

        //mainTransform.setPosition(rigidActor->getTransform()->getPosition());
        //mainTransform.setRotation(rigidActor->getTransform()->getRotation());
    }

    void DG_PhysicsObject::destroyEntity() {
        //physicsManager->removeActor(rigidActor);
        //if (rigidActor != NULL && rigidActor->getRealActor() != NULL) {
            physicsManager->getScene()->removeActor(*rigidActor->getRealActor());
        //}
        mainMaterial->destroyMaterial();
        rigidMesh->destroyMesh();
        rigidActor->destroyActor();
        mainMeshRenderer->destroyMeshes();
        //delete mainTransform;
        //delete mainMeshRenderer;
        mainMeshRenderer->active = 0;
        //delete rigidActor;
        *rigidActor = nullptr;
        delete rigidMesh;
        //*rigidMesh = nullptr;
        //delete mainMeshRenderer;
        //delete mainMaterial;
    }

    void DG_PhysicsObject::update() {
        if (mainMeshRenderer != nullptr && rigidActor->getTransform() != nullptr) {
            rigidActor->updateActor();
            mainTransform.setPosition(rigidActor->getTransform()->getPosition());
            mainTransform.setRotation(rigidActor->getTransform()->getRotation());
        }
    }
} // DemonGame