//
// Created by simon on 1/29/23.
//

#include "npc_charger.h"

namespace Protal {
    npc_charger::npc_charger(DemonEngine::Engine *engine, glm::vec3 startingPosition, float *health) : _engine(engine), _health(health){
        _controller = _engine->createWorldEntity();
        _controller->createEntityFromMesh("spike", startingPosition);
        //_controller->setName("Charger");


        /*
        _controller->setContactCallback([this](DemonGame::DG_PhysicsObject *, DG_Object *obj){
            if (obj->type == DemonGame::DYNAMIC && strcmp(obj->physObj->getName().c_str(), "Charger")) {
                float velocity = obj->physObj->getActor()->getRealActor()->getLinearVelocity().magnitude();
                float mass = obj->physObj->getMass();
                printf("Got Force: %f\n", velocity * mass);
                enemyHealth -= (velocity * mass) * 10.0f;
                enemyHealth = fmax(enemyHealth, 0.0f);
            }
        });
         */


        _camera = _engine->getCamera();
    }

    void npc_charger::init() {}
    void npc_charger::loop(){
        glm::vec3 moveTarget = (-glm::normalize(_controller->getActor()->getPosition() - (_camera->getPosition()
                                                                              - glm::vec3(0.0f, 5.0f, 0.0f))));
        if (glm::distance(_controller->getActor()->getPosition(), _camera->getPosition()) > 5.0f &&
            !_engine->getEvent()->getKey(SDL_SCANCODE_P) &&
            enemyHealth > 0.0f) {
            _controller->getActor()->applyForce((moveTarget / (100.0f / fmax(enemyHealth, 0.01f))) * 15.0f);
        } else {
            *_health -= 20.0f * _engine->getDeltaTime();
        }
    }
    void npc_charger::destroy(){
        //_controller->destroyEntity();
    }
} // Protal