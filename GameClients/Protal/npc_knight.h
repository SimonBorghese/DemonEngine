//
// Created by simon on 3/8/23.
//

#ifndef DEMONENGINE_NPC_KNIGHT_H
#define DEMONENGINE_NPC_KNIGHT_H
#include <GameClients/GameClient.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <DemonGame/Shared/DG_AnimatedEntity.h>
#include <DemonGame/Master/Engine.h>
#include <DemonNPC/Level.h>
#include <DemonNPC/Path.h>

#ifndef GET_SECONDS
#define GET_SECONDS() (SDL_GetTicks() / 1000.0f)
#endif


namespace Protal {

    class npc_knight : public GameClient{
    public:
        npc_knight(glm::vec3 startPosition, DNPC::Level *level, DemonEngine::Engine *gameEngine);

        void init();
        void loop();
        void destroy();
    private:
        DemonGame::DG_AnimatedEntity *_entity;
        DemonPhysics::DP_CharacterController *_controller;
        DNPC::Level *_level;
        DNPC::Path *_path;
        DemonEngine::Engine *_engine;
        glm::vec3 _previousPosition;
        int dead = 0;
    };

} // Protal

#endif //DEMONENGINE_NPC_KNIGHT_H
