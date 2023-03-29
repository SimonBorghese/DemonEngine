//
// Created by simon on 9/23/22.
//

#ifndef DEMONENGINE_DG_TRIGGER_H
#define DEMONENGINE_DG_TRIGGER_H
#include "DG_Entity.h"
#include <functional>
#include <DemonPhysics/DP_RigidActor.h>
#include <DemonPhysics/DP_RigidConvexMesh.h>
#include <DemonPhysics/DP_RigidTriangleMesh.h>
#include <DemonPhysics/DP_PhysicsManager.h>
#include <DemonBase/b_PhysUserData.h>
#include <DemonPhysics/DP_RigidTriggerActor.h>

namespace DemonGame {

    class DG_Trigger : public DG_Entity{
    public:
        DG_Trigger(DGL::Shader *targetShader,
                DemonPhysics::DP_PhysicsManager *targetManager) : DG_Entity(targetShader),
        physicsManager(targetManager) {
            strcpy(&clientInfo.magicString[0], MAGIC_STRING);
            clientInfo.client = nullptr;
        }

        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        void createEntityFromExistingMesh(DemonBase::b_Mesh **meshes, uint32_t numMesh,
                                          glm::vec3 pos = glm::vec3(0.0f),
                                          glm::vec3 rotation = glm::vec3(0.0f));

        void destroyEntity();

        void update();

        void setClient(GameClient *client) { clientInfo.client = client; }

        DemonPhysics::DP_RigidActor *getActor() { return rigidActor; }
    protected:
        DemonPhysics::DP_PhysicsManager *physicsManager;
        DemonPhysics::DP_RigidTriggerActor *rigidActor;
        DemonPhysics::DP_RigidConvexMesh *rigidMesh;
        DemonPhysics::DP_PhysicsMaterial *mainMaterial;

        DemonGame::ClientInfo clientInfo;
    };

} // DemonGame

#endif //DEMONENGINE_DG_TRIGGER_H
