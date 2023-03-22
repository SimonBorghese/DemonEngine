//
// Created by simon on 3/22/23.
//

#ifndef DEMONENGINE_WEAPON_ZAPPER_H
#define DEMONENGINE_WEAPON_ZAPPER_H

#include <GameClients/GameClient.h>
#include <DemonGame/Master/Engine.h>

#ifndef GET_SECONDS
#define GET_SECONDS() (SDL_GetTicks() / 1000.0f)
#endif

namespace Protal {

    class weapon_zapper : public GameClient {
    public:
        weapon_zapper(DemonEngine::Engine *engine, DemonPhysics::DP_CharacterController *attachedController);

        virtual ~weapon_zapper() = default;


        void init();

        void loop();

        void destroy();

    private:
        DemonEngine::Engine *_engine;
        DemonPhysics::DP_CharacterController *_controller;
        DemonGame::DG_AnimatedEntity *_zapper;
        int _firing = 0;
        int _callbackId = 0;

    };

} // Protal

#endif //DEMONENGINE_WEAPON_ZAPPER_H
