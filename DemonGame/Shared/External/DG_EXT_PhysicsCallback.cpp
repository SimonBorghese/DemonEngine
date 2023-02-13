//
// Created by simon on 6/30/22.
//

#include "DG_EXT_PhysicsCallback.h"

namespace DemonGame {
    void DG_EXT_PhysicsCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
        for (unsigned int p = 0; p < nbPairs; p++) {
            const PxContactPair &cp = pairs[p];
            //printf("Pair names: %s %s\n", pairHeader.actors[0]->getName(), pairHeader.actors[1]->getName());


            if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                DemonBase::DemonUserData::generalStruct *actorOneDesc =
                        (DemonBase::DemonUserData::generalStruct *) pairHeader.actors[0]->userData;
                DemonBase::DemonUserData::generalStruct *actorTwoDesc =
                        (DemonBase::DemonUserData::generalStruct *) pairHeader.actors[1]->userData;
                // Ensure neither descriptor are null
                if (actorOneDesc != NULL && actorTwoDesc != NULL) {
                    // Check magic string
                    if (!strncmp(&actorOneDesc->magicString[0], "IOBJ", 4)) {
                        // If this object is static, return, no need to contact
                        if (actorOneDesc->type == STATIC) {
                            return;
                        }

                        if (actorOneDesc->type == DYNAMIC) {
                            // Get the real description
                            DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *physicsObjDesc =
                                    (DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *) actorOneDesc->structReference;
                            // Check for onContact function
                            if (physicsObjDesc->onContact != nullptr) {
                                // Create the DG_Object
                                DG_Object *reference = new DG_Object;
                                reference->type = actorTwoDesc->type;
                                switch (reference->type) {
                                    case STATIC:
                                        reference->rigidObj =
                                                ((DemonBase::DemonUserData::DP_RIGID_OBJ_DESC *)
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
                                delete reference;
                            }
                        } else {
                            DemonBase::DemonUserData::DP_CHARACTER_OBJ_DESC *characterObjDesc =
                                    (DemonBase::DemonUserData::DP_CHARACTER_OBJ_DESC *) actorOneDesc->structReference;

                            // if we have a contact function
                            if (characterObjDesc->characterCallback != nullptr) {
                                // Create the DG_Object
                                DG_Object *reference = new DG_Object;
                                reference->type = actorTwoDesc->type;
                                switch (reference->type) {
                                    case STATIC:
                                        reference->rigidObj =
                                                ((DemonBase::DemonUserData::DP_RIGID_OBJ_DESC *)
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
                                characterObjDesc->characterCallback(reference);
                                delete reference;
                            }
                        }
                    }

                    if (!strncmp(&actorTwoDesc->magicString[0], "IOBJ", 4)) {
                        // If this object is static, return, no need to contact
                        if (actorTwoDesc->type == STATIC) {
                            return;
                        }

                        if (actorTwoDesc->type == DYNAMIC) {
                            // Get the real description
                            DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *physicsObjDesc =
                                    (DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *) actorTwoDesc->structReference;
                            // Check for onContact function
                            if (physicsObjDesc->onContact != nullptr) {
                                // Create the DG_Object
                                DG_Object *reference = new DG_Object;
                                reference->type = actorOneDesc->type;
                                switch (reference->type) {
                                    case STATIC:
                                        reference->rigidObj =
                                                ((DemonBase::DemonUserData::DP_RIGID_OBJ_DESC *)
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
                                delete reference;
                            }
                        } else {
                            // Get the real description
                            DemonBase::DemonUserData::DP_CHARACTER_OBJ_DESC *characterObjDesc =
                                    (DemonBase::DemonUserData::DP_CHARACTER_OBJ_DESC *) actorTwoDesc->structReference;
                            // Check for onContact function
                            if (characterObjDesc->characterCallback != nullptr) {
                                // Create the DG_Object
                                DG_Object *reference = new DG_Object;
                                reference->type = actorOneDesc->type;
                                switch (reference->type) {
                                    case STATIC:
                                        reference->rigidObj =
                                                ((DemonBase::DemonUserData::DP_RIGID_OBJ_DESC *)
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
                                characterObjDesc->characterCallback(reference);
                                delete reference;
                            }
                        }

                    }

                }
            }
        }
    }

    void DG_EXT_PhysicsCallback::onTrigger(PxTriggerPair* pairs, PxU32 count){
        for (uint32_t i = 0; i < count; i++){
            if (pairs[i].otherActor){
                DemonBase::DemonUserData::generalStruct *triggerDesc =
                        (DemonBase::DemonUserData::generalStruct*) pairs[i].triggerActor->userData;
                DemonBase::DemonUserData::generalStruct *actorDesc =
                        (DemonBase::DemonUserData::generalStruct*) pairs[i].otherActor->userData;
                if (triggerDesc && !strcmp(triggerDesc->magicString, "IOBJ") && triggerDesc->type == DG_OBJ_TYPE::TRIGGER){
                    DemonBase::DemonUserData::DP_TRIGGER_OBJ_DESC *triggerObjDesc =
                            ((DemonBase::DemonUserData::DP_TRIGGER_OBJ_DESC *) triggerDesc->structReference);
                    if (!strcmp(triggerObjDesc->magicString, "IOBJ") && triggerObjDesc->triggerActivate){
                        if (pairs[i].otherActor->getName() && !strcmp(pairs[i].otherActor->getName(), "character")){
                            (triggerObjDesc->triggerActivate)(NULL, true);
                        }
                        else {
                            DG_Object *reference = new DG_Object;
                            reference->type = actorDesc->type;
                            switch (reference->type) {
                                case STATIC:
                                    reference->rigidObj =
                                            ((DemonBase::DemonUserData::DP_RIGID_OBJ_DESC *)
                                                    actorDesc->structReference)->reference;
                                    break;
                                case DYNAMIC:
                                    reference->physObj =
                                            ((DemonBase::DemonUserData::DP_PHYSICS_OBJ_DESC *)
                                                    actorDesc->structReference)->reference;
                                    break;
                                default:
                                    break;
                            }
                            // Call onContact
                            (triggerObjDesc->triggerActivate)(reference,
                                                            false);
                            delete reference;
                        }
                    }
                }
            }
        }
    }
} // DemonGame