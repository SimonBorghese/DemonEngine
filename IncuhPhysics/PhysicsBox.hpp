#ifndef PHYSICS_BOX
#define PHYSICS_BOX
#include <IncuhPhysics/PhysicsObject.hpp>
#include <IncuhPhysics/PhysicsMaterial.hpp>
#include <IncuhPhysics/PhysicsCore.hpp>

namespace IncuhPhysics {
    class PhysicsBox : public PhysicsObject {
    public:
        PhysicsBox(physx::PxVec3 position, physx::PxVec3 scale, RigidTypes rigidType, PhysicsMaterial *dMat,
                   PhysicsCore *core);

        virtual ~PhysicsBox();

    private:


    };
}

#endif