//
// Created by simon on 5/28/22.
//

#ifndef DEMONENGINE_DR_MESHRENDERER_H
#define DEMONENGINE_DR_MESHRENDERER_H
#include "DR_Mesh.h"
#include "DR_Shader.h"
#include <vector>

namespace DemonRender {

    class DR_MeshRenderer {
    public:
        DR_MeshRenderer() {}

        void addMesh(DR_Mesh *targetMesh);
        void setShader(DR_Shader *targetShader);

        void renderMeshes();
    private:
        std::vector<DR_Mesh*> _targetMeshes;
        DR_Shader *_targetShader;
    };

} // DemonRender

#endif //DEMONENGINE_DR_MESHRENDERER_H
