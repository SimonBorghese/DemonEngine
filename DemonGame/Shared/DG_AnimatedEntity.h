//
// Created by simon on 9/15/22.
//

#ifndef DEMONENGINE_DG_ANIMATEDENTITY_H
#define DEMONENGINE_DG_ANIMATEDENTITY_H

class DA_riggedMesh;

#include "DG_Entity.h"
#include <DemonAnimation/DA_riggedMesh.h>
#include <DemonIO/DI_AnimationSceneLoader.h>
#include <iostream>
#include <memory>

namespace DemonGame {

    class DG_AnimatedEntity : public DG_Entity{
    public:
        DG_AnimatedEntity(DGL::Shader *targetShader) :
                DG_Entity(targetShader) {

        }

        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        void playAnimation(float currentTime);

        void setAnimation(int index);
        // WARNING: DON'T USE VERY SLOW
        void setAnimation(std::string name);

        void playOnce(float currentTime);

        int isAnimationFinished(float currentTime);

        int getAnimationIndex();

        int enableDepth = 1;

        std::vector<DemonAnimation::DA_riggedMesh*> getMeshes();
    private:
        DemonAnimation::DA_riggedMesh **animMeshes;
        DemonBase::b_Mesh **renderMesh;
        unsigned int numMeshes;
    };

} // DemonGame

#endif //DEMONENGINE_DG_ANIMATEDENTITY_H
