//
// Created by simon on 1/10/23.
//

#include "MeshRenderer.h"

namespace DGL {
    Shader* MeshRenderer::_globalShader = nullptr;

    MeshRenderer::MeshRenderer(Shader *shader, Mesh **mesh, uint32_t numMesh) : _shader(shader), _meshes(mesh), _numMeshes(numMesh){}

    MeshRenderer::MeshRenderer(Shader *shader, DemonBase::b_Mesh **mesh, uint32_t numMesh) : _shader(shader), _numMeshes(numMesh){
        _meshes = (Mesh**) malloc(sizeof(Mesh*) * numMesh);
        for (uint32_t m = 0; m < numMesh; m++){
            _meshes[m] = new Mesh(*mesh[m]);
            _meshes[m]->createMesh();
        }
    }

    void MeshRenderer::bindTransform(){
        if (!_globalShader) {
            _shader->useShader();
            _shader->uniformMat4("model", _transform.getModel());
        } else{
            _globalShader->useShader();
            _globalShader->uniformMat4("model", _transform.getModel());
        }
    }
    void MeshRenderer::render(DGL::Shader *overrideShader){
        if (!_globalShader) {
            if (!overrideShader) {
                _shader->useShader();
                bindTransform();
                _shader->bindGlobals();
                for (uint32_t m = 0; m < _numMeshes; m++) {
                    _meshes[m]->getTexture()->bindTextures();
                    DGL::Shader::remakeTextures();
                    _shader->uniformInt("_diffuse", 0);
                    _shader->uniformInt("_normal", 1);
                    _meshes[m]->renderMesh();
                }
            } else{
                overrideShader->useShader();
                overrideShader->uniformMat4("model", _transform.getModel());
                overrideShader->bindGlobals();
                for (uint32_t m = 0; m < _numMeshes; m++) {
                    _meshes[m]->getTexture()->bindTextures();
                    DGL::Shader::remakeTextures();
                    overrideShader->uniformInt("_diffuse", 0);
                    overrideShader->uniformInt("_normal", 1);
                    overrideShader->uniformInt("_shadowMap", 2);
                    _meshes[m]->renderMesh();
                }
            }
        }
        else{
            _globalShader->useShader();
            bindTransform();
            _globalShader->bindGlobals();
            bindTransform();
            for (uint32_t m = 0; m < _numMeshes; m++) {
                _meshes[m]->getTexture()->bindTextures();
                DGL::Shader::remakeTextures();
                _globalShader->uniformInt("_diffuse", 0);
                _globalShader->uniformInt("_normal", 1);
                _globalShader->uniformInt("_shadowMap", 2);
                _meshes[m]->renderMesh();
            }
        }

        _isRendered = false;
        for (uint32_t m = 0; m < _numMeshes; m++){
            if (DGL::Mesh::_enableOcculusion && _meshes[m]->getNumSamples() > 0){
                _isRendered = true;
            }
        }
    }
    void MeshRenderer::destroyMeshes(){
        for (uint32_t m = 0; m < _numMeshes; m++){
            _meshes[m]->destroyMesh();
            delete _meshes[m];
        }
    }
    std::vector<Mesh*> MeshRenderer::getMeshes(){
        return {&_meshes[0], &_meshes[_numMeshes]};
    }

    bool MeshRenderer::getRenderStatus() const {
        return _isRendered;
    }
} // DGL