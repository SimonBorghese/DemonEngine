//
// Created by simon on 1/29/23.
//

#ifndef DEMONENGINE_NPC_CHARGER_H
#define DEMONENGINE_NPC_CHARGER_H
#include <GameClients/GameClient.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <DemonGame/Shared/DG_Entity.h>
#include <DemonGame/Master/Engine.h>
#include "prop_block.h"

namespace Protal {

    class npc_charger : public GameClient {
    public:
        npc_charger(DemonEngine::Engine *engine, glm::vec3 startingPosition, float *health);

        void init();

        void loop();

        void destroy();

        float getHealth() { return enemyHealth; }

        void onContact(GameClient *client);

    private:
        DemonGame::DG_PhysicsObject *_controller;
        DGL::Camera *_camera;
        DemonEngine::Engine *_engine;
        float *_health;
        float enemyHealth = 100.0f;
        int active = 1;



    };

} // Protal

#endif //DEMONENGINE_NPC_CHARGER_H
