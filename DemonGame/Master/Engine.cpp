//
// Created by simon on 6/10/22.
//
#include "Engine.h"


#include <DemonMacro/DemonBench.h>

namespace DemonEngine {

    void Engine::createEngine() {
        // Create the camera
        _defaultCamera->configureProjection((float) _fov, (float) _width / (float) _height, 0.1f, (float) _zFar);
        _enabledCamera = _defaultCamera;


        _debugShader = new DGL::Shader(GAME_DEBUG_SHADER_VERTEX, GAME_DEBUG_SHADER_FRAGMENT);
        _debugShader->createShader();

        _shadowShader = new DGL::Shader("vertex_lightspace.glsl", "geometry_lightspace.glsl", "fragment_empty.glsl");
        _shadowShader->createShader();

        _mainPhysicsManager->createPhysics(glm::vec3(0.0f, -9.81f * 2.0f, 0.0f));
        _mainPhysicsManager->getScene()->setSimulationEventCallback(new DemonGame::DG_EXT_PhysicsCallback);

        _defaultWindow->clearWindow();
        _mainEvents->pollEvents();

        _mainOverlay = new DemonUI::DU_Overlay(_defaultWindow->getWindow(), _defaultWindow->getContext());
        _mainOverlay->newFrame();
        _mainEvents->callBack = std::bind(&DemonUI::DU_Overlay::updateEvents, _mainOverlay, std::placeholders::_1);
        _currentTicks = SDL_GetTicks();
    }

    int Engine::gameLoop(float deltaTime) {
        _currentTicks = SDL_GetTicks();


        //uint32_t startTicks = SDL_GetTicks();
        if (deltaTime <= 0.0f) {
            _mainPlayer->updateCamera();
            DGL::Shader::remakeGlobals();

            _enabledCamera->setMatrix();
            _world->updateAll();
        } else {

            _defaultWindow->clearWindow();

            _mainEvents->pollEvents();
            DemonBench::Benchmark("Shadows", [this]() {
                DGL::Mesh::_enableOcculusion = 0;
                for (auto light: _lightEntities) {
                    light->renderShadowBuffer(_shadowShader, [this]() {
                        _world->updateAll(_shadowShader, glm::mat4(1.0f), glm::mat4(1.0f), DGL::MeshRenderer::MESH_FLAGS::MESH_RENDER_SHADOW);
                    });
                }
            });

            glViewport(0, 0, _defaultWindow->getWidth(), _defaultWindow->getHeight());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            _debugShader->useShader();
            //glActiveTexture(GL_TEXTURE2);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexture);
            for (uint s = 0; s < 20; s++){
                _debugShader->uniformInt(fmt::format("_shadowMap[{}]", s), 2+s);
            }
            _defaultCamera->setMatrix();
            DGL::Mesh::_enableOcculusion = 1;

            //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            DemonBench::Benchmark("Final Render", [this](){
                _world->updateAll(_debugShader, glm::mat4(0.0f), glm::mat4(0.0f), DGL::MeshRenderer::MESH_FLAGS::MESH_RENDERED);
                midRenderFunc();
                _mainOverlay->render();
            });



            _mainOverlay->newFrame();
            getCameraController()->updateCamera();

