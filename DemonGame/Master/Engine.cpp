//
// Created by simon on 6/10/22.
//

#include "Engine.h"

namespace DemonEngine {

    void Engine::createEngine() {
        // Create the rendering manager
        _renderingManager->createRenderer(GAME_NAME, _width, _height);

        // Create the object shader
        _shaderObject->createProgram(fmt::format("{}/{}", GAME_SHADER_DIRECTORY, GAME_OBJECT_SHADER_VERTEX).c_str(),
                                     fmt::format("{}/{}", GAME_SHADER_DIRECTORY, GAME_OBJECT_SHADER_FRAGMENT).c_str());

        // Create the camera
        _mainCamera->configureProjection((float) _fov, (float) _width / (float) _height, 0.1f, (float) _zFar);
        _renderingManager->setCamera(_mainCamera);

        _mainPhysicsManager->createPhysics(glm::vec3(0.0f, -9.81f, 0.0f));

        _renderingManager->newFrame();
        _mainEvents->pollEvents();
    }

    int Engine::gameLoop() {
        _currentTicks = SDL_GetTicks();
        _renderingManager->render();
        _mainPhysicsManager->simulate(1.0f / 60.0f);

        _renderingManager->newFrame();
        _mainEvents->pollEvents();
        _mainPlayer->updateCamera();
        _world->updateAll();

        return _mainEvents->getCloseState() || _mainEvents->getKeyDown(SDL_SCANCODE_ESCAPE);
    }


    void Engine::destroyEngine() {
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

    DemonGame::DG_RigidEntity *Engine::createWorldObject() {
        DemonGame::DG_RigidEntity *goodEnt = new DemonGame::DG_RigidEntity(_renderingManager, _shaderObject,
                                                                           _mainPhysicsManager);
        _world->addWorldObject(goodEnt);
        return goodEnt;
    }

    DemonGame::DG_PhysicsObject *Engine::createWorldEntity() {
        DemonGame::DG_PhysicsObject *goodEnt = new DemonGame::DG_PhysicsObject(_renderingManager, _shaderObject,
                                                                               _mainPhysicsManager);
        _world->addWorldEntity(goodEnt);
        return goodEnt;
    }

    DemonRender::DemonLight::DR_DL_BasicLight* Engine::createPointLight(POINT_LIGHT_INFO info){
        DemonRender::DemonLight::DR_DL_BasicLight_CREATE_INFO  lightCreateInfo;
        lightCreateInfo.distance = info.distance;
        lightCreateInfo.colour = info.colour;
        lightCreateInfo.specularStrength = info.specularStrength;
        lightCreateInfo.specularAccuracy = info.specularAccuracy;
        lightCreateInfo.ambientStrength = info.ambientStrength;
        lightCreateInfo.position = info.position;

        DemonRender::DemonLight::DR_DL_BasicLight *goodLight =
                new DemonRender::DemonLight::DR_DL_BasicLight(getObjectShader());
        goodLight->initLight(lightCreateInfo);
        _world->addLightEntity(goodLight);
        return goodLight;
    }
    DemonRender::DemonLight::DR_DL_BasicLight* Engine::createSpotLight(SPOT_LIGHT_INFO info){
        DemonRender::DemonLight::DR_DL_BasicLight_CREATE_INFO  lightCreateInfo;
        lightCreateInfo.distance = info.distance;
        lightCreateInfo.colour = info.colour;
        lightCreateInfo.specularStrength = info.specularStrength;
        lightCreateInfo.specularAccuracy = info.specularAccuracy;
        lightCreateInfo.ambientStrength = info.ambientStrength;
        lightCreateInfo.position = info.position;
        lightCreateInfo.direction = info.direction;
        lightCreateInfo.cutOffDegree = info.cutOffDegree;
        lightCreateInfo.outerCutOffDegree = info.outerCutOffDegree;

        DemonRender::DemonLight::DR_DL_BasicLight *goodLight =
                new DemonRender::DemonLight::DR_DL_BasicLight(getObjectShader());
        goodLight->initLight(lightCreateInfo);
        _world->addLightEntity(goodLight);
        return goodLight;
    }
    DemonRender::DemonLight::DR_DL_BasicLight* Engine::createDirectionalLight(DIRECTIONAL_LIGHT_INFO info){
        DemonRender::DemonLight::DR_DL_BasicLight_CREATE_INFO  lightCreateInfo;
        lightCreateInfo.colour = info.colour;
        lightCreateInfo.specularStrength = info.specularStrength;
        lightCreateInfo.specularAccuracy = info.specularAccuracy;
        lightCreateInfo.ambientStrength = info.ambientStrength;
        lightCreateInfo.direction = info.direction;

        DemonRender::DemonLight::DR_DL_BasicLight *goodLight =
                new DemonRender::DemonLight::DR_DL_BasicLight(getObjectShader());
        goodLight->initLight(lightCreateInfo);
        _world->addLightEntity(goodLight);
        return goodLight;
    }

    DemonRender::DemonLight::DR_DL_BasicLight* Engine::createEasyPointLight(glm::vec3 position, float distance, float strength, glm::vec3 colour){
        POINT_LIGHT_INFO hardInfo; // :flushed:
        hardInfo.position = position;
        hardInfo.distance = distance;
        hardInfo.colour = colour;
        hardInfo.specularAccuracy = 32;
        hardInfo.specularStrength = strength;
        hardInfo.ambientStrength = strength;
        return createPointLight(hardInfo); // Create a HARD point light :flushed:
    }
    DemonRender::DemonLight::DR_DL_BasicLight* Engine::createEasySpotLight(glm::vec3 position, glm::vec3 direction, float angle, float distance, float strength, glm::vec3 colour){
        SPOT_LIGHT_INFO hardInfo;
        hardInfo.position = position;
        hardInfo.direction = direction;
        hardInfo.cutOffDegree = angle;
        hardInfo.outerCutOffDegree = angle+2.0f;
        hardInfo.distance = distance;
        hardInfo.ambientStrength = strength;
        hardInfo.specularStrength = strength;
        hardInfo.specularAccuracy = 32;
        hardInfo.colour = colour;
        return createSpotLight(hardInfo);
    }
    DemonRender::DemonLight::DR_DL_BasicLight* Engine::createEasyDirectionalLight(glm::vec3 direction, float strength, glm::vec3 colour){
        DIRECTIONAL_LIGHT_INFO hardInfo;
        hardInfo.direction = direction;
        hardInfo.specularStrength = strength;
        hardInfo.ambientStrength = strength;
        hardInfo.specularAccuracy = 32;
        hardInfo.colour = colour;
        return createDirectionalLight(hardInfo);
    }

    DemonPhysics::DP_CharacterController *Engine::createFPSController(glm::vec3 startPos, float height, float radius) {
        DemonPhysics::DP_PhysicsMaterial mat;
        mat.createMaterial(getPhysicsManager()->getPhysics());
        DemonPhysics::DP_CharacterController *controller = new DemonPhysics::DP_CharacterController(glm::vec3(startPos),
                                                                                                    mat.getMaterial(),
                                                                                                    height, radius,
                                                                                                    getPhysicsManager()->getControllerManager());
        getCameraController()->setController(controller);
        return controller;
    }
} // DemonEngine