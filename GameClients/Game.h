//
// Created by simon on 1/20/23.
//

#ifndef DEMONENGINE_GAME_H
#define DEMONENGINE_GAME_H
#include <DemonGame/Master/Engine.h>
#include <DemonGame/Master/BSPLoader.h>
#include <DemonGame/Master/World.h>


class Game{
public:
    virtual void init() = 0;
    virtual int  loop() = 0;
    virtual void close() = 0;
protected:
    DemonEngine::Engine *engine;
    DemonEngine::BSPLoader *bspLoader;
    DemonEngine::World *world;
};

#endif //DEMONENGINE_GAME_H
