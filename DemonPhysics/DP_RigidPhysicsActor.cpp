//
// Created by simon on 6/9/22.
//

#include "DP_RigidPhysicsActor.h"

namespace DemonPhysics {
    void DP_RigidPhysicsActor::createActor(physx::PxPhysics *physx, physx::PxMaterial *mat) {
        _mainActor = (physx::PxRigidDynamic *) physx->createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f)));
        _ptrActorDynamic = (physx::PxRigidDynamic *) _mainActor;
        physx::PxTransform relativePose(physx::PxQuat(1, physx::PxVec3(0.0f, 0.0f, 0.0f)));


        if (_targetMeshes.size() < 1) {
            _mainShapes.push_back(physx::PxRigidActorExt::createExclusiveShape(*_mainActor,
                                                                               physx::PxConvexMeshGeometry(
                                                                                       targetMesh->getConvexMesh()),
                                                                               *mat));
        }
        else{
            for (unsigned  int m = 0; m < _targetMeshes.size(); m++){
                _mainShapes.push_back(physx::PxRigidActorExt::createExclusiveShape(*_mainActor,
                                                                                   physx::PxConvexMeshGeometry(
                                                                                           _targetMeshes.at(m)->getConvexMesh()),
                                                                                   *mat));
                // END MESH GEN
                if (_mainShapes.at(_mainShapes.size()-1) != NULL) {
                    _mainShapes.at(_mainShapes.size()-1)->setLocalPose(relativePose);
                } else {
                    _mainActor = NULL;
                    return;
                }
            }
        }

        //((physx::PxRigidDynamic*) _mainActor)
        getRealActor()->setMass(1.0f);
        getRealActor()->setMassSpaceInertiaTensor(physx::PxVec3(1.0f));
        physx::PxRigidBodyExt::updateMassAndInertia(*getRealActor(), 1.0f);

    }
} // DemonPhysics