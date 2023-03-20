//
// Created by simon on 6/10/22.
//

#ifndef DEMONENGINE_ENGINE_H
#define DEMONENGINE_ENGINE_H
/*
 * A master class for managing all elements of the engine
 */
#include <DemonGL/DemonGL.h>
#include <DemonGame/GAME_DEF.h>
#include <DemonGame/Shared/DG_AnimatedEntity.h>
#include <DemonPhysics/DP_PhysicsManager.h>
#include <DemonGame/Shared/DG_Event.h>
#include <DemonGame/Shared/DG_BasicCameraController.h>
#include <DemonGame/Shared/DG_RigidEntity.h>
#include <DemonGame/Shared/DG_PhysicsObject.h>
#include "World.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <vector>
#include <DemonGame/Shared/External/DG_EXT_PhysicsCallback.h>
#include <DemonGame/Shared/DG_Trigger.h>
#include <DemonUI/DU_Overlay.h>
#include <GameClients/GameClient.h>

namespace DemonEngine {

    class Engine {
    public:
        // Create all the objects needed
        Engine(unsigned int width, unsigned int height, unsigned int fov = 70, unsigned int zFar = 1000) : _width(
                width),
                                                                                                           _height(height),
                                                                                                           _fov(fov),
                                                                                                           _zFar(zFar) {
            _defaultWindow = new DGL::Window(GAME_NAME, width, height);
            _defaultCamera = new DGL::Camera();

            _mainEvents = new DemonGame::DG_Event;

            _mainPhysicsManager = new DemonPhysics::DP_PhysicsManager;

            _mainPlayer = new DemonGame::DG_BasicCameraController(_mainEvents, _defaultCamera, 0.5f);

            _world = new World(_mainPhysicsManager);

        }

        // Engine state functions
        void createEngine();

        int gameLoop(float deltaTime = 0.0f);

        void destroyEngine();

        // Some basic crap
        DemonGame::DG_RigidEntity *createWorldObject(DGL::Shader *targetShader = nullptr);

        DemonGame::DG_PhysicsObject *createWorldEntity(DGL::Shader *targetShader = nullptr);

        DemonGame::DG_Entity *createVisualEntity(DGL::Shader *targetShader = nullptr);

        DemonGame::DG_AnimatedEntity *createAnimatedEntity(DGL::Shader *targetShader = nullptr);

        DemonGame::DG_Trigger *createTrigger(DGL::Shader *targetShader = nullptr);

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


        DGL::Light* createPointLight(POINT_LIGHT_INFO info);
        DGL::Light* createSpotLight(SPOT_LIGHT_INFO info);
        DGL::Light* createDirectionalLight(DIRECTIONAL_LIGHT_INFO info);

        DGL::Light* createEasyPointLight(glm::vec3 position, float distance, float strength, glm::vec3 colour = glm::vec3(1.0f));
        DGL::Light* createEasySpotLight(glm::vec3 position, glm::vec3 direction, float angle, float distance, float strength, glm::vec3 colour = glm::vec3(1.0f));
        DGL::Light* createEasyDirectionalLight(glm::vec3 direction, float strength, glm::vec3 colour = glm::vec3(1.0f));



        DemonPhysics::DP_CharacterController *createFPSController(glm::vec3 startPos, float height, float radius);

        void setGravity(glm::vec3 newGravity) {
            _mainPhysicsManager->getScene()->setGravity(physx::PxVec3(newGravity.x,
                                                                      newGravity.y,
                                                                      newGravity.z));
        }
        glm::vec3 getGravity() { return glm::vec3(_mainPhysicsManager->getScene()->getGravity().x,
                                                  _mainPhysicsManager->getScene()->getGravity().y,
                                                  _mainPhysicsManager->getScene()->getGravity().z);}


        // All the delicious getter functions
        DGL::Camera *getCamera() { return _defaultCamera; }

        DGL::Window *getWindow() { return _defaultWindow; }

        DemonGame::DG_Event *getEvent() { return _mainEvents; }

        DemonPhysics::DP_PhysicsManager *getPhysicsManager() { return _mainPhysicsManager; }

        DemonGame::DG_BasicCameraController *getCameraController() { return _mainPlayer; }

        World *getWorld() { return _world; }

        void setCamera(DGL::Camera *newCam){
            if (!newCam){
                _enabledCamera = _defaultCamera;
                return;
            }
            _enabledCamera = newCam;

        }
        DGL::Camera *getDefaultCamera() { return _defaultCamera; }

        DGL::Shader *getDefaultShader() { return _debugShader; }

        double getDeltaTime() {
            return (double) ((SDL_GetTicks() - _currentTicks)) / 1000;
        }

        void updateDeltatime() { _currentTicks = SDL_GetTicks(); }

        DemonUI::DU_Overlay* getOverlay() { return _mainOverlay; }

        std::vector<DGL::Light*>* getLights() { return &_lightEntities; }

        DemonPhysics::DP_CharacterController* createCharacterController(glm::vec3 position, float height, float radius,
                                                                        float stepOffset = 2.0f, float scaleCoeff = 1.0f,
                                                                        float volumeGrowth = 1.0f, float density = 2.0f,
                                                                        float slopeLimit = 50.0f,
                                                                        DemonPhysics::DP_PhysicsMaterial *material = nullptr);

        std::function<void()> midRenderFunc = []() -> void {};

        int getGameState(std::string name);

        int *getGameStatePTR(std::string name);

        void setGameState(std::string name, int value);

        void setGameStatePTR(std::string name, int *ptr);

        void addClient(GameClient *client);

        void addClient(std::string name, GameClient *client);

        GameClient *getClient(std::string name);

        void destroyScene();

    private:
        // Some fundimental variables
        unsigned int _width, _height, _fov, _zFar;

        // The foundation objects
        DGL::Window *_defaultWindow;
        DGL::Camera *_defaultCamera;
        DGL::Camera *_enabledCamera;
        DGL::Shader *_debugShader;
        DemonGame::DG_Event *_mainEvents;
        DemonPhysics::DP_PhysicsManager *_mainPhysicsManager;
        DemonGame::DG_BasicCameraController *_mainPlayer;

        DemonUI::DU_Overlay *_mainOverlay;

        std::vector<DGL::Light*> _lightEntities;
        DGL::Shader *_shadowShader;
        DGL::Shader *_shadowShaderSpotLight;

        // T h e   g a m e
        World *_world;

        // delta time stuff
        unsigned int _currentTicks;

        // Game state variables
        std::map<std::string, int *> _gameState;

        //std::vector<GameClient*> _clients;
        std::map<std::string, GameClient *> _clients;


    };

} // DemonEngine

#endif //DEMONENGINE_ENGINE_H
