//
// Created by simon on 6/2/22.
//

#include "DP_PhysicsManager.h"

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
        if (!pFoundation) {
            printf("Error: PxCreateFoundation Failed, Line: %d\n", __LINE__);
            return;
        }

        /*
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
        */

        // Create the physics
        // Add PVD to end for PVD
        pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pFoundation, physx::PxTolerancesScale(), true);
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

        // Begin creating our scene
        pPhysDec = new physx::PxSceneDesc(physx::PxTolerancesScale());
        // Assign gravity to our assigned one
        pPhysDec->gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);

        if (!pPhysDec->cpuDispatcher) {
            // Create CPU dispatcher with 2 threads (for now)
            pCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
            if (!pCpuDispatcher)
                printf("PxDefaultCpuDispatcherCreate failed!\n");
            pPhysDec->cpuDispatcher = pCpuDispatcher;

        }
        if (!pPhysDec->filterShader)
            pPhysDec->filterShader = NotifyAllFilterShader;



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
} // DemonPhysics