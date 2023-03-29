//
// Created by simon on 6/30/22.
//

#include "DG_EXT_PhysicsCallback.h"

namespace DemonGame {
    void DG_EXT_PhysicsCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
        for (unsigned int p = 0; p < nbPairs; p++) {
            const PxContactPair &cp = pairs[p];

            if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                auto *clientOne =
                        (DemonGame::ClientInfo *) pairHeader.actors[0]->userData;
                auto *clientTwo =
                        (DemonGame::ClientInfo *) pairHeader.actors[1]->userData;

                GameClient *client = nullptr;
                GameClient *other = nullptr;
                if (clientOne && !strncmp(MAGIC_STRING, &clientOne->magicString[0], 4)) {
                    client = clientOne->client;
                }
                if (clientTwo && !strncmp(MAGIC_STRING, &clientTwo->magicString[0], 4)) {
                    other = clientTwo->client;
                }
                if (client) {
                    client->onContact(other);
                }
                if (other) {
                    other->onContact(client);
                }
            }
        }
    }

    void DG_EXT_PhysicsCallback::onTrigger(PxTriggerPair* pairs, PxU32 count){
        for (uint32_t i = 0; i < count; i++){
            if (pairs[i].otherActor) {
                auto *clientOne =
                        (DemonGame::ClientInfo *) pairs[i].triggerActor->userData;
                auto *clientTwo =
                        (DemonGame::ClientInfo *) pairs[i].otherActor->userData;

                GameClient *client;
                GameClient *other;
                if (clientOne && !strncmp(MAGIC_STRING, &clientOne->magicString[0], 4)) {
                    client = clientOne->client;
                }
                if (clientTwo && !strncmp(MAGIC_STRING, &clientTwo->magicString[0], 4)) {
                    other = clientTwo->client;
                }
                if (client) {
                    client->onTrigger(other);
                }

            }
        }
    }
} // DemonGame