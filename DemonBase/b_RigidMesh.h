//
// Created by simon on 7/31/22.
//

#ifndef DEMONENGINE_B_RIGIDMESH_H
#define DEMONENGINE_B_RIGIDMESH_H
#include "b_Mesh.h"
#include <PxPhysicsAPI.h>
#include <PxShape.h>



namespace DemonBase {

    class b_RigidMesh : public b_Mesh {
    public:
        enum TYPE{
            CONVEX,
            TRIANGLE
        };

        b_RigidMesh(Vertex *vertices, unsigned int vertexLen) : DemonBase::b_Mesh(vertices, vertexLen, NULL,
                                                                                  0){}
        b_RigidMesh(DemonBase::b_Mesh *bMesh) :
                DemonBase::b_Mesh(bMesh->getVerticesVector().data(),
                                  bMesh->getVerticesVector().size(),
                                  bMesh->getIndicesVector().data(),
                                  bMesh->getIndicesVector().size()) {}

        virtual ~b_RigidMesh() {
            free(_geometry);
        }


        virtual void createMesh(physx::PxPhysics *physx, physx::PxCooking *cooking) = 0;

        physx::PxGeometry* getGeometry() { return _geometry;}

        virtual void destroyMesh() = 0;

    protected:
        physx::PxGeometry *_geometry = nullptr;

    };

} // DemonBase

#endif //DEMONENGINE_B_RIGIDMESH_H
