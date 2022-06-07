#include <IncuhPhysics/PhysicsCapsuleController.hpp>
namespace IncuhPhysics {
    PhysicsCapsuleController::PhysicsCapsuleController(PhysicsMaterial *dMat, float height, float radius,
                                                       float stepOffset, float scaleCoeff, float volumeGrowth,
                                                       float density, float slopeLimit) {
        pMainController = NULL;
        pMaterial = dMat;
        pChrDec = new physx::PxCapsuleControllerDesc();
        pChrDec->height = height;
        pChrDec->radius = radius;
        pChrDec->stepOffset = stepOffset;
        pChrDec->material = pMaterial->getMaterial();
        pChrDec->scaleCoeff = scaleCoeff;
        pChrDec->volumeGrowth = volumeGrowth;
        pChrDec->density = density;
        pChrDec->slopeLimit = slopeLimit;
        pChrDec->climbingMode = physx::PxCapsuleClimbingMode::eEASY;
        pChrDec->maxJumpHeight = 2.0f;
    }

    PhysicsCapsuleController::~PhysicsCapsuleController() {
        if (pMainController) {
            pMainController->release();
        }
        if (pMaterial) {
            delete pMaterial;
        }
        delete pChrDec;
    }

    void PhysicsCapsuleController::makeConfig(physx::PxControllerManager *controllerManager) {
        pMainController = controllerManager->createController(*pChrDec);
        pMainController->setUserData((void *) 0);
        //pMainController->setPosition(physx::PxExtendedVec3())
    }
}
