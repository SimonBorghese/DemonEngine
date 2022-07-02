//
// Created by simon on 6/9/22.
//

#ifndef DEMONENGINE_B_RIGIDACTOR_H
#define DEMONENGINE_B_RIGIDACTOR_H

#include <PhysX/PxRigidActor.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <DemonWorld/DW_Transform.h>


namespace DemonBase {

    class b_RigidActor {
    public:

        b_RigidActor() { _mainTransform.createTransform(); }

        physx::PxRigidActor *getActor() { return _mainActor; }

        physx::PxShape *getShape() { return _mainShape; }

        DemonWorld::DW_Transform *getTransform() { return &_mainTransform; }

        void setTransform(DemonWorld::DW_Transform transform) {
            _mainTransform = transform;
            physx::PxTransform targetTransform;
            targetTransform.p = DemonWorld::DW_Transform::glmToPhys(_mainTransform.getPosition());
            glm::quat tq = _mainTransform.getRotation();
            targetTransform.q = physx::PxQuat(tq.x, tq.y, tq.z, tq.w);
            _mainActor->setGlobalPose(targetTransform);
        }

        void updateActor() {
            physx::PxTransform targetTransform;
            targetTransform.p = DemonWorld::DW_Transform::glmToPhys(_mainTransform.getPosition());
            glm::quat tq = _mainTransform.getRotation();
            targetTransform.q = physx::PxQuat(tq.x, tq.y, tq.z, tq.w);

            //_mainActor->setGlobalPose()

            glm::vec3 pos = DemonWorld::DW_Transform::PhysToGlm(_mainActor->getGlobalPose().p);
            glm::quat quat = glm::quat(_mainActor->getGlobalPose().q.w, _mainActor->getGlobalPose().q.x,
                                       _mainActor->getGlobalPose().q.y, _mainActor->getGlobalPose().q.z);

            //_mainTransform.createTransform(DemonWorld::DW_Transform::PhysToGlm(pos), )
            _mainTransform.setPosition(pos);
            _mainTransform.setRotation(quat);
        }

        void setEmbedData(void *data){
            _mainActor->userData = data;
        }

        void* getEmbedData(){
            return _mainActor->userData;
        }

        glm::vec3 getPosition() {
            return glm::vec3(_mainActor->getGlobalPose().p.x,
                             _mainActor->getGlobalPose().p.y,
                             _mainActor->getGlobalPose().p.z);
        }

        glm::quat getRotation(){
            return glm::quat(_mainActor->getGlobalPose().q.w,
                             _mainActor->getGlobalPose().q.x,
                             _mainActor->getGlobalPose().q.y,
                             _mainActor->getGlobalPose().q.z);
        }
        void setPosition(glm::vec3 position) {
            //printf("Setting 0 %f %f %f\n", target[0], target[1], target[2]);
            _mainActor->setGlobalPose(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), _mainActor->getGlobalPose().q));
        }

        void setRotation(glm::quat rotation) {
            //printf("Setting 0 %f %f %f\n", target[0], target[1], target[2]);
            _mainActor->setGlobalPose(physx::PxTransform(_mainActor->getGlobalPose().p, physx::PxQuat( rotation.x, rotation.y, rotation.z, rotation.w )));

        }

    protected:
        physx::PxRigidActor *_mainActor;
        physx::PxShape *_mainShape;
        DemonWorld::DW_Transform _mainTransform;
    };

} // DemonBase

#endif //DEMONENGINE_B_RIGIDACTOR_H
