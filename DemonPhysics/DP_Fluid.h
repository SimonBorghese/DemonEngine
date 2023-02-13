//
// Created by simon on 10/3/22.
//

#ifndef DEMONENGINE_DP_FLUID_H
#define DEMONENGINE_DP_FLUID_H

#ifdef _PHYSX3
#include "DP_PhysicsManager.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>

namespace DemonPhysics {
    /*
     *
     * PxParticleFluid *ps = engine->getPhysicsManager()->getPhysics()->createParticleFluid(maxParticles, false);
    ps->setGridSize(3.0);
    ps->setMaxMotionDistance(2.0f);
    ps->setDamping(0.0f);
    ps->setRestitution(0.2f);
    ps->setDynamicFriction(0.05f);
    ps->setDamping(0.5f);
    ps->setRestitution(0.0f);
    ps->setDynamicFriction(0.0f);
    ps->setViscosity(300.0f);
    ps->setStiffness(100.0f);
    //ps->setExternalAcceleration(PxVec3(0.0f, -9.81f, 0.0f));
    //ps->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

    if (ps){
        engine->getPhysicsManager()->getScene()->addActor(*ps);
    }
     */
    class DP_Fluid {
    public:
        typedef struct{
            float mass;
            float gridSize;
            float maxMotion;
        } particleDescription;

        DP_Fluid(DP_PhysicsManager *physicsManager, uint32_t particleMax, particleDescription description);

        virtual ~DP_Fluid();

        void createParticles(std::vector<uint32_t> indices, std::vector<glm::vec3> positions, std::vector<glm::vec3> velocities);

        void emitParticles(uint32_t num, glm::vec3 startPos, glm::vec3 startVelocity);

        std::map<uint32_t, glm::vec3> getParticlePositions();

        void releaseParticles(std::vector<uint32_t> indices);

        physx::PxParticleFluid *getFluid() { return _fluid; }

    private:
        physx::PxParticleFluid *_fluid = nullptr;
        particleDescription _currentDescription;
        uint32_t _maxParticle;
        std::vector<uint32_t> _availableIndices;

    };

} // DemonPhysics

#else
#error "You are not using PhysX 3 and yet are trying to include PhysX 3 features (Particles in this case)"
#endif // _PHYSX3


#endif //DEMONENGINE_DP_FLUID_H
