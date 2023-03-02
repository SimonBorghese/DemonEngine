//
// Created by simon on 2/14/23.
//

#include "cl_player.h"

namespace Protal {

    cl_player::cl_player(glm::vec3 startPosition, float height, float radius, DemonEngine::Engine *engine) : _engine(engine){
        _controller = engine->createFPSController(startPosition, height, radius);
        _personalLight = engine->createEasyPointLight(startPosition, 50.0f, 1.0f);
    }

    void cl_player::init(){
        _controller->setName("Player");
        _engine->getEvent()->addKeyCallback([this](int scancode){
            if (!_engine->getGameState("noclip")) {
                switch (scancode) {
                    case SDL_SCANCODE_W:
                        _controller->move(_engine->getCamera()->getFPSFront() * speed);
                        break;
                    case SDL_SCANCODE_S:
                        _controller->move(-_engine->getCamera()->getFPSFront() * speed);
                        break;
                    case SDL_SCANCODE_A:
                        _controller->move(-_engine->getCamera()->getCameraRight() * speed);
                        break;
                    case SDL_SCANCODE_D:
                        _controller->move(_engine->getCamera()->getCameraRight() * speed);
                        break;
                    default:
                        break;

                }
            } else{
                switch (scancode) {
                    case SDL_SCANCODE_W:
                        _controller->translate(_engine->getCamera()->getPosition() + _engine->getCamera()->getCameraFront() * speed);
                        break;
                    case SDL_SCANCODE_S:
                        _controller->translate(_engine->getCamera()->getPosition() + -_engine->getCamera()->getCameraFront() * speed);
                        break;
                    case SDL_SCANCODE_A:
                        _controller->translate(_engine->getCamera()->getPosition() + -_engine->getCamera()->getCameraRight() * speed);
                        break;
                    case SDL_SCANCODE_D:
                        _controller->translate(_engine->getCamera()->getPosition() + _engine->getCamera()->getCameraRight() * speed);
                        break;
                    default:
                        break;

                }
            }
        });

        _engine->getEvent()->addKeyDownCallback([this](int scancode){
            switch (scancode){
                case SDL_SCANCODE_E:
                    auto projectile = _engine->createWorldEntity();
                    projectile->createEntityFromMesh("block.obj", _controller->getPosition() + (_engine->getCamera()->getCameraFront() * 5.0f));
                    projectile->setMass(1000.0f);
                    projectile->getActor()->applyForce(_engine->getCamera()->getCameraFront() * 5000.0f);
                    break;
            }
        });
    }
    void cl_player::loop(){
        _personalLight->setPosition(_engine->getCamera()->getPosition());
        if (!_engine->getGameState("noclip")) {
            speed = 1.0f;
            _controller->move(glm::vec3(0.0f, -9.81f, 0.0f));
        }
        else{
            speed = 5.0f;
        }

    }
    void cl_player::destroy(){

    }
    DemonPhysics::DP_CharacterController *cl_player::getController(){
        return _controller;
    }
} // Protal