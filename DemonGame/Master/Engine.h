//
// Created by simon on 6/10/22.
//

#ifndef DEMONENGINE_ENGINE_H
#define DEMONENGINE_ENGINE_H
/*
 * A master class for managing all elements of the engine
 */
#include <DemonGame/GAME_DEF.h>
#include <DemonRender/DR_RenderManager.h>
#include <DemonRender/DR_Shader.h>
#include <DemonPhysics/DP_PhysicsManager.h>
#include <DemonRender/DR_Camera.h>
#include <DemonGame/Shared/DG_Event.h>
#include <DemonGame/Shared/DG_BasicCameraController.h>
#include <DemonGame/Shared/DG_RigidEntity.h>
#include <DemonGame/Shared/DG_PhysicsObject.h>
#include "World.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <vector>

namespace DemonEngine {

    class Engine {
    public:
        // Create all the objects needed
        Engine(unsigned int width, unsigned int height, unsigned int fov = 70, unsigned int zFar = 100) : _width(width),
        _height(height), _fov(fov), _zFar(zFar) {
            _renderingManager = new DemonRender::DR_RenderManager;
            _shaderObject = new DemonRender::DR_Shader;
            _mainCamera = new DemonRender::DR_Camera(_shaderObject);
            _mainEvents = new DemonGame::DG_Event;

            _mainPhysicsManager = new DemonPhysics::DP_PhysicsManager;

            _mainPlayer = new DemonGame::DG_BasicCameraController(_mainEvents, _mainCamera, 0.5f);

            _world = new World;
        }

        // Engine state functions
        void createEngine();
        int gameLoop();
        void destroyEngine();

        // Some basic crap
        DemonGame::DG_RigidEntity* createWorldObject();
        DemonGame::DG_PhysicsObject* createWorldEntity();


        // All the delicious getter functions
        DemonRender::DR_RenderManager* getRenderingManager() { return _renderingManager; }
        DemonRender::DR_Shader* getObjectShader() { return _shaderObject; }
        DemonRender::DR_Camera* getCamera() { return _mainCamera; }
        DemonGame::DG_Event* getEvent() { return _mainEvents; }
        DemonPhysics::DP_PhysicsManager* getPhysicsManager() { return _mainPhysicsManager; }
        DemonGame::DG_BasicCameraController* getCameraController() { return _mainPlayer; }

        World* getWorld() { return _world; }
    private:
        // Some fundimental variables
        unsigned int _width, _height, _fov, _zFar;

        // The foundation objects
        DemonRender::DR_RenderManager *_renderingManager;
        DemonRender::DR_Shader *_shaderObject;
        DemonRender::DR_Camera *_mainCamera;
        DemonGame::DG_Event *_mainEvents;
        DemonPhysics::DP_PhysicsManager *_mainPhysicsManager;
        DemonGame::DG_BasicCameraController *_mainPlayer;

        // T h e   g a m e
        World *_world;

    };

} // DemonEngine

#endif //DEMONENGINE_ENGINE_H
