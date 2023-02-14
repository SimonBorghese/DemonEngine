//
// Created by simon on 2/13/23.
//

#ifndef DEMONENGINE_NPC_173_H
#define DEMONENGINE_NPC_173_H
#include <GameClients/GameClient.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <DemonGame/Shared/DG_Entity.h>
#include <DemonGame/Master/Engine.h>
#include <DemonNPC/Level.h>
#include <DemonNPC/Path.h>

namespace Protal {

    class npc_173 : public GameClient{
    public:
        npc_173(const char *model, glm::vec3 startPosition, DNPC::Level *level, DemonEngine::Engine *gameEngine);

        void init();
        void loop();
        void destroy();
    private:
        DG_Entity *_entity;
        DemonPhysics::DP_CharacterController *_controller;
        DNPC::Level *_level;
        DNPC::Path *_path;
        DemonEngine::Engine *_engine;
        glm::vec3 _previousPosition;


    };

} // Protal

#endif //DEMONENGINE_NPC_173_H
