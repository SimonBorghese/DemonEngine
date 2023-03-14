//
// Created by simon on 3/14/23.
//

#ifndef DEMONENGINE_DOOR_BARS_H
#define DEMONENGINE_DOOR_BARS_H

#include <GameClients/GameClient.h>
#include <DemonGame/Master/Engine.h>
#include <DemonGame/Shared/DG_RigidEntity.h>

namespace Protal {

    class door_bars : public GameClient {
    public:
        door_bars(DemonEngine::Engine *engine, DG_RigidEntity *bars, float openHeight);

        void init();

        void loop();

        void destroy();

        void openDoor();

    private:
        DemonEngine::Engine *_engine;
        DG_RigidEntity *_bars;
        float _openHeight;
        int open = 0;
        glm::vec3 startPos = glm::vec3(0.0f);


    };

} // Protal

#endif //DEMONENGINE_DOOR_BARS_H
