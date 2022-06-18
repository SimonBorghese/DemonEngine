//
// Created by simon on 6/10/22.
//

#ifndef DEMONENGINE_DG_PHYSICSOBJECT_H
#define DEMONENGINE_DG_PHYSICSOBJECT_H

#include "DG_Entity.h"
#include <DemonPhysics/DP_RigidActor.h>
#include <DemonPhysics/DP_RigidPhysicsActor.h>
#include <DemonRender/DR_RenderManager.h>
#include <DemonPhysics/DP_PhysicsManager.h>

namespace DemonGame {

    class DG_PhysicsObject : public DG_Entity {
    public:
        DG_PhysicsObject(DemonRender::DR_RenderManager *targetRender,
                         DemonRender::DR_Shader *targetShader,
                         DemonPhysics::DP_PhysicsManager *targetManager) :
                DG_Entity(targetRender, targetShader),
                physicsManager(targetManager) {
            //rigidActor = new DemonPhysics::DP_RigidActor(rigidMesh);
        }

        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        void destroyEntity();

        void update();

        DemonPhysics::DP_RigidActor *getActor() { return rigidActor; }

    protected:
        DemonPhysics::DP_PhysicsManager *physicsManager;
        DemonPhysics::DP_RigidPhysicsActor *rigidActor;
        DemonPhysics::DP_RigidMesh *rigidMesh;
        DemonPhysics::DP_PhysicsMaterial *mainMaterial;
    };
} // DemonGame

#endif //DEMONENGINE_DG_PHYSICSOBJECT_H
