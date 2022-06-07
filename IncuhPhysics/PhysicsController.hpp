#ifndef PHYSICS_CONTROLLER
#define PHYSICS_CONTROLLER
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/characterkinematic/PxController.h>

// Needed for time calculation, bad to include but I need it
#include <SDL2/SDL.h>

// A abstract class for physics controllers

namespace IncuhPhysics {
    class PhysicsController {
    public:

        void getPosition(float *target) {
            if (target == NULL) {
                return;
            }
            target[0] = pMainController->getPosition().x;
            target[1] = pMainController->getPosition().y;
            target[2] = pMainController->getPosition().z;


        }

        void setController(physx::PxController *newController) { pMainController = newController; }

        void move(float *forward) {
            if (!pLastTime) { pLastTime = SDL_GetTicks(); }
            if (forward == NULL) { return; }

            pMainController->move(physx::PxVec3(forward[0], forward[1], forward[2]), 0.0f, (SDL_GetTicks() - pLastTime),
                                  pControlFilter);
            pLastTime = SDL_GetTicks();
        }

        void translate(float *pos) {
            if (pos == NULL) { return; }

            pMainController->setPosition(physx::PxExtendedVec3(pos[0], pos[1], pos[2]));
        }

        int onGround() {
            physx::PxControllerState state;
            pMainController->getState(state);
            return (!state.isMovingUp && (state.touchedActor != NULL));
        }

        physx::PxController *getController() { return pMainController; }

        virtual void makeConfig(physx::PxControllerManager *controllerManager) = 0;

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

#endif
