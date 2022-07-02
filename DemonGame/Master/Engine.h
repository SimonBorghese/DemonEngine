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
#include <DemonRender/DemonLights/DR_DL_BasicLight.h>
#include "World.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <vector>

namespace DemonEngine {

    class Engine {
    public:
        // Create all the objects needed
        Engine(unsigned int width, unsigned int height, unsigned int fov = 70, unsigned int zFar = 1000) : _width(
                width),
                                                                                                           _height(height),
                                                                                                           _fov(fov),
                                                                                                           _zFar(zFar) {
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
        DemonGame::DG_RigidEntity *createWorldObject();

        DemonGame::DG_PhysicsObject *createWorldEntity();

        // Lighting objects
        typedef struct{
            glm::vec3 position;
            float distance;
            float ambientStrength;
            float specularStrength;
            unsigned int specularAccuracy;
            glm::vec3 colour;
        } POINT_LIGHT_INFO;

        typedef struct{
            glm::vec3 position;
            glm::vec3 direction;
            float distance;
            float ambientStrength;
            float specularStrength;
            unsigned int specularAccuracy;
            float cutOffDegree;
            float outerCutOffDegree;
            glm::vec3 colour;
        } SPOT_LIGHT_INFO;

        typedef struct{
            glm::vec3 direction;
            float ambientStrength;
            float specularStrength;
            unsigned int specularAccuracy;
            glm::vec3 colour;
        } DIRECTIONAL_LIGHT_INFO;


        DemonRender::DemonLight::DR_DL_BasicLight* createPointLight(POINT_LIGHT_INFO info);
        DemonRender::DemonLight::DR_DL_BasicLight* createSpotLight(SPOT_LIGHT_INFO info);
        DemonRender::DemonLight::DR_DL_BasicLight* createDirectionalLight(DIRECTIONAL_LIGHT_INFO info);

        DemonRender::DemonLight::DR_DL_BasicLight* createEasyPointLight(glm::vec3 position, float distance, float strength, glm::vec3 colour = glm::vec3(1.0f));
        DemonRender::DemonLight::DR_DL_BasicLight* createEasySpotLight(glm::vec3 position, glm::vec3 direction, float angle, float distance, float strength, glm::vec3 colour = glm::vec3(1.0f));
        DemonRender::DemonLight::DR_DL_BasicLight* createEasyDirectionalLight(glm::vec3 direction, float strength, glm::vec3 colour = glm::vec3(1.0f));



        DemonPhysics::DP_CharacterController *createFPSController(glm::vec3 startPos, float height, float radius);


        // All the delicious getter functions
        DemonRender::DR_RenderManager *getRenderingManager() { return _renderingManager; }

        DemonRender::DR_Shader *getObjectShader() { return _shaderObject; }

        DemonRender::DR_Camera *getCamera() { return _mainCamera; }

        DemonGame::DG_Event *getEvent() { return _mainEvents; }

        DemonPhysics::DP_PhysicsManager *getPhysicsManager() { return _mainPhysicsManager; }

        DemonGame::DG_BasicCameraController *getCameraController() { return _mainPlayer; }

        World *getWorld() { return _world; }

        double getDeltaTime() {
            return (double) ((SDL_GetTicks() - _currentTicks)) / 1000;
        }

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

        // delta time stuff
        unsigned int _currentTicks;

    };

} // DemonEngine

#endif //DEMONENGINE_ENGINE_H
