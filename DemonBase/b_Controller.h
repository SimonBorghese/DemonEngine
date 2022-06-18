//
// Created by simon on 6/13/22.
//

#ifndef DEMONENGINE_B_CONTROLLER_H
#define DEMONENGINE_B_CONTROLLER_H

#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/characterkinematic/PxController.h>

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

        void setController(physx::PxController *newController) { pMainController = newController; }

        void move(glm::vec3 forward) {
            if (!pLastTime) { pLastTime = SDL_GetTicks(); }

            pMainController->move(physx::PxVec3(forward.x, forward.y, forward.z), 0.0f, (SDL_GetTicks() - pLastTime),
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
            name = newName;
        }

        const char *getName() { return name; }

    protected:
        physx::PxMaterial *pMainMat;
        const char *name;
        physx::PxController *pMainController;
        physx::PxControllerFilters pControlFilter;

        uint32_t pLastTime = 0;
    };
}

#endif //DEMONENGINE_B_CONTROLLER_H
