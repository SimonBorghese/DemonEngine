//
// Created by simon on 6/2/22.
//
#ifndef DEMONENGINE_DP_PHYSICSMANAGER_H
#define DEMONENGINE_DP_PHYSICSMANAGER_H
#include <glm/glm.hpp>
#include "DP_RigidMesh.h"
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

        void createRigidMesh(DP_RigidMesh *target);

        void closePhysics();
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