//
// Created by simon on 3/14/23.
//

#include "door_bars.h"

namespace Protal {
    door_bars::door_bars(DemonEngine::Engine *engine, DemonGame::DG_RigidEntity *bars, float openHeight) : _engine(
            engine),
                                                                                                           _bars(bars),
                                                                                                           _openHeight(
                                                                                                                   openHeight) {}

    void door_bars::init() {

    }

    void door_bars::loop() {
        if (open) {
            _bars->getActor()->translate(
                    (startPos - _bars->getActor()->getPosition()) * (float) _engine->getDeltaTime());
        }
        if (_bars->getActor()->getPosition() == startPos) {
            open = 0;
        }

    }

    void door_bars::destroy() {

    }

    void door_bars::openDoor() {
        open = !open;
        startPos = _bars->getActor()->getPosition() + glm::vec3(0.0f, -_openHeight * ((-2.0f * open) + 1.0f), 0.0f);
    }
} // Protal