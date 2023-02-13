//
// Created by simon on 6/7/22.
//

#ifndef DEMONENGINE_DP_RIGIDACTOR_H
#define DEMONENGINE_DP_RIGIDACTOR_H

#include <PxPhysicsAPI.h>
#include <PxRigidActor.h>
#include <DemonBase/b_RigidMesh.h>
#include <DemonBase/b_RigidActor.h>
#include <DemonBase/b_GameObject.h>
#include <glm/glm.hpp>
#include <vector>

namespace DemonPhysics {

    class DP_RigidActor : public DemonBase::b_RigidActor {
    public:
        DP_RigidActor(DemonBase::b_RigidMesh *mesh) : targetMesh(mesh) {}
        DP_RigidActor(std::vector<DemonBase::b_RigidMesh*> meshes) : _targetMeshes(meshes) {}

        void createActor(physx::PxPhysics *physx, physx::PxMaterial *mat);

        void destroyActor();

    protected:
        DemonBase::b_RigidMesh *targetMesh;
        std::vector<DemonBase::b_RigidMesh*> _targetMeshes;
    };

} // DemonPhysics

#endif //DEMONENGINE_DP_RIGIDACTOR_H
