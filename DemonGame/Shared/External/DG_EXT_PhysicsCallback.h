//
// Created by simon on 6/30/22.
//

#ifndef DEMONENGINE_DG_EXT_PHYSICSCALLBACK_H
#define DEMONENGINE_DG_EXT_PHYSICSCALLBACK_H
#include <PhysX/PxSimulationEventCallback.h>
#include "../DG_PhysicsObject.h"

using namespace physx;

namespace DemonGame {

    class DG_EXT_PhysicsCallback : public physx::PxSimulationEventCallback{
    public:
        virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
        virtual void onTrigger(PxTriggerPair*, PxU32) {}
        virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
        virtual void onWake(PxActor** , PxU32 ) {}
        virtual void onSleep(PxActor** , PxU32 ){}
        virtual void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}

    private:

    };

} // DemonGame

#endif //DEMONENGINE_DG_EXT_PHYSICSCALLBACK_H
