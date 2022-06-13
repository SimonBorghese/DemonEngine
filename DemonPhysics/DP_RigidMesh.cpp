//
// Created by simon on 6/6/22.
//

#include "DP_RigidMesh.h"

namespace DemonPhysics {
    DP_RigidMesh::DP_RigidMesh(Vertex *vertices, unsigned int vertexLen) : DemonBase::b_Mesh(vertices, vertexLen, NULL,0){
        convexDesc.points.count     = (vertexLen/3);
        convexDesc.points.stride    = sizeof(Vertex);
        convexDesc.points.data      = &(vertices[0]);
        convexDesc.flags            = physx::PxConvexFlag::eCOMPUTE_CONVEX;
    }

    DP_RigidMesh::DP_RigidMesh(DemonBase::b_Mesh *bMesh) :
            DemonBase::b_Mesh(bMesh->getVerticesVector().data(),
                              bMesh->getVerticesVector().size(),
                              bMesh->getIndicesVector().data(),
                              bMesh->getIndicesVector().size()){
        convexDesc.points.count     = (_vertices.size()/3);
        convexDesc.points.stride    = sizeof(Vertex);
        convexDesc.points.data      = &(_vertices.data()[0]);
        convexDesc.flags            = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    }

    void DP_RigidMesh::createMesh(physx::PxPhysics *physx, physx::PxCooking *cooking){
        physx::PxDefaultMemoryOutputStream buf;
        physx::PxConvexMeshCookingResult::Enum result;
        if(!cooking->cookConvexMesh(convexDesc, buf, &result)){
            printf("FAILED TO MAKE CONVEX MESH: %d\n", result);
        }
        physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        _mesh = physx->createConvexMesh(input);
    }

    void DP_RigidMesh::destroyMesh(){
        _mesh->release();
    }
} // DemonPhysics