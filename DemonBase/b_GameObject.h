//
// Created by simon on 6/8/22.
//

#ifndef DEMONENGINE_B_GAMEOBJECT_H
#define DEMONENGINE_B_GAMEOBJECT_H
#include <DemonWorld/DW_Transform.h>


namespace DemonBase {
    class b_GameObject {
    public:
        virtual void update() = 0;

        DemonWorld::DW_Transform *getTransform() { return &mainTransform; }

    protected:
        DemonWorld::DW_Transform mainTransform;
    };
} // DemonBase

#endif //DEMONENGINE_B_GAMEOBJECT_H
