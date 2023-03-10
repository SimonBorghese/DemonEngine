//
// Created by simon on 2/14/23.
//

#include "cl_player.h"

namespace Protal {

    cl_player::cl_player(glm::vec3 startPosition, float height, float radius, DemonEngine::Engine *engine) :
    _startPosition(startPosition), _height(height), _radius(radius), _engine(engine){}

    void cl_player::init(){
        _controller = _engine->createFPSController(_startPosition, _height, _radius);
        _personalLight = _engine->createEasyPointLight(_startPosition, 50.0f, 1.0f);
        _controller->setName("character");
        _keyCallback = _engine->getEvent()->addKeyCallback([this](int scancode){
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

        _keyDownCallback = _engine->getEvent()->addKeyDownCallback([this](int scancode){
            switch (scancode){
                case SDL_SCANCODE_R:
                    auto projectile = _engine->createWorldEntity();
                    projectile->createEntityFromMesh("block", _controller->getPosition() +
                                                              (_engine->getCamera()->getCameraFront() * 5.0f));
                    projectile->setMass(10.0f);
                    projectile->getActor()->applyForce(_engine->getCamera()->getCameraFront() * 1000.0f);
                    break;
            }
        });
    }
    void cl_player::loop() {
        _personalLight->setPosition(_engine->getCamera()->getPosition());
        if (!_engine->getGameState("noclip")) {
            speed = 1.0f;
            _controller->move(glm::vec3(0.0f, -9.81f * 2.0f, 0.0f) * (float) _engine->getDeltaTime());
        } else {
            speed = 5.0f;
        }

        // pickup
        // Move currently held object
        if (_heldObject) {
            glm::vec3 targetPosition =
                    _engine->getCamera()->getPosition() + (_engine->getCamera()->getCameraFront() * 10.0f);
            glm::vec3 currentPositon = _heldObject->getActor()->getPosition();
            _heldObject->getActor()->setVelocity(
                    ((targetPosition - currentPositon) * (glm::distance(targetPosition, currentPositon) * 1.0f)));
            _heldObject->getActor()->setRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
        }
        if (_engine->getEvent()->getKeyDown(SDL_SCANCODE_E)) {
            if (!_heldObject) {
                auto rayCastCall = _engine->getPhysicsManager()->rayCastd(
                        _engine->getCamera()->getPosition() + (_engine->getCamera()->getCameraFront() * 2.0f),
                        _engine->getCamera()->getCameraFront(), 30.0f);
                if (rayCastCall.numberHits > 0) {
                    auto firstHit = rayCastCall.hits[0];
                    if (firstHit.object && firstHit.object->type == DemonGame::DYNAMIC) {
                        _heldObject = firstHit.object->physObj;
                    }
                }
            } else {
                if (_heldObject) { _heldObject->enableGravity(); }
                _heldObject = nullptr;
            }

        }

    }
    void cl_player::destroy(){
        //_controller->getController()->release();
        //_personalLight->destroyLight();
        _engine->getEvent()->removeKeyCallback(_keyCallback);
        _engine->getEvent()->removeKeyDownCallback(_keyDownCallback);

    }
    DemonPhysics::DP_CharacterController *cl_player::getController(){
        return _controller;
    }
} // Protal