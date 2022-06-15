//
// Created by simon on 6/10/22.
//

#include "Engine.h"

namespace DemonEngine {

    void Engine::createEngine(){
        // Create the rendering manager
        _renderingManager->createRenderer(GAME_NAME, _width, _height);

        // Create the object shader
        _shaderObject->createProgram(fmt::format("{}/{}", GAME_SHADER_DIRECTORY, GAME_OBJECT_SHADER_VERTEX).c_str(), fmt::format("{}/{}", GAME_SHADER_DIRECTORY, GAME_OBJECT_SHADER_FRAGMENT).c_str());

        // Create the camera
        _mainCamera->configureProjection((float) _fov, (float) _width / (float) _height, 0.1f, (float) _zFar);
        _renderingManager->setCamera(_mainCamera);

        _mainPhysicsManager->createPhysics(glm::vec3(0.0f, -9.81f, 0.0f));

        _renderingManager->newFrame();
        _mainEvents->pollEvents();
    }

    int Engine::gameLoop(){
        _renderingManager->render();
        _mainPhysicsManager->simulate(1.0f/60.0f);

        _renderingManager->newFrame();
        _mainEvents->pollEvents();
        _mainPlayer->updateCamera();
        _world->updateAll();

        return _mainEvents->getCloseState();
    }


    void Engine::destroyEngine(){
        delete _mainPlayer;

        _mainPhysicsManager->closePhysics();
        delete _mainPhysicsManager;

        delete _mainEvents;

        delete _mainCamera;

        _shaderObject->destroyProgram();
        delete _shaderObject;

        _renderingManager->destroyRenderer();
        delete _renderingManager;
    }

    DemonGame::DG_RigidEntity* Engine::createWorldObject(){
        DemonGame::DG_RigidEntity *goodEnt = new DemonGame::DG_RigidEntity(_renderingManager, _shaderObject, _mainPhysicsManager);
        _world->addWorldObject(goodEnt);
        return goodEnt;
    }
    DemonGame::DG_PhysicsObject* Engine::createWorldEntity(){
        DemonGame::DG_PhysicsObject *goodEnt = new DemonGame::DG_PhysicsObject(_renderingManager, _shaderObject, _mainPhysicsManager);
        _world->addWorldEntity(goodEnt);
        return goodEnt;
    }

    DemonPhysics::DP_CharacterController* Engine::createFPSController(glm::vec3 startPos, float height, float radius){
        DemonPhysics::DP_PhysicsMaterial mat;
        mat.createMaterial(getPhysicsManager()->getPhysics());
        DemonPhysics::DP_CharacterController *controller = new DemonPhysics::DP_CharacterController(glm::vec3(startPos), mat.getMaterial(),  height, radius, getPhysicsManager()->getControllerManager());
        getCameraController()->setController(controller);
        return controller;
    }
} // DemonEngine