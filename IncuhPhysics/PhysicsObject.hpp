#ifndef PHYSICS_OBJECT
#define PHYSICS_OBJECT
#include <PhysX/PxPhysicsAPI.h>
#include <string>

// A abstract class for physics objects

namespace IncuhPhysics {
    enum RigidTypes {
        STATIC,
        DYNAMIC
    };

    class PhysicsObject {
    public:

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

        void applyForce(float *direction, float force) {
            if (direction == NULL) {
                return;
            }
            pMainRigid->addForce(physx::PxVec3(direction[0] * force, direction[1] * force, direction[2] * force),
                                 physx::PxForceMode::eACCELERATION);
            //pMainRigid->setAngularVelocity()
        }

        void setVelocity(float *direction, float force) {
            if (direction == NULL) {
                return;
            }
            pMainRigid->setLinearVelocity(
                    physx::PxVec3(direction[0] * force, direction[1] * force, direction[2] * force));
            //pMainRigid->setAngularVelocity()

        }


        physx::PxRigidBody *getRigidBody() { return pMainRigid; }

        physx::PxMaterial *getMaterial() { return pMainMat; }

        physx::PxShape *getShape() { return pMainShape; }

        void setName(const char *newName) {
            pMainRigid->setName(std::string(newName).c_str());
            name = newName;
        }

        const char *getName() { return name; }

    protected:
        physx::PxRigidBody *pMainRigid;
        physx::PxMaterial *pMainMat;
        physx::PxShape *pMainShape;
        const char *name;
    };
}

#endif
