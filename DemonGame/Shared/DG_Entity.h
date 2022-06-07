//
// Created by simon on 6/1/22.
//

#ifndef DEMONENGINE_DG_ENTITY_H
#define DEMONENGINE_DG_ENTITY_H
#include <string>
#include <DemonRender/DR_RenderManager.h>
#include <DemonRender/DR_MeshRenderer.h>
#include <DemonRender/DR_Shader.h>
#include <DemonWorld/DW_Transform.h>
#include <DemonIO/DI_SceneLoader.h>

namespace DemonGame {
    class DG_Entity: public DemonWorld::DW_Transform {
    public:
        DG_Entity(DemonRender::DR_RenderManager *targetRender, DemonRender::DR_Shader *targetShader) :
        renderManager(targetRender),
        meshShader(targetShader) {}


        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        DemonRender::DR_MeshRenderer* getMeshRenderer() { return mainMeshRenderer; }

        void destroyEntity();

        //DemonWorld::DW_Transform *getTransform() { return mainTransform; }
    private:
        //DemonWorld::DW_Transform *mainTransform;
        DemonRender::DR_MeshRenderer *mainMeshRenderer;

        DemonRender::DR_RenderManager *renderManager;
        DemonRender::DR_Shader *meshShader;
    };

} // DemonGame

#endif //DEMONENGINE_DG_ENTITY_H
