//
// Created by simon on 5/28/22.
//

#include "DR_MeshRenderer.h"

namespace DemonRender {
    void DR_MeshRenderer::addMesh(DR_Mesh *targetMesh){
        _targetMeshes.push_back(targetMesh);
    }
    void DR_MeshRenderer::setShader(DR_Shader *targetShader){
        _targetShader = targetShader;
    }

    void DR_MeshRenderer::renderMeshes(){
        _targetShader->useProgram();
        for (unsigned int m = 0; m < _targetMeshes.size(); m++){
            _targetMeshes.at(m)->renderMesh();
        }

    }
} // DemonRender