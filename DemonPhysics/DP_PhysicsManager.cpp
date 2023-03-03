//
// Created by simon on 6/2/22.
//

#include "DP_PhysicsManager.h"
// Private include?
#include <DemonGame/Shared/External/DG_EXT_PhysicsCallback.h>


namespace DemonPhysics {

    PxFilterFlags NotifyAllFilterShader(
            PxFilterObjectAttributes attributes0, PxFilterData,
            PxFilterObjectAttributes attributes1, PxFilterData,
            PxPairFlags& pairFlags, const void*, PxU32)
    {
        if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
            return PxFilterFlag::eDEFAULT;
        }

        // Until it becomes a problem, lets notify all contacts
        pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;

        return PxFilterFlag::eDEFAULT;
    }


    void DP_PhysicsManager::createPhysics(glm::vec3 gravity) {
        pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pAllocate, pError);
        //printf("Physics version: %x\n", PX_PHYSICS_VERSION);
        if (!pFoundation) {
            printf("Error: PxCreateFoundation Failed, Line: %d\n", __LINE__);
            return;
        }


        pPvd = physx::PxCreatePvd(*pFoundation);
        if (!pPvd){
            printf("Error: PxCreatePvd Failed, Line: %d\n", __LINE__);
            return;
        }
        pTransport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
        if (!pTransport){
            printf("Error: PxDefaultPvdSocketTransportCreate failed, Line: %d\n", __LINE__);
            return;
        }
        pPvd->connect(*pTransport, physx::PxPvdInstrumentationFlag::eALL);


        // Create the physics
        // Add PVD to end for PVD
        pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pFoundation, physx::PxTolerancesScale(), true, pPvd);
        if (pPhysics == NULL) {
            printf("Failed to Create Physics, Line: %d\n", __LINE__);
            return;
        }

        physx::PxTolerancesScale scale_px = physx::PxTolerancesScale();
        //scale_px.speed = 0.0001f;


        pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pFoundation, physx::PxCookingParams(scale_px));
        if (!pCooking) {
            printf("PxCreateCooking failed!\n");
            return;
        }

        // Initalize extensions
        if (!PxInitExtensions(*pPhysics, NULL)) {
            printf("Failed to initalize extensions\n");
            return;
        }

#ifndef DEMON_NO_GPU_PHYSX
        PxCudaContextManagerDesc cudaContextManagerDesc;

        pCudaContextManager = PxCreateCudaContextManager(*pFoundation, cudaContextManagerDesc, PxGetProfilerCallback());

#endif

        // Begin creating our scene
        pPhysDec = new physx::PxSceneDesc(physx::PxTolerancesScale());
        // Assign gravity to our assigned one
        pPhysDec->gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);

        if (!pPhysDec->cpuDispatcher) {
            // Create CPU dispatcher with 2 threads (for now)
            pCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
            if (!pCpuDispatcher)
                printf("PxDefaultCpuDispatcherCreate failed!\n");
            pPhysDec->cpuDispatcher = pCpuDispatcher;

        }
        if (!pPhysDec->filterShader)
            pPhysDec->filterShader = NotifyAllFilterShader;

        pPhysDec->broadPhaseType = physx::PxBroadPhaseType::eSAP;
        pPhysDec->flags |= PxSceneFlag::eENABLE_STABILIZATION;

        pPhysDec->simulationEventCallback = new DemonGame::DG_EXT_PhysicsCallback;

#ifndef DEMON_NO_GPU_PHYSX
      pPhysDec->cudaContextManager = pCudaContextManager;
      pPhysDec->flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
      pPhysDec->broadPhaseType = physx::PxBroadPhaseType::eGPU;
#endif




        // Create our scene
        pScene = pPhysics->createScene(*pPhysDec);
        if (pScene == NULL) {
            printf("Failed to create scene\n");
            return;
        }

        // Make our character controller manager
        pControlManager = PxCreateControllerManager(*pScene);
        if (!pControlManager) {
            printf("Error: PxCreateControllerManager failed, Line: %d\n", __LINE__);
            return;
        }

        physx::PxRaycastBuffer  b;

    }

    void DP_PhysicsManager::closePhysics() {
        pControlManager->release();
        pScene->release();
        pCpuDispatcher->release();
        delete pPhysDec;
        pCooking->release();
        PxCloseExtensions();
        pPhysics->release();
        pFoundation->release();
    }

    physx::PxRaycastBuffer DP_PhysicsManager::rayCast(glm::vec3 origin, glm::vec3 direction, float distance){
        PxRaycastBuffer hit;
        bool success = pScene->raycast(physx::PxVec3(origin.x, origin.y, origin.z),
                                       physx::PxVec3(direction.x, direction.y, direction.z),
                                       distance, hit);
        if (!success){
            //printf("[Physics] RAY CAST FAILED!\n");
            hit.nbTouches = 0;
            hit.hasBlock = false;
        }
        return hit;

    }

    physx::PxSweepBuffer DP_PhysicsManager::sweepCube(glm::vec3 origin, glm::vec3 direction, float distance, glm::vec3 cubeScale){
        physx::PxSweepBuffer hit;

        PxBoxGeometry sweepGeo = PxBoxGeometry(physx::PxVec3(cubeScale.x, cubeScale.y, cubeScale.z));
        bool success = pScene->sweep(sweepGeo, physx::PxTransform(physx::PxVec3(origin.x, origin.y, origin.z)),
                                     physx::PxVec3(direction.x, direction.y, direction.z),
                                     distance, hit);
        if (!success){
            //printf("[Physics] SWEEP CAST FAILED!\n");
            hit.nbTouches = 0;
            hit.hasBlock = false;
        }
        return hit;
    }
} // DemonPhysics