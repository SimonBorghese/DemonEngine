//
// Created by simon on 6/9/22.
//

#ifndef DEMONENGINE_DP_PHYSICSMATERIAL_H
#define DEMONENGINE_DP_PHYSICSMATERIAL_H

#include <PxPhysicsAPI.h>
#include <PxMaterial.h>

namespace DemonPhysics {
    class DP_PhysicsMaterial {
    public:
        DP_PhysicsMaterial(float staticFric = 1.0f, float dynamicFric = 1.0f, float _restitution = 1.0f) :
                staticFriction(staticFric),
                dynamicFriction(dynamicFric),
                restitution(_restitution) {}

        void createMaterial(physx::PxPhysics *phys) {
            mat = phys->createMaterial(staticFriction, dynamicFriction, restitution);
#ifndef _PHYSX3
            mat->setFlags(physx::PxMaterialFlag::eIMPROVED_PATCH_FRICTION);
#endif
            mat->setFlag(physx::PxMaterialFlag::eCOMPLIANT_CONTACT, true);
        }

        void destroyMaterial() {
            if (mat != nullptr && mat->isReleasable()) {
                mat->release();
            }
        }

        physx::PxMaterial *getMaterial() { return mat; }

    private:
        float staticFriction;
        float dynamicFriction;
        float restitution;
        physx::PxMaterial *mat;

    };
}

#endif //DEMONENGINE_DP_PHYSICSMATERIAL_H
