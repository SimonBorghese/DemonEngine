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

namespace DemonEngine {

    class World {
    public:
        World() {}

        int addWorldGeneric(DemonGame::DG_Entity *ent);

        int addWorldObject(DemonGame::DG_RigidEntity *ent);

        int addWorldEntity(DemonGame::DG_PhysicsObject *ent);

        int addLightEntity(DemonBase::b_Light *light);

        void updateAll();

        void clearAll();

        DemonGame::DG_Entity *removeWorldGeneric(unsigned int pointer);

        DemonGame::DG_RigidEntity *removeWorldObject(unsigned int pointer);

        DemonGame::DG_PhysicsObject *removeWorldEntity(unsigned int pointer);

        void removeWorldEntityValue(DemonGame::DG_PhysicsObject * pointer);

        void removeWorldObjectValue(DemonGame::DG_RigidEntity * pointer);

        void removeWorldGenericValue(DemonGame::DG_Entity * pointer);

        DemonBase::b_Light *removeLightEntity(unsigned int pointer);

    private:
        std::vector<DemonGame::DG_Entity *> _genericEntites;
        std::vector<DemonGame::DG_RigidEntity *> _worldObjects;
        std::vector<DemonGame::DG_PhysicsObject *> _worldEntites;
        std::vector<DemonBase::b_Light*> _lightEntites;

    };

} // DemonEngine

#endif //DEMONENGINE_WORLD_H
