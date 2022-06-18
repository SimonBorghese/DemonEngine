//
// Created by simon on 6/13/22.
//

#ifndef DEMONENGINE_WORLD_H
#define DEMONENGINE_WORLD_H

#include <vector>
#include <DemonGame/Shared/DG_Entity.h>
#include <DemonGame/Shared/DG_RigidEntity.h>
#include <DemonGame/Shared/DG_PhysicsObject.h>

namespace DemonEngine {

    class World {
    public:
        World() {}

        int addWorldGeneric(DemonGame::DG_Entity *ent);

        int addWorldObject(DemonGame::DG_RigidEntity *ent);

        int addWorldEntity(DemonGame::DG_PhysicsObject *ent);

        void updateAll();

        void clearAll();

        DemonGame::DG_Entity *removeWorldGeneric(unsigned int pointer);

        DemonGame::DG_RigidEntity *removeWorldObject(unsigned int pointer);

        DemonGame::DG_PhysicsObject *removeWorldEntity(unsigned int pointer);

    private:
        std::vector<DemonGame::DG_Entity *> _genericEntites;
        std::vector<DemonGame::DG_RigidEntity *> _worldObjects;
        std::vector<DemonGame::DG_PhysicsObject *> _worldEntites;

    };

} // DemonEngine

#endif //DEMONENGINE_WORLD_H
