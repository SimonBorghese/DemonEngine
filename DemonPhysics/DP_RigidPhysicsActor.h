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
        DP_RigidPhysicsActor(std::vector<DP_RigidMesh*> meshes) : DP_RigidActor(meshes) {}

        void createActor(physx::PxPhysics *physx, physx::PxMaterial *mat);

        void applyForce(glm::vec3 forceDirections){
            _ptrActorDynamic->addForce(physx::PxVec3(forceDirections.x, forceDirections.y, forceDirections.z), physx::PxForceMode::eACCELERATION);
        }
        void setVelocity(glm::vec3 forceDirections){
            _ptrActorDynamic->setLinearVelocity(physx::PxVec3(forceDirections.x, forceDirections.y, forceDirections.z));
        }

        physx::PxRigidDynamic *getRealActor() { return (physx::PxRigidDynamic *) _mainActor; }

        void updateMassInteria(float desnsity){ physx::PxRigidBodyExt::updateMassAndInertia(*getRealActor(), desnsity);}


    private:
        physx::PxRigidDynamic *_ptrActorDynamic;

    };

} // DemonPhysics

#endif //DEMONENGINE_DP_RIGIDPHYSICSACTOR_H
