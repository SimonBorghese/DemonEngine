//
// Created by simon on 6/13/22.
//

#ifndef DEMONENGINE_B_CONTROLLER_H
#define DEMONENGINE_B_CONTROLLER_H

#include <PxPhysicsAPI.h>
#include <characterkinematic/PxController.h>
#include <DemonBase/b_PhysUserData.h>

// Needed for time calculation, bad to include but I need it
#include <SDL2/SDL.h>

#include <glm/glm.hpp>

namespace DemonBase {
    class b_Controller {
    public:

        void getPosition(glm::vec3 *target) {
            if (target == NULL) {
                return;
            }
            *target = glm::vec3(pMainController->getPosition().x,
                                pMainController->getPosition().y,
                                pMainController->getPosition().z);
        }
        glm::vec3 getPosition(){
            return glm::vec3(pMainController->getPosition().x,
                             pMainController->getPosition().y,
                             pMainController->getPosition().z);
        }

        void setController(physx::PxController *newController) { pMainController = newController; }

        void move(glm::vec3 forward) {
            if (!pLastTime) { pLastTime = SDL_GetTicks(); }

            pMainController->move(physx::PxVec3(forward.x, forward.y, forward.z), 0.0f, (SDL_GetTicks() - pLastTime),
                                  pControlFilter);
            if (realTarget != glm::vec3(0.0f)) {
                forceTarget = glm::normalize(realTarget);
                pMainController->move(physx::PxVec3(forceTarget.x, forceTarget.y * 33.0f, forceTarget.z), 0.1f,
                                      (SDL_GetTicks() - pLastTime), pControlFilter);
                realTarget -= glm::normalize(realTarget);
                //glm::clamp(realTarget, glm::vec3(0.0f), glm::vec3(2000.0f));
                if (realTarget.x <= 0.0f || realTarget.y <= 0.0f || realTarget.z <= 0.0f){
                    realTarget = glm::vec3(0.0f);
                }
            }
            pLastTime = SDL_GetTicks();
        }
        void setRealTarget(glm::vec3 newTarget){
            realTarget += newTarget;
        }

        void move(glm::vec3 forward, float time) {
            if (!pLastTime) { pLastTime = SDL_GetTicks(); }

            pMainController->move(physx::PxVec3(forward.x, forward.y, forward.z), 0.0f, (time),
                                  pControlFilter);
            pLastTime = SDL_GetTicks();
        }

        void translate(glm::vec3 pos) {
            pMainController->setPosition(physx::PxExtendedVec3(pos.x, pos.y, pos.z));
        }

        int onGround() {
            physx::PxControllerState state;
            pMainController->getState(state);
            return (!state.isMovingUp && (state.touchedActor != NULL));
        }

        physx::PxController *getController() { return pMainController; }

        //virtual void makeConfig(physx::PxControllerManager *controllerManager) = 0;

        physx::PxMaterial *getMaterial() { return pMainMat; }

        void setName(const char *newName) {
            pMainController->getActor()->setName(newName);
            generalStruct.name = std::string(newName);
            name = newName;
        }

        void setUserPointer(void *target){
            generalStruct.originalObject = target;
        }

        void setContactCallback(std::function<void(DG_Object*)> callback){
            objDesc.characterCallback = callback;
        }

        const char *getName() { return name; }
    protected:
        physx::PxMaterial *pMainMat;
        const char *name;
        physx::PxController *pMainController;
        physx::PxControllerFilters pControlFilter;

        uint32_t pLastTime = 0;
        glm::vec3 realTarget = glm::vec3(0.0f);
        glm::vec3 forceTarget = glm::vec3(0.0f);

        DemonBase::DemonUserData::generalStruct generalStruct;
        DemonBase::DemonUserData::DP_CHARACTER_OBJ_DESC objDesc;
    };
}

#endif //DEMONENGINE_B_CONTROLLER_H
