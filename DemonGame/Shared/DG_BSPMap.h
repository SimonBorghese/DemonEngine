//
// Created by simon on 7/9/22.
//

#ifndef DEMONENGINE_DG_BSPMAP_H
#define DEMONENGINE_DG_BSPMAP_H

#include "DG_Entity.h"
#include <DemonPhysics/DP_RigidTriangleMesh.h>
#include <DemonPhysics/DP_RigidActor.h>
#include <DemonPhysics/DP_RigidPhysicsActor.h>
#include <DemonRender/DR_RenderManager.h>
#include <DemonPhysics/DP_PhysicsManager.h>
#include <DemonBase/b_MeshEntity.h>
#include <CBSP/CBSP.h>

namespace DemonGame {

    class DG_BSPMap: public DG_Entity {
    public:
        DG_BSPMap(DemonRender::DR_RenderManager *targetRender,
        DemonRender::DR_Shader *targetShader,
                DemonPhysics::DP_PhysicsManager *targetManager) : DG_Entity(targetRender, targetShader),
        physicsManager(targetManager) {
        }

        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        void createEntityFromCBSP(CBSP *map,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        void destroyEntity();

        void update();

        DemonPhysics::DP_RigidActor *getActor() { return rigidActor; }

    protected:
        DemonPhysics::DP_PhysicsManager *physicsManager;
        DemonPhysics::DP_RigidActor *rigidActor;
        DemonPhysics::DP_RigidConvexMesh *rigidMesh;
        DemonPhysics::DP_PhysicsMaterial *mainMaterial;

    };

} // DemonGame

#endif //DEMONENGINE_DG_BSPMAP_H
