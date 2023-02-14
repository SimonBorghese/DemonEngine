//
// Created by simon on 2/13/23.
//

#include "npc_173.h"

namespace Protal {
    npc_173::npc_173(const char *model, glm::vec3 startPosition, DNPC::Level *level, DemonEngine::Engine *gameEngine) : _level(level), _engine(gameEngine){
        _entity = gameEngine->createWorldObject();
        _entity->createEntityFromMesh(model, startPosition, glm::vec3(0.0f), glm::vec3(0.4));

    }

    void npc_173::init(){
        _path = _level->getPath(_engine->getCamera()->getPosition(), _entity->getActor()->getPosition());
        _previousPosition = _engine->getCamera()->getPosition();
    }
    void npc_173::loop(){
        glm::vec3 playerP = glm::vec3(_engine->getCamera()->getPosition());
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

        glm::vec3 viewForward = playerP - _entity->getTransform()->getPosition();
        viewForward = glm::normalize(viewForward);
        //float dot = glm::dot(viewForward, engine->getCamera()->getCameraFront());
        //printf("Dot product: %f\n", dot);

        glm::vec3 moveTarget = glm::normalize(_path->getNextTarget() - _entity->getTransform()->getPosition()) *
                               (float) _engine->getDeltaTime() * 30.0f;
        moveTarget.y = 0.0f;
        if (!_entity->getMeshRenderer()->getRenderStatus()) {
            float roty = glm::atan((playerP.z - origin.z) / (playerP.x - origin.x));

            if ((playerP.x - origin.x) > 0){
                roty = roty - glm::radians(180.0f);
            }
            roty = (-roty - glm::radians(90.0f));
            _entity->getActor()->setRotation(glm::quat(glm::vec3(0.0f,roty,0.0f)));

            if (glm::distance(_entity->getTransform()->getPosition(), _path->getNextTarget()) > 1.0f) {
                _entity->getActor()->translate(moveTarget);
            } else {
                _path->advanceTarget();
            }
        }

    }
    void npc_173::destroy(){

    }
} // Protal