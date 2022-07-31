//
// Created by simon on 7/31/22.
//

#ifndef DEMONENGINE_DP_RIGIDTRIANGLEMESH_H
#define DEMONENGINE_DP_RIGIDTRIANGLEMESH_H
#include <DemonBase/b_RigidMesh.h>

namespace DemonPhysics {

    class DP_RigidTriangleMesh : public DemonBase::b_RigidMesh{
    public:
        DP_RigidTriangleMesh(Vertex *vertices, unsigned int vertexLen, unsigned int *indices, unsigned int indiceLen) : DemonBase::b_RigidMesh(vertices, vertexLen) {
            triangleMeshDesc.points.count = (vertexLen / 3);
            triangleMeshDesc.points.stride = sizeof(Vertex);
            triangleMeshDesc.points.data = &(vertices[0]);

            triangleMeshDesc.triangles.count = indiceLen / 3;
            triangleMeshDesc.triangles.stride = sizeof(unsigned int) * 3;
            triangleMeshDesc.triangles.data = indices;
            triangleMeshDesc.flags = physx::PxMeshFlag::eFLIPNORMALS;
            //triangleMeshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
        }
        DP_RigidTriangleMesh(DemonBase::b_Mesh *bMesh) : DemonBase::b_RigidMesh(bMesh) {
            triangleMeshDesc.points.count = (_vertices.size());
            triangleMeshDesc.points.stride = sizeof(Vertex);
            triangleMeshDesc.points.data = &(_vertices.data()[0]);

            triangleMeshDesc.triangles.count = _indices.size() / 3;
            triangleMeshDesc.triangles.stride = sizeof(unsigned int) * 3;
            triangleMeshDesc.triangles.data = &(_indices.data()[0]);
            triangleMeshDesc.flags = physx::PxMeshFlag::eFLIPNORMALS;

            //convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
        }


        void createMesh(physx::PxPhysics *physx, physx::PxCooking *cooking);
        void destroyMesh();

        physx::PxGeometry* getGeometry() { return _geometry;}
    private:
        physx::PxTriangleMeshDesc triangleMeshDesc;
        physx::PxTriangleMesh *_mesh = nullptr;

    };

} // DemonPhysics

#endif //DEMONENGINE_DP_RIGIDTRIANGLEMESH_H
