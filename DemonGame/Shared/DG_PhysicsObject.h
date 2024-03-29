//
// Created by simon on 6/10/22.
//

#ifndef DEMONENGINE_DG_PHYSICSOBJECT_H
#define DEMONENGINE_DG_PHYSICSOBJECT_H
#include "DG_Entity.h"
#include <DemonBase/b_RigidMesh.h>
#include <DemonPhysics/DP_RigidConvexMesh.h>
#include <DemonPhysics/DP_RigidActor.h>
#include <DemonPhysics/DP_RigidPhysicsActor.h>
#include <DemonGL/DemonGL.h>
#include <DemonPhysics/DP_PhysicsManager.h>
#include <functional>
#include <string>

#include <DemonBase/b_PhysUserData.h>

namespace DemonGame {

    class DG_PhysicsObject : public DG_Entity {
    public:
        DG_PhysicsObject(DGL::Shader *targetShader,
                         DemonPhysics::DP_PhysicsManager *targetManager) :
                DG_Entity(targetShader),
                physicsManager(targetManager) {
            strcpy(&clientInfo.magicString[0], MAGIC_STRING);
            clientInfo.client = nullptr;
        }


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

        DemonPhysics::DP_RigidPhysicsActor *getActor() { return rigidActor; }

        float getMass() { return rigidActor->getRealActor()->getMass(); }
        void setMass(float newMass) { rigidActor->getRealActor()->setMass(newMass); }

        void disableGravity() {
            rigidActor->getRealActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
        }
        void enableGravity() {
            rigidActor->getRealActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
        }

        glm::vec3 getSpaceMassInertiaTensor() { return glm::vec3(
        rigidActor->getRealActor()->getMassSpaceInertiaTensor().x,
        rigidActor->getRealActor()->getMassSpaceInertiaTensor().y,
        rigidActor->getRealActor()->getMassSpaceInertiaTensor().z); }

        void setSpaceMassInertiaTensor(glm::vec3 tensor) {
            rigidActor->getRealActor()->setMassSpaceInertiaTensor(physx::PxVec3(tensor.x, tensor.y, tensor.z));
        }

        void updateMassInertia(float desnity) {
            rigidActor->updateMassInteria(desnity);
        }

        void setPosition(glm::vec3 newPos) {
            rigidActor->setPosition(newPos);
        }

        void setMaterial(DemonPhysics::DP_PhysicsMaterial *newMat) { mainMaterial = newMat; }

        void setClient(GameClient *client) { clientInfo.client = client; }

    protected:
        DemonPhysics::DP_PhysicsManager *physicsManager;
        DemonPhysics::DP_RigidPhysicsActor *rigidActor;
        DemonPhysics::DP_RigidConvexMesh *rigidMesh;
        DemonPhysics::DP_PhysicsMaterial *mainMaterial = nullptr;

        DemonGame::ClientInfo clientInfo;
    };
} // DemonGame

#endif //DEMONENGINE_DG_PHYSICSOBJECT_H
