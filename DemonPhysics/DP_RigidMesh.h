//
// Created by simon on 6/6/22.
//

#ifndef DEMONENGINE_DP_RIGIDMESH_H
#define DEMONENGINE_DP_RIGIDMESH_H
#include <DemonBase/b_Mesh.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxShape.h>

namespace DemonPhysics {

    class DP_RigidMesh: public DemonBase::b_Mesh {
    public:
        DP_RigidMesh(Vertex *vertices, unsigned int vertexLen);
        DP_RigidMesh(DemonBase::b_Mesh *bMesh);

        physx::PxConvexMesh* getConvexMesh() { return _mesh; }

        void createMesh(physx::PxPhysics *physx, physx::PxCooking *cooking);

        void destroyMesh();

    private:
        //std::vector<float> _rawVertices;
        physx::PxConvexMeshDesc convexDesc;
        physx::PxConvexMesh *_mesh = nullptr;

    };

} // DemonPhysics

#endif //DEMONENGINE_DP_RIGIDMESH_H
