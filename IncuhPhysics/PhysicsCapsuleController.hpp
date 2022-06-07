#ifndef CAPSULE_CONTROLLER
#define CAPSULE_CONTROLLER
#include <IncuhPhysics/PhysicsController.hpp>
#include <IncuhPhysics/PhysicsMaterial.hpp>
#include <IncuhPhysics/PhysicsCore.hpp>

namespace IncuhPhysics {
    class PhysicsCapsuleController : public PhysicsController {
    public:
        PhysicsCapsuleController(PhysicsMaterial *dMat, float height, float radius, float stepOffset, float scaleCoeff,
                                 float volumeGrowth, float density, float slopeLimit);

        virtual ~PhysicsCapsuleController();

        void makeConfig(physx::PxControllerManager *controllerManager);

    private:
        physx::PxCapsuleControllerDesc *pChrDec;
        PhysicsMaterial *pMaterial;

    };
}

#endif
