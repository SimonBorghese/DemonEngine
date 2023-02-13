//
// Created by simon on 6/1/22.
//

#ifndef DEMONENGINE_DG_ENTITY_H
#define DEMONENGINE_DG_ENTITY_H

#include <string>
#include <functional>
#include <DemonGL/DemonGL.h>
#include <DemonWorld/DW_Transform.h>
#include <DemonIO/DI_SceneLoader.h>
#include <DemonIO/DI_BSPLoader.h>
#include <DemonBase/b_MeshEntity.h>

namespace DemonGame {
    class DG_Entity : public DemonBase::b_MeshEntity {
    public:
        DG_Entity(DGL::Shader *targetShader) :
                _shader(targetShader) {}

        virtual ~DG_Entity(){}


        void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f));

        void createEntityFromExistingMesh(DemonBase::b_Mesh **meshes, uint32_t numMesh,
                                          glm::vec3 pos = glm::vec3(0.0f),
                                          glm::vec3 rotation = glm::vec3(0.0f));
        DGL::MeshRenderer *getMeshRenderer() { return _primaryMesh; }

        void destroyEntity();

        // The entity only renders a mesh, that is to be managed by the render manager, thus update is useless
        void update(DGL::Shader *overrideShader) {
            if (_preRender) { _preRender(); }
            if (enableRender) {
                _primaryMesh->render(overrideShader);
            }
            if (_updateFunc) {_updateFunc((DG_Entity*) this);}
            if (_postRender) { _postRender();}
        }

        void setUpdateFunc(std::function<void(DG_Entity*)> newFunc) { _updateFunc = newFunc; }
        void setPrerenderFunc(std::function<void()> newPre) { _preRender = newPre; }
        void setPostrenderFunc(std::function<void()> newPost) { _postRender = newPost; }

        void toggleRender(int enable){
            enableRender = enable;
        }

        int getRender(){
            return enableRender;
        }

    protected:
        DGL::MeshRenderer *_primaryMesh;
        DGL::Shader *_shader;

        DemonBase::b_Mesh **_loadedMesh;
        uint32_t _numMeshes;

        int enableRender = 1;

        std::function<void(DG_Entity*)> _updateFunc = nullptr;
        std::function<void()> _preRender = nullptr;
        std::function<void()> _postRender = nullptr;
    };

} // DemonGame

#endif //DEMONENGINE_DG_ENTITY_H
