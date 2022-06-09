//
// Created by simon on 6/9/22.
//

#ifndef DEMONENGINE_DP_PHYSICSMATERIAL_H
#define DEMONENGINE_DP_PHYSICSMATERIAL_H
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxMaterial.h>

namespace DemonPhysics{
    class DP_PhysicsMaterial{
    public:
        DP_PhysicsMaterial(float staticFric = 1.0f, float dynamicFric = 1.0f, float _interia = 1.0f) :
        staticFriction(staticFric),
        dynamicFriction(dynamicFric),
        intertia(_interia) {}

        void createMaterial(physx::PxPhysics *phys){
            mat = phys->createMaterial(staticFriction, dynamicFriction, intertia);
        }
        void destroyMaterial(){
            mat->release();
        }

        physx::PxMaterial* getMaterial() { return mat; }
    private:
        float staticFriction;
        float dynamicFriction;
        float intertia;
        physx::PxMaterial *mat;

    };
}

#endif //DEMONENGINE_DP_PHYSICSMATERIAL_H
