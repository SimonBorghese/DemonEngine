//
// Created by simon on 6/9/22.
//

#include "DP_RigidPhysicsActor.h"

namespace DemonPhysics {
    void DP_RigidPhysicsActor::createActor(physx::PxPhysics *physx, physx::PxMaterial *mat){
        _mainActor = (physx::PxRigidDynamic*) physx->createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f)));
        physx::PxTransform relativePose(physx::PxQuat(1, physx::PxVec3(0.0f,0.0f,0.0f)));


        _mainShape = physx::PxRigidActorExt::createExclusiveShape(*_mainActor,
                                                                  physx::PxConvexMeshGeometry(targetMesh->getConvexMesh()),
                                                                  *mat);

        // END MESH GEN
        if (_mainShape != NULL){
            _mainShape->setLocalPose(relativePose);
        }
        else{
            _mainActor = NULL;
            return;
        }
        //((physx::PxRigidDynamic*) _mainActor)
        getRealActor()->setMass(1.0f);
        getRealActor()->setMassSpaceInertiaTensor(physx::PxVec3(1.0f));
        physx::PxRigidBodyExt::updateMassAndInertia(*getRealActor(), 5.0f);

    }
} // DemonPhysics