//
// Created by simon on 3/22/23.
//

#include "weapon_zapper.h"

namespace Protal {
    weapon_zapper::weapon_zapper(DemonEngine::Engine *engine, DemonPhysics::DP_CharacterController *attachedController)
            : _engine(engine), _controller(attachedController) {
        _zapper = _engine->createAnimatedEntity();
        _zapper->createEntityFromMesh("zapper");

        _zapper->setPrerenderFunc([]() {
            //glDisable(GL_DEPTH_TEST);
            //glClear(GL_DEPTH_BUFFER_BIT);
            //glDepthFunc(GL_ALWAYS);
            //glDepthRange(0.0f, 0.1f);
        });
        _zapper->setPostrenderFunc([]() {
            //glEnable(GL_DEPTH_TEST);
            //glClear(GL_DEPTH_BUFFER_BIT);
            //glDepthFunc(GL_LESS);
        });

    }


    void weapon_zapper::init() {
        _zapper->setAnimation(4);
        _callbackId = _engine->getEvent()->addKeyDownCallback([this](int key) {
            switch (key) {
                case SDL_SCANCODE_E:
                    if (_zapper->isAnimationFinished(GET_SECONDS())) {
                        _firing = 1;
                        _zapper->playOnce(GET_SECONDS());
                    }
                    break;
            }
        });
    }

    void weapon_zapper::loop() {
        _zapper->playAnimation(GET_SECONDS());
        _zapper->getTransform()->setScale(glm::vec3(0.005f, 0.005f, -0.005f));

        _zapper->getTransform()->setPosition(_engine->getCamera()->getPosition() +
                                             (_engine->getCamera()->getCameraFront() * 2.0f) +
                                             (_engine->getCamera()->getCameraRight() * 2.0f) +
                                             glm::vec3(0.0f, -2.0f, 0.0f));

        _zapper->getTransform()->setRotation(glm::vec3(glm::radians(-_engine->getCamera()->getYRotation()),
                                                       glm::radians((-(_engine->getCamera()->getXRotation())) + 90.0f),
                                                       0.0f));


    }

    void weapon_zapper::destroy() {
        _engine->getEvent()->removeKeyDownCallback(_callbackId);
    }
} // Protal