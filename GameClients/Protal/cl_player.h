//
// Created by simon on 2/14/23.
//

#ifndef DEMONENGINE_CL_PLAYER_H
#define DEMONENGINE_CL_PLAYER_H

#include <DemonGame/Master/Engine.h>
#include <GameClients/GameClient.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <DemonGL/Light.h>

#define PL_GRAVITY -9.81f
#define DUCK_MULTIPLIER 0.3f
#define FLASHLIGHT_VAR "pl_flashlight"
#define NOCLIP_VAR "noclip"

namespace Protal {

    class cl_player : public GameClient {
    public:
        cl_player(glm::vec3 startPosition, float height, float radius, DemonEngine::Engine *engine);

        virtual ~cl_player() = default;

        void init();

        void loop();
        void destroy();

        DemonPhysics::DP_CharacterController *getController();
    private:
        DemonPhysics::DP_CharacterController *_controller;
        DGL::Light *_personalLight;
        float speed = 1.0f;

        glm::vec3 _startPosition;
        float _height;
        float _radius;
        DemonEngine::Engine *_engine;

        int _keyCallback = 0;
        int _keyDownCallback = 0;

        DemonGame::DG_PhysicsObject *_heldObject = nullptr;
        float gravity = PL_GRAVITY;
        float pl_height = 0.0f;

        int pl_flashlight = 0;
        int pl_noclip = 0;

    };

} // Protal

#endif //DEMONENGINE_CL_PLAYER_H
