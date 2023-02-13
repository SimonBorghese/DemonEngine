//
// Created by simon on 6/7/22.
//

#ifndef DEMONENGINE_DG_RIGIDENTITY_H
#define DEMONENGINE_DG_RIGIDENTITY_H

#include "DG_Entity.h"
#include <DemonGL/DemonGL.h>
#include <DemonPhysics/DP_RigidActor.h>
#include <DemonPhysics/DP_RigidConvexMesh.h>
#include <DemonPhysics/DP_RigidTriangleMesh.h>
#include <DemonPhysics/DP_PhysicsManager.h>
#include <DemonBase/b_PhysUserData.h>

namespace DemonGame {

    class DG_RigidEntity : public DG_Entity {
    public:
        DG_RigidEntity(DGL::Shader *targetShader,
                       DemonPhysics::DP_PhysicsManager *targetManager) : DG_Entity(targetShader),
                                                                         physicsManager(targetManager) {
            //rigidActor = new DemonPhysics::DP_RigidActor(rigidMesh);
            generalStruct.name = "UNNAMED";
            strncpy(&generalStruct.magicString[0], "IOBJ", sizeof(char) * 5);
            generalStruct.type = DemonGame::STATIC;
            generalStruct.structReference = &objDesc;

            objDesc.reference = (DG_RigidEntity*) this;
            strncpy(&objDesc.magicString[0], "IOBJ", sizeof(char) * 5);

        }

        void setName(const char * name) { generalStruct.name = std::string(name); }
        std::string getName() { return generalStruct.name; }

        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        // Cannot apply scale here
        void createEntityFromExistingMesh(DemonBase::b_Mesh **meshes, uint32_t numMesh,
                                          glm::vec3 pos = glm::vec3(0.0f),
                                          glm::vec3 rotation = glm::vec3(0.0f));

        void destroyEntity();

        void update(DGL::Shader *overrideShader);

        PxSweepBuffer sweepCast(glm::vec3 origin, glm::vec3 direction, float distance, DemonPhysics::DP_PhysicsManager *manager);

        DemonPhysics::DP_RigidActor *getActor() { return rigidActor; }

    protected:
        DemonPhysics::DP_PhysicsManager *physicsManager;
        DemonPhysics::DP_RigidActor *rigidActor;
        DemonPhysics::DP_RigidConvexMesh *rigidMesh;
        DemonPhysics::DP_PhysicsMaterial *mainMaterial = nullptr;

        DemonBase::DemonUserData::generalStruct generalStruct;
        DemonBase::DemonUserData::DP_RIGID_OBJ_DESC objDesc;
    };

} // DemonGame

#endif //DEMONENGINE_DG_RIGIDENTITY_H
