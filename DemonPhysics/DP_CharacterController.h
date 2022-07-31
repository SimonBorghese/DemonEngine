//
// Created by simon on 6/13/22.
//

#ifndef DEMONENGINE_DP_CHARACTERCONTROLLER_H
#define DEMONENGINE_DP_CHARACTERCONTROLLER_H

#include <PhysX/characterkinematic/PxController.h>
#include <PhysX/characterkinematic/PxCapsuleController.h>
#include <DemonBase/b_Controller.h>

namespace DemonPhysics {

    class DP_CharacterController : public DemonBase::b_Controller {
    public:
        DP_CharacterController(glm::vec3 position, physx::PxMaterial *targetMaterial,
                               float height, float radius, physx::PxControllerManager *controllerManager,
                               float stepOffset = 2.0f, float scaleCoeff = 1.0f, float volumeGrowth = 1.0f,
                               float density = 2.0f, float slopeLimit = 90.0f) :
                _characterHeight(height),
                _characterRadius(radius),
                _stepOffset(stepOffset),
                _scaleCoeff(scaleCoeff),
                _volumeGrowth(volumeGrowth),
                _density(density),
                _slopeLimit(slopeLimit) {
            pMainMat = targetMaterial;
            physx::PxCapsuleControllerDesc desc;
            desc.height = _characterHeight;
            desc.radius = _characterRadius;
            desc.climbingMode = physx::PxCapsuleClimbingMode::eEASY;
            desc.slopeLimit = _slopeLimit;
            desc.density = _density;
            desc.volumeGrowth = _volumeGrowth;
            desc.slopeLimit = _slopeLimit;
            desc.scaleCoeff = _scaleCoeff;
            desc.stepOffset = _stepOffset;
            desc.maxJumpHeight = 2.0f;
            desc.contactOffset = 1.0f;
            desc.material = pMainMat;
            desc.position = physx::PxExtendedVec3(position.x, position.y, position.z);

            pMainController = controllerManager->createController(desc);
        }

        void setHeight(float height){
            ((physx::PxCapsuleController*)pMainController)->setHeight(height);
        }
        float getHeight() {
            return ((physx::PxCapsuleController*)pMainController)->getHeight();
        }

        void setRadius(float height){
            ((physx::PxCapsuleController*)pMainController)->setRadius(height);
        }
        float getRadius() {
            return ((physx::PxCapsuleController*)pMainController)->getRadius();
        }

    private:
        float _characterHeight, _characterRadius, _stepOffset, _scaleCoeff, _volumeGrowth, _density, _slopeLimit;

    };

} // DemonPhysics

#endif //DEMONENGINE_DP_CHARACTERCONTROLLER_H
