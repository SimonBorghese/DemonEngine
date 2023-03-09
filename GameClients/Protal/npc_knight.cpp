//
// Created by simon on 3/8/23.
//

#include "npc_knight.h"

namespace Protal {
    npc_knight::npc_knight(glm::vec3 startPosition, DNPC::Level *level, DemonEngine::Engine *gameEngine) : _level(level), _engine(gameEngine){
        _entity = gameEngine->createAnimatedEntity();
        _entity->createEntityFromMesh("knight", startPosition, glm::vec3(0.0f), glm::vec3(1.0));
        _entity->getTransform()->setScale(glm::vec3(0.005));
        _entity->setAnimation(5);
        _entity->setUpdateFunc([this](DG_Entity *aChicken){
            DG_AnimatedEntity *realChicken = (DG_AnimatedEntity*) aChicken;
            if (!dead || (dead && !realChicken->isAnimationFinished(GET_SECONDS()))) {
                realChicken->playAnimation(GET_SECONDS());
            }
        });
        _entity->getMeshRenderer()->addFlag(DGL::MeshRenderer::MESH_FLAGS::MESH_RENDER_SHADOW);
        _controller = gameEngine->createCharacterController(startPosition, 3.0f, 2.0f);
        //_entity->getMeshRenderer()->removeFlag(DGL::MeshRenderer::MESH_FLAGS::MESH_RENDER_SHADOW);

    }

    void npc_knight::init(){
        //_path = _level->getPath(_engine->getCamera()->getPosition(), _entity->getTransform()->getPosition());
        //_previousPosition = _engine->getCamera()->getPosition();
    }
    void npc_knight::loop(){
        /*
        _entity->getTransform()->setPosition(_controller->getPosition() - glm::vec3(0.0f, _controller->getHeight() - 1.0f,  0.0f));
        glm::vec3 playerP = glm::vec3(_engine->getCamera()->getPosition()) - glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 origin = _entity->getTransform()->getPosition() + glm::vec3(0.0f, 2.0f, 0.0f);
        //printf("Follower is rendered: %d\n", follower->getMeshRenderer()->getRenderStatus());b

        if (playerP != _previousPosition){
            _previousPosition = playerP;
            glm::vec3 position = playerP;
            glm::vec3 followerPos = origin;

            uint32_t start = SDL_GetTicks();
            _path = _level->getPath(position, followerPos);
            printf("Benchmark time: %d\n", SDL_GetTicks() - start);
            assert(_path);
        }
        _path->setStartPosition(_entity->getTransform()->getPosition());

        glm::vec3 moveTarget = glm::normalize(_path->getNextTarget() - _entity->getTransform()->getPosition()) *
                               (float) 0.016f * 30.0f;
        //moveTarget.y = 0.0f;
        if (!_entity->getMeshRenderer()->compareFlag(DGL::MeshRenderer::MESH_IN_VIEW)) {
            float roty = glm::atan((playerP.z - origin.z) / (playerP.x - origin.x));

            if ((playerP.x - origin.x) > 0){
                roty = roty - glm::radians(180.0f);
            }
            roty = (-roty - glm::radians(90.0f));
            _entity->getTransform()->setRotation(glm::quat(glm::vec3(0.0f,roty,0.0f)));

            glm::vec3 entityPos = _entity->getTransform()->getPosition();
            glm::vec3 pathTarget = _path->getNextTarget();
            entityPos.y = 0.0f;
            pathTarget.y =0.0f;

            if (glm::distance(entityPos, pathTarget) > 2.0f) {
                //_entity->getTransform()->translate(moveTarget);
                _controller->move(moveTarget);
            } else {
                _path->advanceTarget();
            }
        }

         */

        _entity->getTransform()->setPosition(_controller->getPosition() - glm::vec3(0.0f, 0.0f, 0.0f));


        glm::vec3 playerP = _engine->getCamera()->getPosition();
        glm::vec3 origin = _entity->getTransform()->getPosition();
        if (!dead) {
            if (glm::distance(playerP, origin) >= 10) {
                if (_entity->getAnimationIndex() != 5 && _entity->isAnimationFinished(GET_SECONDS())) {
                    _entity->setAnimation(5);
                }
                if (_entity->getAnimationIndex() == 5) {
                    glm::vec3 moveTarget = glm::normalize(playerP - origin) *
                                           (float) 0.016f * 5.0f;
                    _controller->move(moveTarget);

                    float roty = glm::atan((playerP.z - origin.z) / (playerP.x - origin.x));

                    if ((playerP.x - origin.x) > 0) {
                        roty = roty - glm::radians(180.0f);
                    }
                    roty = (-roty - glm::radians(90.0f));
                    _entity->getTransform()->setRotation(glm::vec3(0.0f, roty, 0.0f));
                }
            } else {
                _entity->setAnimation(0);
            }
        } else{
            if (_entity->getAnimationIndex() != 2){
                _entity->setAnimation(2);
                _entity->playOnce(GET_SECONDS());
            }
        }

        if (!dead) {
            if (_entity->isAnimationFinished(GET_SECONDS())) {
                _entity->playOnce(GET_SECONDS());
            }
        }
        if (_engine->getEvent()->getKeyDown(SDL_SCANCODE_9)){
            dead=1;
        }
        _controller->move(glm::vec3(0.0f, -9.81f, 0.0f));
    }
    void npc_knight::destroy(){

    }
} // Protal