//
// Created by simon on 7/31/22.
//

#ifndef DEMONENGINE_DP_RIGIDCONVEXMESH_H
#define DEMONENGINE_DP_RIGIDCONVEXMESH_H
#include <DemonBase/b_RigidMesh.h>

namespace DemonPhysics {

    class DP_RigidConvexMesh : public DemonBase::b_RigidMesh{
    public:
        DP_RigidConvexMesh(Vertex *vertices, unsigned int vertexLen) : DemonBase::b_RigidMesh(vertices, vertexLen) {
            convexDesc.points.count = (vertexLen / 3);
            convexDesc.points.stride = sizeof(Vertex);
            convexDesc.points.data = &(vertices[0]);
            convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
        }
        DP_RigidConvexMesh(DemonBase::b_Mesh *bMesh) : DemonBase::b_RigidMesh(bMesh) {
            convexDesc.points.count = (_vertices.size());
            convexDesc.points.stride = sizeof(Vertex);
            convexDesc.points.data = &(_vertices.data()[0]);
            convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
        }


        void createMesh(physx::PxPhysics *physx, physx::PxCooking *cooking);
        void destroyMesh();

        physx::PxGeometry* getGeometry() { return _geometry;}
    private:
        physx::PxConvexMeshDesc convexDesc;
        physx::PxConvexMesh *_mesh = nullptr;

    };

} // DemonPhysics

#endif //DEMONENGINE_DP_RIGIDCONVEXMESH_H
