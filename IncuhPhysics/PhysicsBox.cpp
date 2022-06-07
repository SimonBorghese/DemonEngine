#include <IncuhPhysics/PhysicsBox.hpp>

namespace IncuhPhysics {

    PhysicsBox::PhysicsBox(physx::PxVec3 position, physx::PxVec3 scale, RigidTypes rigidType, PhysicsMaterial *dMat,
                           PhysicsCore *core) {
        pMainMat = dMat->getMaterial();
        if (rigidType == DYNAMIC) {
            pMainRigid = core->getPhysics()->createRigidDynamic(physx::PxTransform(position));
            physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));

            pMainShape = physx::PxRigidActorExt::createExclusiveShape(*pMainRigid,
                                                                      physx::PxBoxGeometry(scale.y, scale.x, scale.z),
                                                                      *pMainMat);

            pMainShape->setLocalPose(relativePose);
            pMainRigid->setMass(dMat->getMass());
            pMainRigid->setMassSpaceInertiaTensor(physx::PxVec3(dMat->getSpaceinteriamass()));
            physx::PxRigidBodyExt::updateMassAndInertia(*pMainRigid, 1.0f);
            //scene->addActor(*dMainRigid);
        } else {
            pMainRigid = (physx::PxRigidBody *) core->getPhysics()->createRigidStatic(physx::PxTransform(position));
            physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));

            pMainShape = physx::PxRigidActorExt::createExclusiveShape(*pMainRigid,
                                                                      physx::PxBoxGeometry(scale.y, scale.x, scale.z),
                                                                      *pMainMat);
            //pMainShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
            //pMainShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

            pMainShape->setLocalPose(relativePose);

        }
    }

    PhysicsBox::~PhysicsBox() {
        pMainMat->release();
        pMainRigid->release();
        pMainShape->release();

    }
}
