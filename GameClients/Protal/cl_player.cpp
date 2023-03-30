//
// Created by simon on 2/14/23.
//

#include "cl_player.h"

namespace Protal {

    cl_player::cl_player(glm::vec3 startPosition, float height, float radius, DemonEngine::Engine *engine) :
            _startPosition(startPosition), _height(height), _radius(radius), _engine(engine), pl_height(height) {}

    void cl_player::init(){
        _controller = _engine->createFPSController(_startPosition, _height, _radius);
        _personalLight = _engine->createEasyPointLight(_startPosition, 1000.0f, 0.5f);
        //_personalLight->createShadowBuffer(500,500);
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
            switch (scancode) {
                case SDL_SCANCODE_R: {
                    /*
                    auto projectile = _engine->createWorldEntity();
                    projectile->createEntityFromMesh("block", _controller->getPosition() +
                                                              (_engine->getCamera()->getCameraFront() * 8.0f),
                                                     glm::vec3(0.0f), glm::vec3(1.0f));
                    projectile->setMass(1.0f);
                    projectile->getActor()->applyForce(_engine->getCamera()->getCameraFront() * 2000.0f);
                     */
                    auto block = new prop_block(_engine, "block", _controller->getPosition() +
                                                                  (_engine->getCamera()->getCameraFront() * 8.0f),
                                                _engine->getCamera()->getCameraFront() * 20.0f);
                    _engine->addClient(block);
                }
                    break;
                case SDL_SCANCODE_SPACE: {
                    if (_controller->onGround()) {
                        gravity = (- PL_GRAVITY) * 0.3f;
                    }
                }
                    break;
            }
        });

        _engine->setGameStateCallback(FLASHLIGHT_VAR, [this](int newVal) {
            pl_flashlight = newVal;
        });
        _engine->setGameStateCallback(NOCLIP_VAR, [this](int newVal) {
            pl_noclip = newVal;
        });
    }
    void cl_player::loop() {
        if (!_engine->getEvent()->getKey(SDL_SCANCODE_C) && pl_flashlight) {
            _personalLight->setDistance(20.0f);
            _personalLight->setPosition(
                    _engine->getCamera()->getPosition() + (_engine->getCamera()->getCameraFront() * 2.0f));
            _personalLight->setDirection(_engine->getCamera()->getCameraFront());
        } else {
            _personalLight->setDistance(0.0f);
        }
        if (_engine->getEvent()->getKeyDown(SDL_SCANCODE_F)) {
            _engine->setGameState(FLASHLIGHT_VAR, !_engine->getGameState(FLASHLIGHT_VAR));

        }

        if (!pl_noclip) {
            speed = 1.0f;
            if (gravity != PL_GRAVITY) {
                gravity += PL_GRAVITY * _engine->getDeltaTime();
                gravity = glm::clamp(gravity, PL_GRAVITY, - PL_GRAVITY);
            }
            if (pl_height != _height && !_engine->getEvent()->getKey(SDL_SCANCODE_LCTRL)) {
                float addedHeight = _height * _engine->getDeltaTime() * (1 / DUCK_MULTIPLIER);
                pl_height += addedHeight;
                pl_height = glm::clamp(pl_height, _height * DUCK_MULTIPLIER, _height);
                _controller->move(glm::vec3(0.0f, addedHeight, 0.0f));
            }
            _controller->move(glm::vec3(0.0f, gravity * 10.0f, 0.0f) * (float) _engine->getDeltaTime());
            _controller->setHeight(pl_height);
        } else {
            speed = 5.0f;
        }

        if (_engine->getEvent()->getKey(SDL_SCANCODE_LCTRL)) {
            pl_height = _height * DUCK_MULTIPLIER;
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
                    auto rayHit = rayCastCall.hits[0];
                    if (rayHit.object && rayHit.object->isType("Prop")) {
                        auto prop = (prop_block *) rayHit.object;
                        _heldObject = prop->getPhysicsObject();
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