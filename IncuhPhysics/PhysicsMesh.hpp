#ifndef PHYSICS_MESH
#define PHYSICS_MESH
#include <IncuhPhysics/PhysicsObject.hpp>
#include <IncuhPhysics/PhysicsMaterial.hpp>
#include <IncuhPhysics/PhysicsCore.hpp>

#include <vector>
namespace IncuhPhysics {
    class PhysicsMesh : public PhysicsObject {
    public:
        PhysicsMesh(physx::PxVec3 position, std::vector<float> *vertices, RigidTypes rigidType, PhysicsMaterial *dMat,
                    PhysicsCore *core, void *currentModel = NULL);

        PhysicsMesh(physx::PxVec3 position, std::vector<float> *vertices, std::vector<uint32_t> *indicies,
                    RigidTypes rigidType, PhysicsMaterial *dMat, PhysicsCore *core, void *currentModel = NULL);

        virtual ~PhysicsMesh();

    private:
        physx::PxTriangleMesh *pTriMesh;
        physx::PxConvexMesh *pConvexMesh;
        PhysicsMaterial *pMainMaterial;


    };
}

#endif
