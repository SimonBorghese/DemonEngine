//
// Created by simon on 2/14/23.
//

#ifndef DEMONENGINE_CL_PLAYER_H
#define DEMONENGINE_CL_PLAYER_H
#include <DemonGame/Master/Engine.h>
#include <GameClients/GameClient.h>
#include <DemonPhysics/DP_CharacterController.h>

namespace Protal {

    class cl_player : public GameClient{
    public:
        cl_player(glm::vec3 startPosition, float height, float radius, DemonEngine::Engine *engine);

        void init();
        void loop();
        void destroy();

        DemonPhysics::DP_CharacterController *getController();
    private:
        DemonPhysics::DP_CharacterController *_controller;
        DemonEngine::Engine *_engine;

    };

} // Protal

#endif //DEMONENGINE_CL_PLAYER_H
