//
// Created by simon on 6/7/22.
//

#include "DG_RigidEntity.h"

namespace DemonGame {
    void DG_RigidEntity::createEntityFromMesh(const char *meshFile,
                                              glm::vec3 pos,
                                              glm::vec3 rotation,
                                              glm::vec3 scale) {
        DemonGame::DG_Entity::createEntityFromMesh(meshFile, pos, rotation, scale);


        std::vector<DemonBase::b_RigidMesh*> meshes;
        for (unsigned int m = 0; m < _numMeshes; m++) {
             meshes.push_back((DemonBase::b_RigidMesh*) new DemonPhysics::DP_RigidTriangleMesh(_loadedMesh[m]));
            physicsManager->cookMesh(meshes.at(meshes.size()-1));
        }

        rigidActor = new DemonPhysics::DP_RigidActor(meshes);

        if (!mainMaterial) {
            mainMaterial = new DemonPhysics::DP_PhysicsMaterial(0.0f, 0.0f, 0.0f);
        }
        physicsManager->cookMaterial(mainMaterial);
        //physicsManager->cookActor(rigidActor, &matgood);
        rigidActor->createActor(physicsManager->getPhysics(), mainMaterial->getMaterial());
        physicsManager->addActor(rigidActor);
        rigidActor->setTransform(*mainTransform);
        rigidActor->setEmbedData(&clientInfo);
    }

    void DG_RigidEntity::createEntityFromExistingMesh(DemonBase::b_Mesh **meshes, uint32_t numMesh,
                                      glm::vec3 pos,
                                      glm::vec3 rotation){
        DG_Entity::createEntityFromExistingMesh(meshes, numMesh, pos, rotation);

        std::vector<DemonBase::b_RigidMesh*> rigidMeshes;
        for (unsigned int m = 0; m < _numMeshes; m++) {
            rigidMeshes.push_back((DemonBase::b_RigidMesh*) new DemonPhysics::DP_RigidTriangleMesh(_loadedMesh[m]));
            physicsManager->cookMesh(rigidMeshes.at(rigidMeshes.size()-1));
        }

        rigidActor = new DemonPhysics::DP_RigidActor(rigidMeshes);

        if (!mainMaterial) {
            mainMaterial = new DemonPhysics::DP_PhysicsMaterial(0.0f, 0.0f, 0.0f);
        }
        physicsManager->cookMaterial(mainMaterial);
        //physicsManager->cookActor(rigidActor, &matgood);
        rigidActor->createActor(physicsManager->getPhysics(), mainMaterial->getMaterial());
        physicsManager->addActor(rigidActor);
        rigidActor->setTransform(*mainTransform);
        rigidActor->setEmbedData(&clientInfo);

    }

    void DG_RigidEntity::destroyEntity() {
        mainMaterial->destroyMaterial();
        //rigidMesh->destroyMesh();
        physicsManager->removeActor(rigidActor);
        rigidActor->destroyActor();
        _primaryMesh->destroyMeshes();
        //delete mainTransform;
        delete _primaryMesh;
    }

    void DG_RigidEntity::update(DGL::Shader *overrideShader) {
        DG_Entity::update(overrideShader);
        rigidActor->updateActor();
        mainTransform->setPosition(rigidActor->getTransform()->getPosition());
        mainTransform->setRotation(rigidActor->getTransform()->getRotation());

    }

    PxSweepBuffer DG_RigidEntity::sweepCast(glm::vec3 origin, glm::vec3 direction, float distance, DemonPhysics::DP_PhysicsManager *manager){
        physx::PxSweepBuffer hit;
        //PxQueryFilterData filterData(PxQueryFlag::eSTATIC);

        bool success = manager->getScene()->sweep(*rigidMesh->getGeometry(), physx::PxTransform(physx::PxVec3(origin.x, origin.y, origin.z)),
                                     physx::PxVec3(direction.x, direction.y, direction.z),
                                     distance, hit);
        if (!success){
            //printf("[Physics] SWEEP CAST FAILED!\n");
            hit.nbTouches = 0;
            hit.hasBlock = false;
        }
        return hit;

    }
} // DemonGame