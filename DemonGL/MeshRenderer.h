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

        enum MESH_FLAGS{
            NO_FLAG = 0,
            MESH_RENDERED = 1,
            MESH_RENDER_SHADOW = 2,
            MESH_IN_VIEW = 4
        };

        ushort getFlags();
        void setFlags(ushort flags);
        int compareFlag(MESH_FLAGS testFlag);
        int addFlag(MESH_FLAGS flag);
        int removeFlag(MESH_FLAGS flag);
    private:
        Shader *_shader;
        GLuint _diffuseLocation = 0, _normalLocation = 0;
        Mesh **_meshes;
        uint32_t _numMeshes;
        DemonWorld::DW_Transform _transform;

        ushort _meshFlags = MESH_RENDERED | MESH_RENDER_SHADOW | MESH_IN_VIEW;
    };

} // DGL

#endif //DEMONENGINE_MESHRENDERER_H
