//
// Created by simon on 6/10/22.
//

#include "DG_PhysicsObject.h"

namespace DemonGame {
    void DG_PhysicsObject::createEntityFromMesh(const char *meshFile,
                                                glm::vec3 pos,
                                                glm::vec3 rotation,
                                                glm::vec3 scale) {
        // Create our base transform
        mainTransform.createTransform(pos, rotation, glm::vec3(1.0f));
        // Create the mesh renderer and set it to our shader
        mainMeshRenderer = new DemonRender::DR_MeshRenderer();
        mainMeshRenderer->setShader(meshShader);
        // Load the meshes then add them to the render manager
        unsigned int outLen;
        DemonBase::b_Mesh **normalMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile(meshFile,
                                                                                     &outLen,
                                                                                     scale);
        mainMeshRenderer->loadExistingMeshes(normalMesh, outLen);
        renderManager->addMeshGroup(mainMeshRenderer);
        // Create the mesh and actors
        // Create a vector of all the meshes in our list
        std::vector<DemonBase::b_RigidMesh*> meshes;
        for (unsigned int m = 0; m < outLen; m++) {
            meshes.push_back((DemonBase::b_RigidMesh*) new DemonPhysics::DP_RigidConvexMesh(normalMesh[m]));
            physicsManager->cookMesh(meshes.at(meshes.size()-1));
        }
        rigidActor = new DemonPhysics::DP_RigidPhysicsActor(meshes);
        // Create the material for the actor
        mainMaterial = new DemonPhysics::DP_PhysicsMaterial;
        physicsManager->cookMaterial(mainMaterial);
        // Finally actually create the actor and add it to the scene
        rigidActor->createActor(physicsManager->getPhysics(), mainMaterial->getMaterial());
        physicsManager->addActor(rigidActor);
        // Bind our transform to the actor & mesh renderer
        rigidActor->setTransform(mainTransform);
        mainMeshRenderer->bindTransform(&mainTransform);
        // Set the user data
        rigidActor->setEmbedData(&generalStruct);

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