//
// Created by simon on 9/15/22.
//

#ifndef DEMONENGINE_DG_ANIMATEDENTITY_H
#define DEMONENGINE_DG_ANIMATEDENTITY_H
#include "DG_Entity.h"
#include <DemonAnimation/DA_riggedMesh.h>
#include <DemonIO/DI_AnimationSceneLoader.h>


namespace DemonGame {

    class DG_AnimatedEntity : public DG_Entity{
    public:
        DG_AnimatedEntity(DemonRender::DR_RenderManager *targetRender, DemonRender::DR_Shader *targetShader) :
                DG_Entity(targetRender, targetShader) {

        }

        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        void playAnimation(float currentTime);

        void setAnimation(int index);

        void playOnce(float currentTime);

        int isAnimationFinished(float currentTime);
    private:
        DemonAnimation::DA_riggedMesh **animMeshes;
        unsigned int numMeshes;
    };

} // DemonGame

#endif //DEMONENGINE_DG_ANIMATEDENTITY_H
