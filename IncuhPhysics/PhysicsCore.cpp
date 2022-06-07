#include <IncuhPhysics//PhysicsCore.hpp>
namespace IncuhPhysics {

    PhysicsCore::PhysicsCore(physx::PxVec3 gravity) {
        // Create our foundation
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
        pPhysDec->gravity = gravity;

        if (!pPhysDec->cpuDispatcher) {
            // Create CPU dispatcher with 2 threads (for now)
            pCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
            if (!pCpuDispatcher)
                printf("PxDefaultCpuDispatcherCreate failed!\n");
            pPhysDec->cpuDispatcher = pCpuDispatcher;

        }
        if (!pPhysDec->filterShader)
            pPhysDec->filterShader = physx::PxDefaultSimulationFilterShader;


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

        //pScene->setSimulationEventCallback(new PhysicsTriggerCallback());

    }

    PhysicsCore::~PhysicsCore() { cleanUp(); }

    void PhysicsCore::addPhysicsObject(PhysicsObject *object) {
        if (object->getRigidBody() != NULL) {
            pScene->addActor(*(object->getRigidBody()));
        }
    }

    void PhysicsCore::addPhysicsController(PhysicsController *controller, float *startPos) {
        controller->makeConfig(pControlManager);
        if (startPos == NULL) { return; }
        controller->translate(startPos);
    }

    void PhysicsCore::simulate(float step) {
        pScene->simulate(step);
        pScene->fetchResults(true);
    }

    void PhysicsCore::cleanUp() {
        delete pScene->getSimulationEventCallback();
        pControlManager->release();
        delete pPhysDec;
        pScene->release();
        pCpuDispatcher->release();
        PxCloseExtensions();
        pCooking->release();
        pPhysics->release();
        //pPvd->disconnect();
        //pPvd->release();
        pFoundation->release();
    }

    void PhysicsCore::softCleanup() {

        delete pScene->getSimulationEventCallback();

        pScene->flushSimulation();
        pControlManager->release();
        pScene->release();
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
}