//
// Created by simon on 1/10/23.
//

#ifndef DEMONENGINE_MESHRENDERER_H
#define DEMONENGINE_MESHRENDERER_H
#include "Shader.h"
#include <glm/glm.hpp>
#include <DemonWorld/DW_Transform.h>
#include <vector>
#include "Mesh.h"

namespace DGL {

    class MeshRenderer {
    public:
        MeshRenderer(Shader *shader, Mesh **mesh, uint32_t numMesh);
        MeshRenderer(Shader *shader, DemonBase::b_Mesh **mesh, uint32_t numMesh);


        DemonWorld::DW_Transform* getTransform() { return &_transform; }

        void bindTransform();
        void render(DGL::Shader *overrideShader = nullptr);
        void destroyMeshes();

        static Shader* _globalShader;

        std::vector<Mesh*> getMeshes();

        bool getRenderStatus();
    private:
        Shader *_shader;
        Mesh **_meshes;
        uint32_t _numMeshes;
        DemonWorld::DW_Transform _transform;
        bool _isRendered = false;
    };

} // DGL

#endif //DEMONENGINE_MESHRENDERER_H
