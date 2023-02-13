//
// Created by simon on 9/23/22.
//

#ifndef DEMONENGINE_DP_RIGIDTRIGGERACTOR_H
#define DEMONENGINE_DP_RIGIDTRIGGERACTOR_H
#include "DP_RigidActor.h"

namespace DemonPhysics {

    class DP_RigidTriggerActor : public DP_RigidActor{
    public:
        DP_RigidTriggerActor(DemonBase::b_RigidMesh *mesh) : DP_RigidActor(mesh) {}
        DP_RigidTriggerActor(std::vector<DemonBase::b_RigidMesh*> meshes) : DP_RigidActor(meshes) {}

        void createActor(physx::PxPhysics *physx, physx::PxMaterial *mat);

        void destroyActor();
    private:

    };

} // DemonPhysics

#endif //DEMONENGINE_DP_RIGIDTRIGGERACTOR_H
