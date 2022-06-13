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
} // DemonEngine