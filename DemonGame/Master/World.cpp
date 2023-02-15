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

    int World::addLightEntity(DGL::Light *light){
        _lightEntites.push_back(light);
        return _lightEntites.size() - 1;
    }

    void World::updateAll(DGL::Shader *overrideShader, glm::mat4 view, glm::mat4 projection,  DGL::MeshRenderer::MESH_FLAGS flagCheck) {
        if (view != glm::mat4(0.0f)){
            overrideShader->setView(view);
            overrideShader->setProjection(projection);
        }

        if (flagCheck == DGL::MeshRenderer::MESH_FLAGS::NO_FLAG) {
            // Update Generics
            for (unsigned int g = 0; g < _genericEntites.size(); g++) {
                _genericEntites.at(g)->update(overrideShader);
            }

            for (unsigned int g = 0; g < _worldObjects.size(); g++) {
                _worldObjects.at(g)->update(overrideShader);
            }

            for (unsigned int g = 0; g < _worldEntites.size(); g++) {
                _worldEntites.at(g)->update(overrideShader);
            }
        } else{
            // Update Generics
            for (unsigned int g = 0; g < _genericEntites.size(); g++) {
                if (_genericEntites.at(g)->getMeshRenderer()->compareFlag(flagCheck)) {
                    _genericEntites.at(g)->update(overrideShader);
                }
            }

            for (unsigned int g = 0; g < _worldObjects.size(); g++) {
                if  (_worldObjects.at(g)->getMeshRenderer()->compareFlag(flagCheck)) {
                    _worldObjects.at(g)->update(overrideShader);
                }
            }

            for (unsigned int g = 0; g < _worldEntites.size(); g++) {
                if  (_worldEntites.at(g)->getMeshRenderer()->compareFlag(flagCheck)) {
                    _worldEntites.at(g)->update(overrideShader);
                }
            }
        }
    }

    void World::clearAll() {
        _genericEntites.clear();
        _worldObjects.clear();
        _worldEntites.clear();
        _lightEntites.clear();
    }

    void World::destroyWorld(){
        for (auto genericEntity : _genericEntites){
            //removeWorldGenericValue(genericEntity);
            genericEntity->destroyEntity();
            delete genericEntity;
        }

        for (auto worldObjects : _worldObjects){
            //removeWorldObjectValue(worldObjects);
            worldObjects->destroyEntity();
            delete worldObjects;
        }

        for (auto worldEntity = _worldEntites.begin(); worldEntity != _worldEntites.end(); worldEntity++){
            //removeWorldEntityValue(*worldEntity);
            printf("Freeing 1\n");
            if (worldEntity != std::next(worldEntity) && worldEntity != std::prev(worldEntity)) {
                (*worldEntity)->destroyEntity();
                delete *worldEntity;
            } else{
                break;
            }
        }

        for (auto lightEntity : _lightEntites){
            //lightEntity->destroyLight();
            //lightEntity->renderLight();
            removeLightEntityValue(lightEntity);
        }

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

    void World::removeWorldEntityValue(DemonGame::DG_PhysicsObject *  pointer) {
        std::remove(_worldEntites.begin(), _worldEntites.end(),pointer);
    }

    void World::removeWorldObjectValue(DemonGame::DG_RigidEntity *  pointer) {
        std::remove(_worldObjects.begin(), _worldObjects.end(),pointer);
    }
    void World::removeWorldGenericValue(DemonGame::DG_Entity *  pointer) {
        std::remove(_genericEntites.begin(), _genericEntites.end(),pointer);
    }

    void World::removeLightEntityValue(DGL::Light * pointer){
        std::remove(_lightEntites.begin(), _lightEntites.end(), pointer);
    }

    DGL::Light *World::removeLightEntity(unsigned int pointer){
        DGL::Light *target = _lightEntites.at(pointer);
        _lightEntites.erase(_lightEntites.cbegin() + pointer);
        return target;
    }
} // DemonEngine