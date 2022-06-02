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
        if (modelLocation == 0){
            modelLocation = _targetShader->getUniformLocation("model");
        }
        if (_currentTransform != nullptr){
            _targetShader->bindMatrix4f(modelLocation, _currentTransform->getModel());
        }
        for (unsigned int m = 0; m < _targetMeshes.size(); m++){
            _targetShader->bindDiffuseTexture(_targetMeshes.at(m)->getTexture());
            _targetMeshes.at(m)->renderMesh();
        }

    }

    void DR_MeshRenderer::loadMeshFromFile(const char *file){
        Assimp::Importer *oModelLoader = new Assimp::Importer;
        const aiScene *oScene = oModelLoader->ReadFile(file, aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_FindInvalidData | aiProcess_OptimizeMeshes);
        if (strcmp("", oModelLoader->GetErrorString()) != 0)
        {
            (fmt::print("Failed to load: {}, Error: {}\n", file, oModelLoader->GetErrorString()));
        }

        for (unsigned int m = 0; m < oScene->mNumMeshes; m++) {
            DR_Mesh *targetMesh = new DR_Mesh;
            unsigned int numVertices = oScene->mMeshes[m]->mNumVertices;
            // Stack allocations go brrrrr....
            Vertex m_vertices[numVertices];
            for (unsigned int v = 0; v < numVertices; v++){
                m_vertices[v].iPosition.x = oScene->mMeshes[m]->mVertices[v].x;
                m_vertices[v].iPosition.y = oScene->mMeshes[m]->mVertices[v].y;
                m_vertices[v].iPosition.z = oScene->mMeshes[m]->mVertices[v].z;

                for (int t = 0; t < AI_MAX_NUMBER_OF_TEXTURECOORDS; t++) {
                    if (oScene->mMeshes[m]->mTextureCoords[t]) {
                        m_vertices[v].iTextCord.x = (oScene->mMeshes[m]->mTextureCoords[t][v]).x;
                        m_vertices[v].iTextCord.y = (oScene->mMeshes[m]->mTextureCoords[t][v]).y;
                        break;
                    }
                }
                if (!oScene->mMeshes[m]->mTextureCoords[0]) {
                    m_vertices[v].iTextCord.x = 0.0f;
                    m_vertices[v].iTextCord.y = 0.0f;
                }
            }

            std::vector<uint32_t> m_indices;
            for (unsigned int f = 0; f < oScene->mMeshes[m]->mNumFaces; f++){
                aiFace face = oScene->mMeshes[m]->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; i++){
                    m_indices.push_back(face.mIndices[i]);
                }

            }

            targetMesh->createMesh((&m_vertices[0]), numVertices, m_indices.data(), m_indices.size());
            addMesh(targetMesh);

            uint32_t mIndex = oScene->mMeshes[m]->mMaterialIndex;
            aiMaterial *mat = oScene->mMaterials[mIndex];
            if (mat != NULL){
                aiString tex_loc;
                mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
                std::string base_filename = tex_loc.C_Str();
                base_filename = base_filename.substr(base_filename.find_last_of("/\\") + 1);

                targetMesh->createTextureFromSTB(base_filename.c_str(), false);
            }
        }
        delete oModelLoader;

    }

    void DR_MeshRenderer::destroyMeshes(){
        for (unsigned int m = 0; m < _targetMeshes.size(); m++){
            _targetMeshes.at(m)->destroyMesh();
            delete _targetMeshes.at(m);
        }
    }
} // DemonRender