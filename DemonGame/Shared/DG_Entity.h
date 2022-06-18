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
#include <DemonBase/b_GameObject.h>

namespace DemonGame {
    class DG_Entity : public DemonBase::b_GameObject {
    public:
        DG_Entity(DemonRender::DR_RenderManager *targetRender, DemonRender::DR_Shader *targetShader) :
                renderManager(targetRender),
                meshShader(targetShader) { mainTransform.createTransform(); }


        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        DemonRender::DR_MeshRenderer *getMeshRenderer() { return mainMeshRenderer; }

        void destroyEntity();

        // The entity only renders a mesh, that is to be managed by the render manager, thus update is useless
        void update() {}


        DemonWorld::DW_Transform *getTransform() { return &mainTransform; }

    protected:
        DemonWorld::DW_Transform mainTransform;
        DemonRender::DR_MeshRenderer *mainMeshRenderer;

        DemonRender::DR_RenderManager *renderManager;
        DemonRender::DR_Shader *meshShader;
    };

} // DemonGame

#endif //DEMONENGINE_DG_ENTITY_H
