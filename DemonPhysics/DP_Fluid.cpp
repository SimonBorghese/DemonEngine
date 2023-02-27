//
// Created by simon on 10/3/22.
//
#ifdef _PHYSX3
#include "DP_Fluid.h"

namespace DemonPhysics {

    DP_Fluid::DP_Fluid(DP_PhysicsManager *physicsManager, uint32_t particleMax, particleDescription description) :
            _currentDescription(description), _maxParticle(particleMax){
        _particleSystem = physicsManager->getPhysics()->createPBDParticleSystem(*physicsManager->getCudaContext());

        /*
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

        ps->setParticleMass(0.01);
        ps->setMaxMotionDistance(0.5f);
        ps->setGridSize(10.0);
        //ps->setExternalAcceleration(PxVec3(0.0f, -98.1f, 0.0f));
        //ps->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
         */

        /*
        _fluid->setParticleMass(_currentDescription.mass);
        _fluid->setGridSize(_currentDescription.gridSize);
        _fluid->setMaxMotionDistance(_currentDescription.maxMotion);

        if (_fluid) {
            physicsManager->getScene()->addActor(*_fluid);
        }
        _availableIndices = std::vector<uint32_t>(_maxParticle, 1);
         */
        _fluid = physicsManager->getPhysics()->createParticleBuffer(particleMax, 10, physicsManager->getCudaContext());
    }

    DP_Fluid::~DP_Fluid() {}

    void DP_Fluid::createParticles(std::vector<uint32_t> indices, std::vector<glm::vec3> positions, std::vector<glm::vec3> velocities){
        PxParticleCreationData particleCreationData;

        std::vector<PxVec3> startingPoses;
        std::vector<PxVec3> velocityPoses;
        std::vector<uint32_t> flags;
        srand((unsigned int)time(NULL));

        for (unsigned int p = 0; p < indices.size(); p++){
            uint32_t index = indices.at(p);
            _availableIndices.at(index) = 0;
            startingPoses.insert(startingPoses.begin() + index, PxVec3(positions.at(index).x, positions.at(index).y, positions.at(index).z));
            velocityPoses.insert(velocityPoses.begin() + index, PxVec3(velocities.at(index).x, velocities.at(index).y, velocities.at(index).z));
            flags.insert(flags.begin() + index, PxParticleBaseFlag::eENABLED | PxParticleBaseFlag::eCOLLISION_WITH_DYNAMIC_ACTORS | PxParticleBaseFlag::eCOLLISION_TWOWAY);
        }

        particleCreationData.numParticles = indices.size();
        particleCreationData.indexBuffer = PxStrideIterator<const uint32_t>(indices.data());
        particleCreationData.positionBuffer = PxStrideIterator<const PxVec3>(startingPoses.data());
        particleCreationData.velocityBuffer = PxStrideIterator<const PxVec3>((velocityPoses.data()), 0);
        particleCreationData.flagBuffer = PxStrideIterator<const uint32_t>(flags.data());


        bool success = _fluid->createParticles(particleCreationData);
        assert(success);
        printf("Created particles!\n");
    }

    void DP_Fluid::emitParticles(uint32_t num, glm::vec3 startPos, glm::vec3 startVelocity){
        std::vector<uint32_t> indices;
        std::vector<PxVec3> startingPositions;
        std::vector<PxVec3> startingVelocites;
        std::vector<uint32_t> flags;
        for (unsigned int i = 0; i < _availableIndices.size(); i++){
            if (_availableIndices.at(i)){
                _availableIndices.at(i) = 0;
                indices.push_back(i);
                startingPositions.push_back(PxVec3(startPos.x, startPos.y, startPos.z));
                startingVelocites.push_back(PxVec3(startVelocity.x, startVelocity.y, startVelocity.z));
                flags.push_back(PxParticleBaseFlag::eENABLED | PxParticleBaseFlag::eCOLLISION_WITH_DYNAMIC_ACTORS | PxParticleBaseFlag::eCOLLISION_TWOWAY);
                if (indices.size() == num){
                    break;
                }
            }
        }
        PxParticleCreationData particleCreationData;
        particleCreationData.numParticles = indices.size();
        particleCreationData.indexBuffer = PxStrideIterator<const uint32_t>(indices.data());
        particleCreationData.positionBuffer = PxStrideIterator<const PxVec3>(startingPositions.data());
        particleCreationData.velocityBuffer = PxStrideIterator<const PxVec3>((startingVelocites.data()), 0);
        particleCreationData.flagBuffer = PxStrideIterator<const uint32_t>(flags.data());


        bool success = _fluid->createParticles(particleCreationData);
        assert(success);
        printf("Created particles!\n");


    }

    std::map<uint32_t, glm::vec3> DP_Fluid::getParticlePositions(){
        std::map<uint32_t, glm::vec3> currentMap;
        PxParticleReadData* rd = _fluid->lockParticleReadData();

        if (rd)
        {
            PxStrideIterator<const PxParticleFlags> flagsIt(rd->flagsBuffer);
            PxStrideIterator<const PxVec3> positionIt(rd->positionBuffer);

            for (unsigned i = 0; i < rd->validParticleRange; ++i, ++flagsIt, ++positionIt)
            {
                if (*flagsIt & PxParticleFlag::eVALID)
                {
                    // access particle position
                    const PxVec3& position = *positionIt;
                    currentMap.insert(std::pair<uint32_t, glm::vec3>(i, glm::vec3(position.x, position.y, position.z)));
                }
            }

            // return ownership of the buffers back to the SDK
            rd->unlock();
        }
        return currentMap;
    }

    void DP_Fluid::releaseParticles(std::vector<uint32_t> ){

    }
} // DemonPhysics
#endif
