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

            generalStruct.name = "UNNAMED";
            strncpy(&generalStruct.magicString[0], "IOBJ", sizeof(char) * 5);
            generalStruct.type = DemonGame::TRIGGER;
            generalStruct.structReference = &objDesc;

            strncpy(&objDesc.magicString[0], "IOBJ", sizeof(char) * 5);
            objDesc.triggerActivate = nullptr;
        }

        void setName(const char * name) { generalStruct.name = std::string(name); }
        std::string getName() { return generalStruct.name; }

        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        void destroyEntity();

        void update();

        void setCallback(std::function<void(DG_Object*, bool)> callback) { objDesc.triggerActivate =  callback;}

        DemonPhysics::DP_RigidActor *getActor() { return rigidActor; }
    protected:
        DemonPhysics::DP_PhysicsManager *physicsManager;
        DemonPhysics::DP_RigidTriggerActor *rigidActor;
        DemonPhysics::DP_RigidConvexMesh *rigidMesh;
        DemonPhysics::DP_PhysicsMaterial *mainMaterial;

        DemonBase::DemonUserData::generalStruct generalStruct;
        DemonBase::DemonUserData::DP_TRIGGER_OBJ_DESC objDesc;
    };

} // DemonGame

#endif //DEMONENGINE_DG_TRIGGER_H
