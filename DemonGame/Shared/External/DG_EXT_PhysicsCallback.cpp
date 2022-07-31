//
// Created by simon on 6/30/22.
//

#include "DG_EXT_PhysicsCallback.h"

namespace DemonGame {
    void DG_EXT_PhysicsCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs){
        for (unsigned int p = 0; p < nbPairs; p++){
            const PxContactPair& cp = pairs[p];

            if(cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
            {
                DP_PHYSICS_OBJ_DESC *actorOneDesc = (DP_PHYSICS_OBJ_DESC*) pairHeader.actors[0]->userData;
                DP_PHYSICS_OBJ_DESC *actorTwoDesc = (DP_PHYSICS_OBJ_DESC*) pairHeader.actors[1]->userData;
                if (actorOneDesc != NULL && actorTwoDesc != NULL){
                    if (!strncmp(&actorOneDesc->magicString[0], "IOBJ", 4)){
                        if (actorOneDesc->onContact != nullptr){
                            actorOneDesc->onContact(actorOneDesc->reference, actorTwoDesc->reference);
                        }

                        if (actorTwoDesc->onContact != nullptr){
                            actorTwoDesc->onContact(actorTwoDesc->reference, actorOneDesc->reference);
                        }

                    }
                }

            }
        }
    }
} // DemonGame