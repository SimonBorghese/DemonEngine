//
// Created by simon on 6/9/22.
//

#ifndef DEMONENGINE_DP_RIGIDPHYSICSACTOR_H
#define DEMONENGINE_DP_RIGIDPHYSICSACTOR_H
#include "DP_RigidActor.h"

namespace DemonPhysics {

    class DP_RigidPhysicsActor : public DP_RigidActor {
    public:
        DP_RigidPhysicsActor(DP_RigidMesh *mesh) : DP_RigidActor(mesh) {}

        void createActor(physx::PxPhysics *physx, physx::PxMaterial *mat);
        physx::PxRigidDynamic* getRealActor() { return (physx::PxRigidDynamic*)_mainActor; }
    private:


    };

} // DemonPhysics

#endif //DEMONENGINE_DP_RIGIDPHYSICSACTOR_H
