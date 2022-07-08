//
// Created by simon on 6/7/22.
//

#ifndef DEMONENGINE_DP_RIGIDACTOR_H
#define DEMONENGINE_DP_RIGIDACTOR_H

#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxRigidActor.h>
#include "DP_RigidMesh.h"
#include <DemonBase/b_RigidActor.h>
#include <DemonBase/b_GameObject.h>
#include <glm/glm.hpp>
#include <vector>

namespace DemonPhysics {

    class DP_RigidActor : public DemonBase::b_RigidActor {
    public:
        DP_RigidActor(DP_RigidMesh *mesh) : targetMesh(mesh) {}
        DP_RigidActor(std::vector<DP_RigidMesh*> meshes) : _targetMeshes(meshes) {}

        void createActor(physx::PxPhysics *physx, physx::PxMaterial *mat);

        void destroyActor();

    protected:
        DP_RigidMesh *targetMesh;
        std::vector<DP_RigidMesh*> _targetMeshes;
    };

} // DemonPhysics

#endif //DEMONENGINE_DP_RIGIDACTOR_H
