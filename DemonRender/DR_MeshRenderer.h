//
// Created by simon on 5/28/22.
//

#ifndef DEMONENGINE_DR_MESHRENDERER_H
#define DEMONENGINE_DR_MESHRENDERER_H

#include "DR_Mesh.h"
#include "DR_Shader.h"
#include <DemonWorld/DW_Transform.h>
#include <DemonBase/b_Mesh.h>
#include <DemonPhysics/DP_RigidMesh.h>

#include <vector>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <fmt/core.h>
#include <fmt/format.h>


namespace DemonRender {

    class DR_MeshRenderer {
    public:
        DR_MeshRenderer() {}

        void addMesh(DR_Mesh *targetMesh);

        void loadMeshFromFile(const char *file);

        void loadExistingMeshes(DemonBase::b_Mesh **meshes, unsigned int numMeshes);

        void setShader(DR_Shader *targetShader);

        void bindTransform(DemonWorld::DW_Transform *targetTransform) {
            _currentTransform = targetTransform;
        }

        void destroyMeshes();

        void renderMeshes();

    private:
        std::vector<DR_Mesh *> _targetMeshes;
        DR_Shader *_targetShader;

        DemonWorld::DW_Transform *_currentTransform = nullptr;
        GLuint modelLocation = 0;
    };

} // DemonRender

#endif //DEMONENGINE_DR_MESHRENDERER_H
