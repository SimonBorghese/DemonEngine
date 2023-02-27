//
// Created by simon on 6/2/22.
//
#ifndef DEMONENGINE_DP_PHYSICSMANAGER_H
#define DEMONENGINE_DP_PHYSICSMANAGER_H

#include <glm/glm.hpp>
#include "DP_RigidConvexMesh.h"
#include "DP_RigidActor.h"
#include <DemonBase/b_RigidActor.h>
#include <DemonBase/b_RigidMesh.h>
#include "DP_PhysicsMaterial.h"
#include <PxPhysicsAPI.h>
#include <extensions/PxTriangleMeshExt.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <characterkinematic/PxController.h>

using namespace physx;

namespace DemonPhysics {

    class DP_PhysicsManager {
    public:
        DP_PhysicsManager() {}

        void createPhysics(glm::vec3 gravity = glm::vec3(0.0f));

        //void beginSimulationTiming();
        void simulate(float simulationTime) {
            if (simulationTime < 0.0f){
                printf("Got simulation time less than 0: %f\n", simulationTime);
                assert(simulationTime > 0.0f);
            }
            pScene->simulate(simulationTime);
            pScene->fetchResults(true);
        }

        void closePhysics();

        void addActor(DemonBase::b_RigidActor *actor) { pScene->addActor(*actor->getActor()); }

        void removeActor(DemonBase::b_RigidActor *actor) { pScene->removeActor(*actor->getActor()); }

        void cookMesh(DemonBase::b_RigidMesh *targetMesh) { targetMesh->createMesh(pPhysics, pCooking); }

        void cookActor(DP_RigidActor *targetActor, DP_PhysicsMaterial *mat) {
            targetActor->createActor(pPhysics, mat->getMaterial());
        }

        void cookMaterial(DP_PhysicsMaterial *targetMat) { targetMat->createMaterial(pPhysics); }

        physx::PxRaycastBuffer rayCast(glm::vec3 origin, glm::vec3 direction, float distance);
        physx::PxSweepBuffer   sweepCube(glm::vec3 origin, glm::vec3 direction, float distance, glm::vec3 cubeScale);


        physx::PxPhysics *getPhysics() { return pPhysics; }

        physx::PxCooking *getCooking() { return pCooking; }

        physx::PxScene *getScene() { return pScene; }

        physx::PxControllerManager *getControllerManager() { return pControlManager; }

        physx::PxCudaContextManager *getCudaContext() { return pCudaContextManager; }

    private:
        physx::PxDefaultErrorCallback pError;
        physx::PxDefaultAllocator pAllocate;

        physx::PxPhysics *pPhysics;
        physx::PxFoundation *pFoundation;

        physx::PxPvd *pPvd;
        physx::PxPvdTransport *pTransport;

        physx::PxCooking *pCooking;

        physx::PxDefaultCpuDispatcher *pCpuDispatcher;

        physx::PxScene *pScene;
        physx::PxSceneDesc *pPhysDec;

        physx::PxControllerManager *pControlManager;

        physx::PxCudaContextManager *pCudaContextManager;
    };

} // DemonPhysics

#endif //DEMONENGINE_DP_PHYSICSMANAGER_H