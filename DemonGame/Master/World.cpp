//
// Created by simon on 6/13/22.
//

#include "World.h"

namespace DemonEngine {

    int World::addWorldGeneric(DemonGame::DG_Entity *ent) {
        _genericEntites.push_back(ent);
        return _genericEntites.size() - 1;
    }

    int World::addWorldObject(DemonGame::DG_RigidEntity *ent) {
        _worldObjects.push_back(ent);
        return _worldObjects.size() - 1;
    }

    int World::addWorldEntity(DemonGame::DG_PhysicsObject *ent) {
        _worldEntites.push_back(ent);
        return _worldEntites.size() - 1;
    }

    int World::addLightEntity(DemonBase::b_Light *light){
        _lightEntites.push_back(light);
        return _lightEntites.size() - 1;
    }

    void World::updateAll() {
        // Update Lights
        for (unsigned int l = 0; l < _lightEntites.size(); l++){
            _lightEntites.at(l)->renderLight();
        }

        // Update Generics
        for (unsigned int g = 0; g < _genericEntites.size(); g++) {
            _genericEntites.at(g)->update();
        }

        for (unsigned int g = 0; g < _worldObjects.size(); g++) {
            _worldObjects.at(g)->update();
        }

        for (unsigned int g = 0; g < _worldEntites.size(); g++) {
            _worldEntites.at(g)->update();
        }
    }

    void World::clearAll() {
        _genericEntites.clear();
        _worldObjects.clear();
        _worldEntites.clear();
    }

    DemonGame::DG_Entity *World::removeWorldGeneric(unsigned int pointer) {
        DemonGame::DG_Entity *target = _genericEntites.at(pointer);
        _genericEntites.erase(_genericEntites.cbegin() + pointer);
        return target;
    }

    DemonGame::DG_RigidEntity *World::removeWorldObject(unsigned int pointer) {
        DemonGame::DG_RigidEntity *target = _worldObjects.at(pointer);
        _worldObjects.erase(_worldObjects.cbegin() + pointer);
        return target;
    }

    DemonGame::DG_PhysicsObject *World::removeWorldEntity(unsigned int pointer) {
        DemonGame::DG_PhysicsObject *target = _worldEntites.at(pointer);
        _worldEntites.erase(_worldEntites.cbegin() + pointer);
        return target;
    }

    DemonBase::b_Light *World::removeLightEntity(unsigned int pointer){
        DemonBase::b_Light *target = _lightEntites.at(pointer);
        _lightEntites.erase(_lightEntites.cbegin() + pointer);
        return target;
    }
} // DemonEngine