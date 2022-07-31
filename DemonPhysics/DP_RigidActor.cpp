//
// Created by simon on 6/7/22.
//

#include "DP_RigidActor.h"

namespace DemonPhysics {
    void DP_RigidActor::createActor(physx::PxPhysics *physx, physx::PxMaterial *mat) {
        _mainActor = (physx::PxRigidStatic *) physx->createRigidStatic(physx::PxTransform(physx::PxVec3(0.0f)));
        physx::PxTransform relativePose(physx::PxQuat(1, physx::PxVec3(0.0f, 0.0f, 0.0f)));

        if (_targetMeshes.size() < 1) {
            _mainShapes.push_back(physx::PxRigidActorExt::createExclusiveShape(*_mainActor,
                                                                                       *targetMesh->getGeometry(),
                                                                               *mat));
        }
        else{
            for (unsigned  int m = 0; m < _targetMeshes.size(); m++){
                _mainShapes.push_back(physx::PxRigidActorExt::createExclusiveShape(*_mainActor,
                                                                                   *_targetMeshes.at(m)->getGeometry(),
                                                                                   *mat));
                _mainActor->attachShape(*_mainShapes.at(_mainShapes.size()-1));
                // END MESH GEN
                if (_mainShapes.at(_mainShapes.size()-1) != NULL) {
                    _mainShapes.at(_mainShapes.size()-1)->setLocalPose(relativePose);
                } else {
                    _mainActor = NULL;
                    return;
                }
            }
        }
    }

    void DP_RigidActor::destroyActor() {
        for (unsigned  int m = 0; m < _mainShapes.size(); m++) {
            _mainActor->detachShape(*_mainShapes.at(m));
            _mainShapes.at(m)->release();
        }

        _mainActor->release();
        //_mainShape->release();
    }
} // DemonPhysics