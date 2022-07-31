//
// Created by simon on 7/31/22.
//

#include "DP_RigidTriangleMesh.h"

namespace DemonPhysics {
    void DP_RigidTriangleMesh::createMesh(physx::PxPhysics *physx, physx::PxCooking *cooking) {
        physx::PxDefaultMemoryOutputStream buf;
        physx::PxTriangleMeshCookingResult::Enum result;
        if (!cooking->cookTriangleMesh(triangleMeshDesc, buf, &result)) {
            printf("FAILED TO MAKE CONVEX MESH: %d\n", result);
        }
        physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        _mesh = physx->createTriangleMesh(input);

        _geometry = new physx::PxTriangleMeshGeometry;
        //*_geometry = physx::PxConvexMeshGeometry(_mesh);
        auto tempGeo = physx::PxTriangleMeshGeometry(_mesh);
        memcpy(_geometry, &tempGeo, sizeof(physx::PxTriangleMeshGeometry));
    }

    void DP_RigidTriangleMesh::destroyMesh() {
        _mesh->release();
    }
} // DemonPhysics