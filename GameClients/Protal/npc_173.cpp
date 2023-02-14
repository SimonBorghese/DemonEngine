//
// Created by simon on 2/13/23.
//

#include "npc_173.h"

namespace Protal {
    npc_173::npc_173(const char *model, glm::vec3 startPosition, DNPC::Level *level, DemonEngine::Engine *gameEngine) : _level(level), _engine(gameEngine){
        _entity = gameEngine->createVisualEntity();
        _entity->createEntityFromMesh(model, startPosition, glm::vec3(0.0f), glm::vec3(0.4));
        _controller = gameEngine->createCharacterController(startPosition, 6.0f, 1.0f);

    }

    void npc_173::init(){
        _path = _level->getPath(_engine->getCamera()->getPosition(), _entity->getTransform()->getPosition());
        _previousPosition = _engine->getCamera()->getPosition();
    }
    void npc_173::loop(){
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
        if (!_entity->getMeshRenderer()->getRenderStatus()) {
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

        _controller->move(glm::vec3(0.0f, -9.81f, 0.0f));
    }
    void npc_173::destroy(){

    }
} // Protal