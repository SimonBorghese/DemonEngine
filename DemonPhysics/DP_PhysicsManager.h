//
// Created by simon on 6/2/22.
//
#ifndef DEMONENGINE_DP_PHYSICSMANAGER_H
#define DEMONENGINE_DP_PHYSICSMANAGER_H
#include <glm/glm.hpp>
#include "DP_RigidMesh.h"
#include "DP_RigidActor.h"
#include <DemonBase/b_RigidActor.h>
#include "DP_RigidMesh.h"
#include "DP_PhysicsMaterial.h"
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/extensions/PxTriangleMeshExt.h>
#include <PhysX/extensions/PxDefaultErrorCallback.h>
#include <PhysX/extensions/PxDefaultSimulationFilterShader.h>
#include <PhysX/extensions/PxDefaultAllocator.h>
#include <PhysX/extensions/PxDefaultCpuDispatcher.h>
#include <PhysX/characterkinematic/PxController.h>

namespace DemonPhysics {

    class DP_PhysicsManager {
    public:
        DP_PhysicsManager() {}

        void createPhysics(glm::vec3 gravity = glm::vec3(0.0f));

        //void beginSimulationTiming();
        void simulate(float simulationTime){
            pScene->simulate(simulationTime);
            pScene->fetchResults(true);
        }

        void closePhysics();

        void addActor(DemonBase::b_RigidActor *actor) { pScene->addActor(*actor->getActor()); }
        void removeActor(DemonBase::b_RigidActor *actor) { pScene->removeActor(*actor->getActor()); }

        void cookMesh(DP_RigidMesh *targetMesh) { targetMesh->createMesh(pPhysics, pCooking); }
        void cookActor(DP_RigidActor *targetActor, DP_PhysicsMaterial *mat) { targetActor->createActor(pPhysics, mat->getMaterial()); }
        void cookMaterial(DP_PhysicsMaterial *targetMat) { targetMat->createMaterial(pPhysics); }


        physx::PxPhysics* getPhysics() { return pPhysics; }
        physx::PxCooking* getCooking() { return pCooking; }
        physx::PxScene* getScene() { return pScene; }
    private:
        physx::PxDefaultErrorCallback pError;
        physx::PxDefaultAllocator pAllocate;

        physx::PxPhysics *pPhysics;
        physx::PxFoundation *pFoundation;

        physx::PxCooking *pCooking;

        physx::PxDefaultCpuDispatcher *pCpuDispatcher;

        physx::PxScene *pScene;
        physx::PxSceneDesc *pPhysDec;

        physx::PxControllerManager *pControlManager;
    };

} // DemonPhysics

#endif //DEMONENGINE_DP_PHYSICSMANAGER_H