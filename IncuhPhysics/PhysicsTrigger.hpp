#ifndef PHYSICSTRIGGER_HPP
#define PHYSICSTRIGGER_HPP
#include <IncuhPhysics/PhysicsObject.hpp>
#include <IncuhPhysics/PhysicsMaterial.hpp>
#include <IncuhPhysics/PhysicsCore.hpp>
//#include <stdScale.h>

namespace IncuhPhysics {
    class PhysicsTrigger : public PhysicsObject {
    public:
        PhysicsTrigger(const char *name, physx::PxVec3 position, float scale, PhysicsMaterial *dMat, PhysicsCore *core);

        virtual ~PhysicsTrigger();

    private:
        PhysicsMaterial *pMaterial;
    };
}

#endif // PHYSICSTRIGGER_HPP
