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
                DemonBase::DemonUserData::generalStruct *actorOneDesc =
                        (DemonBase::DemonUserData::generalStruct*) pairHeader.actors[0]->userData;
                DemonBase::DemonUserData::generalStruct *actorTwoDesc =
                        (DemonBase::DemonUserData::generalStruct*) pairHeader.actors[1]->userData;
                // Ensure neither descriptor are null
                if (actorOneDesc != NULL && actorTwoDesc != NULL){
                    // Check magic string
                    if (!strncmp(&actorOneDesc->magicString[0], "IOBJ", 4)) {
                        // If this object is static, return, no need to contact
                        if (actorOneDesc->type == STATIC){
                            return;
                        }
                        // Get the real description
                        DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *physicsObjDesc =
                                (DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC*) actorOneDesc->structReference;
                        // Check for onContact function
                        if (physicsObjDesc->onContact != nullptr) {
                            // Create the DG_Object
                            DG_Object *reference = new DG_Object;
                            reference->type = actorTwoDesc->type;
                            switch (reference->type){
                                case STATIC:
                                    reference->rigidObj =
                                            ((DemonBase::DemonUserData::DP_RIGID_OBJ_DESC*)
                                            actorTwoDesc->structReference)->reference;
                                    break;
                                case DYNAMIC:
                                    reference->physObj =
                                            ((DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *)
                                                    actorTwoDesc->structReference)->reference;
                                    break;
                                default:
                                    break;
                            }
                            // Call onContact
                            physicsObjDesc->onContact(physicsObjDesc->reference, reference);
                        }
                    }

                    if (!strncmp(&actorTwoDesc->magicString[0], "IOBJ", 4)) {
                        // If this object is static, return, no need to contact
                        if (actorTwoDesc->type == STATIC){
                            return;
                        }
                        // Get the real description
                        DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *physicsObjDesc =
                                (DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC*) actorTwoDesc->structReference;
                        // Check for onContact function
                        if (physicsObjDesc->onContact != nullptr) {
                            // Create the DG_Object
                            DG_Object *reference = new DG_Object;
                            reference->type = actorOneDesc->type;
                            switch (reference->type){
                                case STATIC:
                                    reference->rigidObj =
                                            ((DemonBase::DemonUserData::DP_RIGID_OBJ_DESC*)
                                                    actorOneDesc->structReference)->reference;
                                    break;
                                case DYNAMIC:
                                    reference->physObj =
                                            ((DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *)
                                                    actorOneDesc->structReference)->reference;
                                    break;
                                default:
                                    break;
                            }
                            // Call onContact
                            physicsObjDesc->onContact(physicsObjDesc->reference, reference);
                        }
                    }

                }

            }
        }
    }
} // DemonGame