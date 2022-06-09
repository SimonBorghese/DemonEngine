//
// Created by simon on 6/9/22.
//

#ifndef DEMONENGINE_B_RIGIDACTOR_H
#define DEMONENGINE_B_RIGIDACTOR_H
#include <PhysX/PxRigidActor.h>


namespace DemonBase {

    class b_RigidActor {
    public:
        physx::PxRigidActor* getActor() { return _mainActor; }
        physx::PxShape* getShape() { return _mainShape; }
    protected:
        physx::PxRigidActor *_mainActor;
        physx::PxShape *_mainShape;
    };

} // DemonBase

#endif //DEMONENGINE_B_RIGIDACTOR_H
