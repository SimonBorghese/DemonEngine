//
// Created by simon on 6/13/22.
//

#ifndef DEMONENGINE_WORLD_H
#define DEMONENGINE_WORLD_H

#include <vector>
#include <DemonGame/Shared/DG_Entity.h>
#include <DemonGame/Shared/DG_RigidEntity.h>
#include <DemonGame/Shared/DG_PhysicsObject.h>
#include <DemonBase/b_Light.h>
#include <DemonGL/DemonGL.h>
#include <DemonMacro/DemonBench.h>

namespace DemonEngine {

    class World {
    public:
        World(DemonPhysics::DP_PhysicsManager *phys) : _physManager(phys) {}

        int addWorldGeneric(DemonGame::DG_Entity *ent);

        int addWorldObject(DemonGame::DG_RigidEntity *ent);

        int addWorldEntity(DemonGame::DG_PhysicsObject *ent);

        int addLightEntity(DGL::Light *light);

        void updateAll(DGL::Shader *overrideShader = nullptr, glm::mat4 view = glm::mat4(0.0f), glm::mat4 projection = glm::mat4(0.0f), DGL::MeshRenderer::MESH_FLAGS flagCheck = DGL::MeshRenderer::MESH_FLAGS::NO_FLAG);

        void clearAll();

        void destroyWorld();

        DemonGame::DG_Entity *removeWorldGeneric(unsigned int pointer);

        DemonGame::DG_RigidEntity *removeWorldObject(unsigned int pointer);

        DemonGame::DG_PhysicsObject *removeWorldEntity(unsigned int pointer);

        void removeWorldEntityValue(DemonGame::DG_PhysicsObject * pointer);

        void removeWorldObjectValue(DemonGame::DG_RigidEntity * pointer);

        void removeWorldGenericValue(DemonGame::DG_Entity * pointer);

        void removeLightEntityValue(DGL::Light * pointer);

        DGL::Light *removeLightEntity(unsigned int pointer);

    private:
        std::vector<DemonGame::DG_Entity *> _genericEntites;
        std::vector<DemonGame::DG_RigidEntity *> _worldObjects;
        std::vector<DemonGame::DG_PhysicsObject *> _worldEntites;
        std::vector<DGL::Light*> _lightEntites;

        DemonPhysics::DP_PhysicsManager *_physManager;

    };

} // DemonEngine

#endif //DEMONENGINE_WORLD_H