            DemonBench::Benchmark("Physics", [this, deltaTime](){_mainPhysicsManager->simulate(deltaTime);});
            DemonBench::Benchmark("Flip Window", [this]() {_defaultWindow->flipWindow();});
        }


        for (auto client : _clients){
            client->loop();
        }

        return _mainEvents->getCloseState() || _mainEvents->getKeyDown(SDL_SCANCODE_ESCAPE);
    }


    void Engine::destroyEngine() {
        for (auto client : _clients){
            client->destroy();
        }

        delete _mainPlayer;

        _mainPhysicsManager->closePhysics();
        delete _mainPhysicsManager;

        delete _mainEvents;

        delete _defaultCamera;


        delete _defaultWindow;

        delete _world;
    }

    DemonGame::DG_RigidEntity *Engine::createWorldObject(DGL::Shader *targetShader) {
        DemonGame::DG_RigidEntity *goodEnt = new DemonGame::DG_RigidEntity([targetShader, this]() -> DGL::Shader*{
            if (!targetShader) { return _debugShader; }
            return targetShader;
            }(),
                                                                               _mainPhysicsManager);
        goodEnt->pointer = _world->addWorldObject(goodEnt);
        return goodEnt;
    }

    DemonGame::DG_PhysicsObject *Engine::createWorldEntity(DGL::Shader *targetShader) {
        DemonGame::DG_PhysicsObject *goodEnt = new DemonGame::DG_PhysicsObject([targetShader, this]() -> DGL::Shader*{
                                                                                   if (!targetShader) { return _debugShader; }
                                                                                   return targetShader;
                                                                               }(),
                                                                               _mainPhysicsManager);
        goodEnt->pointer = _world->addWorldEntity(goodEnt);
        return goodEnt;
    }

    DemonGame::DG_Entity *Engine::createVisualEntity(DGL::Shader *targetShader){
        DemonGame::DG_Entity *goodEnt = new DemonGame::DG_Entity([targetShader, this]() -> DGL::Shader*{
            if (!targetShader) { return _debugShader; }
            return targetShader;
        }());
        goodEnt->pointer = _world->addWorldGeneric(goodEnt);
        return goodEnt;
    }

    DemonGame::DG_AnimatedEntity *Engine::createAnimatedEntity(DGL::Shader *targetShader){
        DemonGame::DG_AnimatedEntity *goodEnt = new DemonGame::DG_AnimatedEntity([targetShader, this]() -> DGL::Shader*{
            if (!targetShader) { return _debugShader; }
            return targetShader;
        }());
        goodEnt->pointer = _world->addWorldGeneric(goodEnt);
        return goodEnt;
    }

    /*
    DemonGame::DG_BSPMap *Engine::createStaticWorld(){
        DemonGame::DG_BSPMap *goodEnt = new DemonGame::DG_BSPMap(_renderingManager, _shaderObject,
                                                                           _mainPhysicsManager);
        goodEnt->pointer = _world->addWorldObject((DemonGame::DG_RigidEntity*) goodEnt);
        return goodEnt;
    }
     */

    DemonGame::DG_Trigger *Engine::createTrigger(DGL::Shader *targetShader){
        DemonGame::DG_Trigger *goodEnt = new DemonGame::DG_Trigger([targetShader, this]() -> DGL::Shader*{
                                                                       if (!targetShader) { return _debugShader; }
                                                                       return targetShader;
                                                                   }(),
                                                                    _mainPhysicsManager);
        goodEnt->pointer = _world->addWorldGeneric(goodEnt);
        return goodEnt;
    }

    DGL::Light* Engine::createPointLight(POINT_LIGHT_INFO info){

        DGL::Light *newLight = new DGL::Light();
        newLight->setPosition(info.position);
        newLight->setDistance(info.distance);
        newLight->setAmbient(info.ambientStrength);
        newLight->setSpecularStrength(info.specularStrength);
        newLight->setSpecularValue(info.specularAccuracy);
        newLight->setColor(info.colour);
        _lightEntities.push_back(newLight);
        return newLight;
    }
    DGL::Light* Engine::createSpotLight(SPOT_LIGHT_INFO info){
        DGL::Light *newLight = new DGL::Light();
        newLight->setPosition(info.position);
        newLight->setDirection(info.direction);
        newLight->setDistance(info.distance);
        newLight->setAmbient(info.ambientStrength);
        newLight->setSpecularStrength(info.specularStrength);
        newLight->setSpecularValue(info.specularAccuracy);
        newLight->setCutoff(info.cutOffDegree);
        newLight->setOuterCutoff(info.outerCutOffDegree);
        newLight->setColor(info.colour);
        return newLight;
    }
    DGL::Light* Engine::createDirectionalLight(DIRECTIONAL_LIGHT_INFO info){
        DGL::Light *newLight = new DGL::Light();
        newLight->setDirection(info.direction);
        newLight->setAmbient(info.ambientStrength);
        newLight->setSpecularStrength(info.specularStrength);
        newLight->setSpecularValue(info.specularAccuracy);
        newLight->setColor(info.colour);
        return newLight;
    }

    DGL::Light* Engine::createEasyPointLight(glm::vec3 position, float distance, float strength, glm::vec3 colour){
        POINT_LIGHT_INFO hardInfo; // :flushed:
        hardInfo.position = position;
        hardInfo.distance = distance;
        hardInfo.colour = colour;
        hardInfo.specularAccuracy = 32;
        hardInfo.specularStrength = strength;
        hardInfo.ambientStrength = strength / 2.0f;
        return createPointLight(hardInfo); // Create a HARD point light :flushed:
    }
    DGL::Light* Engine::createEasySpotLight(glm::vec3 position, glm::vec3 direction, float angle, float distance, float strength, glm::vec3 colour){
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
    DGL::Light* Engine::createEasyDirectionalLight(glm::vec3 direction, float strength, glm::vec3 colour){
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

    DemonPhysics::DP_CharacterController* Engine::createCharacterController(glm::vec3 position, float height, float radius,
                                                                    float stepOffset, float scaleCoeff,
                                                                    float volumeGrowth, float density,
                                                                    float slopeLimit,
                                                                    DemonPhysics::DP_PhysicsMaterial *material){
        DemonPhysics::DP_PhysicsMaterial *mat = material;
        if (!mat){
            mat = new DemonPhysics::DP_PhysicsMaterial;
            mat->createMaterial(getPhysicsManager()->getPhysics());
        }
        DemonPhysics::DP_CharacterController *controller = new DemonPhysics::DP_CharacterController(position,
                                                                                                    mat->getMaterial(),
                                                                                                    height, radius,
                                                                                                    getPhysicsManager()->getControllerManager(),
                                                                                                    stepOffset, scaleCoeff,
                                                                                                    volumeGrowth, density, slopeLimit);
        return controller;
    }

    int  Engine::getGameState(std::string name){
        auto state = _gameState.find(name);
        if (state != _gameState.end()){
            return *state->second;
        }
        return 0;
    }
    int* Engine::getGameStatePTR(std::string name){
        auto state = _gameState.find(name);
        if (state != _gameState.end()){
            return state->second;
        }
        return nullptr;
    }
    void Engine::setGameState(std::string name, int value){
        auto state = _gameState.find(name);
        if (state != _gameState.end()){
            *_gameState.at(name) = value;
        } else{
            _gameState.insert(std::pair<std::string, int*>(name, new int(value)));
        }
    }
    void Engine::setGameStatePTR(std::string name, int *ptr){
        auto state = _gameState.find(name);
        if (state != _gameState.end()){
            _gameState.at(name) = ptr;
        } else{
            _gameState.insert(std::pair<std::string, int*>(name, ptr));
        }
    }

    void Engine::addClient(GameClient *client){
        _clients.push_back(client);
    }

    void Engine::destroyScene(){
        _world->destroyWorld();
        _world->clearAll();
        for (auto client : _clients){
            client->destroy();
            //delete client;
        }
        _clients.clear();
        for (auto light : _lightEntities){
            light->destroyLight();
            delete light;
        }
        _lightEntities.clear();
        DGL::Light::resetAllLights();

        _mainPhysicsManager->getControllerManager()->purgeControllers();

        _gameState = std::map<std::string, int*>();
    }
} // DemonEngine