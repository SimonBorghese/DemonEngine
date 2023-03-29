//
// Created by simon on 6/10/22.
//

#include "DG_PhysicsObject.h"

namespace DemonGame {
    void DG_PhysicsObject::createEntityFromMesh(const char *meshFile,
                                                glm::vec3 pos,
                                                glm::vec3 rotation,
                                                glm::vec3 scale) {
        DemonGame::DG_Entity::createEntityFromMesh(meshFile, pos, rotation, scale);
        // Create the mesh and actors
        // Create a vector of all the meshes in our list
        std::vector<DemonBase::b_RigidMesh*> meshes;
        for (unsigned int m = 0; m < _numMeshes; m++) {
            meshes.push_back((DemonBase::b_RigidMesh*) new DemonPhysics::DP_RigidConvexMesh(_loadedMesh[m]));
            physicsManager->cookMesh(meshes.at(meshes.size()-1));
        }
        rigidActor = new DemonPhysics::DP_RigidPhysicsActor(meshes);
        // Create the material for the actor
        if (!mainMaterial) {
            mainMaterial = new DemonPhysics::DP_PhysicsMaterial;
        }
        physicsManager->cookMaterial(mainMaterial);
        // Finally actually create the actor and add it to the scene
        rigidActor->createActor(physicsManager->getPhysics(), mainMaterial->getMaterial());
        physicsManager->addActor(rigidActor);
        // Bind our transform to the actor & mesh renderer
        rigidActor->setTransform(*mainTransform);
        // Set the user data
        rigidActor->setEmbedData(&clientInfo);

        //mainTransform.setPosition(rigidActor->getTransform()->getPosition());
        //mainTransform.setRotation(rigidActor->getTransform()->getRotation());
    }

    // Cannot apply scale here
    void DG_PhysicsObject::createEntityFromExistingMesh(DemonBase::b_Mesh **meshes, uint32_t numMesh,
                                      glm::vec3 pos,
                                      glm::vec3 rotation){
        DG_Entity::createEntityFromExistingMesh(meshes, numMesh, pos, rotation);
        // Create the mesh and actors
        // Create a vector of all the meshes in our list
        std::vector<DemonBase::b_RigidMesh*> rigidMeshes;
        for (unsigned int m = 0; m < _numMeshes; m++) {
            rigidMeshes.push_back((DemonBase::b_RigidMesh*) new DemonPhysics::DP_RigidConvexMesh(_loadedMesh[m]));
            physicsManager->cookMesh(rigidMeshes.at(rigidMeshes.size()-1));
        }
        rigidActor = new DemonPhysics::DP_RigidPhysicsActor(rigidMeshes);
        // Create the material for the actor
        if (!mainMaterial) {
            mainMaterial = new DemonPhysics::DP_PhysicsMaterial;
        }
        physicsManager->cookMaterial(mainMaterial);
        // Finally actually create the actor and add it to the scene
        rigidActor->createActor(physicsManager->getPhysics(), mainMaterial->getMaterial());
        physicsManager->addActor(rigidActor);
        // Bind our transform to the actor & mesh renderer
        rigidActor->setTransform(*mainTransform);
        // Set the user data
        rigidActor->setEmbedData(&clientInfo);
    }

    void DG_PhysicsObject::destroyEntity() {
        //physicsManager->removeActor(rigidActor);
        //if (rigidActor != NULL && rigidActor->getRealActor() != NULL) {
            physicsManager->getScene()->removeActor(*rigidActor->getRealActor());
        //}
        mainMaterial->destroyMaterial();
        //rigidMesh->destroyMesh();
        rigidActor->destroyActor();
        _primaryMesh->destroyMeshes();

        //delete mainTransform;
        //delete mainMeshRenderer;
        //delete rigidActor;
        *rigidActor = nullptr;
        //delete rigidMesh;
        //*rigidMesh = nullptr;
        delete _primaryMesh;
        delete mainMaterial;
    }

    void DG_PhysicsObject::update(DGL::Shader *overrideShader) {
        DG_Entity::update(overrideShader);
        if (_primaryMesh != nullptr && rigidActor->getTransform() != nullptr) {
            rigidActor->updateActor();
            mainTransform->setPosition(rigidActor->getTransform()->getPosition());
            mainTransform->setRotation(rigidActor->getTransform()->getRotation());
        }
    }
} // DemonGame