//
// Created by simon on 7/31/22.
//

#include "DP_RigidConvexMesh.h"

namespace DemonPhysics {

    void DP_RigidConvexMesh::createMesh(physx::PxPhysics *physx, physx::PxCooking *cooking) {
        physx::PxDefaultMemoryOutputStream buf;
        physx::PxConvexMeshCookingResult::Enum result;
        if (!cooking->cookConvexMesh(convexDesc, buf, &result)) {
            printf("FAILED TO MAKE CONVEX MESH: %d\n", result);
        }
        physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        _mesh = physx->createConvexMesh(input);

        _geometry = new physx::PxConvexMeshGeometry;
        //*_geometry = physx::PxConvexMeshGeometry(_mesh);
        auto tempGeo = physx::PxConvexMeshGeometry(_mesh);
        memcpy((void*) _geometry, (void*) &tempGeo, sizeof(physx::PxConvexMeshGeometry));
    }

    void DP_RigidConvexMesh::destroyMesh() {
        _mesh->release();
    }
} // DemonPhysics