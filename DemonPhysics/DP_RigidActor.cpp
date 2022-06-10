//
// Created by simon on 6/7/22.
//

#include "DP_RigidActor.h"

namespace DemonPhysics {
    void DP_RigidActor::createActor(physx::PxPhysics *physx, physx::PxMaterial *mat){
        _mainActor = (physx::PxRigidStatic*) physx->createRigidStatic(physx::PxTransform(physx::PxVec3(0.0f)));
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
    }

    void DP_RigidActor::destroyActor(){
        _mainActor->detachShape(*_mainShape);
        _mainActor->release();
        //_mainShape->release();
    }
} // DemonPhysics