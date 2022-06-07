#ifndef PHYSICS_MAT
#define PHYSICS_MAT
#include "PxMaterial.h"
#include <PhysX/PxPhysicsAPI.h>
#include <IncuhPhysics/PhysicsCore.hpp>

// Tiny class for material mangement
// Small enough for it to be header only
namespace IncuhPhysics {
    class PhysicsMaterial {
    public:
        PhysicsMaterial(float staticFriction, float dynamicFriction, float resitiution, PhysicsCore *core,
                        float iMass = 0.0f, float iSIM = 0.0f) : pMass(iMass), pInertiamass(iSIM) {
            pMaterial = core->getPhysics()->createMaterial(staticFriction, dynamicFriction, resitiution);
            pMaterial->setFlags(physx::PxMaterialFlag::eIMPROVED_PATCH_FRICTION);
        }

        ~PhysicsMaterial() { pMaterial->release(); }

        physx::PxMaterial *getMaterial() { return (pMaterial); }

        float getMass() { return (pMass); }

        float getSpaceinteriamass() { return (pInertiamass); }

    private:
        physx::PxMaterial *pMaterial;

        float pMass;
        float pInertiamass;
    };
}
#endif
