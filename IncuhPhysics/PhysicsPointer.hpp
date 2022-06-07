#ifndef PHYSICSPOINTER_HPP
#define PHYSICSPOINTER_HPP
#include <IncuhPhysics/PhysicsObject.hpp>
#include <string>
#include <vector>

namespace IncuhPhysics {
    class PhysicsPointer {
    public:
        PhysicsPointer(physx::PxRigidActor *body, physx::PxShape *shape, std::string name) {
            pMainRigid = body;
            pMainShape = shape;
            this->name = name.c_str();
        }

        void getPosition(float *target) {
            if (target == NULL || pMainRigid == NULL) {
                return;
            }
            //printf("getting 0 %f %f %f\n", pMainRigid->getGlobalPose().p.x, pMainRigid->getGlobalPose().p.y, pMainRigid->getGlobalPose().p.z);
            target[0] = pMainRigid->getGlobalPose().p.x;
            target[1] = pMainRigid->getGlobalPose().p.y;
            target[2] = pMainRigid->getGlobalPose().p.z;
        }

        void getRotation(float *target) {
            if (target == NULL || pMainRigid == NULL) {
                return;
            }
            target[0] = pMainRigid->getGlobalPose().q.w;
            target[1] = pMainRigid->getGlobalPose().q.x;
            target[2] = pMainRigid->getGlobalPose().q.y;
            target[3] = pMainRigid->getGlobalPose().q.z;
        }

        void setPosition(float *target) {
            if (target == NULL || pMainRigid == NULL) {
                return;
            }
            //printf("Setting 0 %f %f %f\n", target[0], target[1], target[2]);
            pMainRigid->setGlobalPose(
                    physx::PxTransform(physx::PxVec3(target[0], target[1], target[2]), pMainRigid->getGlobalPose().q));
        }

        void setRotation(float *target) {
            if (target == NULL || pMainRigid == NULL) {
                return;
            }
            //printf("Setting 0 %f %f %f\n", target[0], target[1], target[2]);
            pMainRigid->setGlobalPose(physx::PxTransform(pMainRigid->getGlobalPose().p,
                                                         physx::PxQuat(target[0], target[1], target[2], target[3])));
        }

        physx::PxRigidActor *pMainRigid;
        physx::PxShape *pMainShape;
        const char *name;
    };
}

#endif // PHYSICSPOINTER_HPP
