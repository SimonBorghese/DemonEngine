//
// Created by simon on 6/6/22.
//

#include "DI_SceneLoader.h"

namespace DemonIO {
    DemonBase::b_Mesh **DI_SceneLoader::loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale) {
        std::vector<DemonBase::b_Mesh> newMeshes;

        Assimp::Importer *oModelLoader = new Assimp::Importer;
        const aiScene *oScene = oModelLoader->ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenUVCoords |
                                                                 aiProcess_FindInvalidData | aiProcess_OptimizeMeshes);
        if (strcmp("", oModelLoader->GetErrorString()) != 0) {
            (fmt::print("Failed to load: {}, Error: {}\n", fileName, oModelLoader->GetErrorString()));
        }

        // Allocate our meshes
        DemonBase::b_Mesh **meshes = (DemonBase::b_Mesh **) malloc(sizeof(DemonBase::b_Mesh *) * oScene->mNumMeshes);

        for (unsigned int m = 0; m < oScene->mNumMeshes; m++) {
            unsigned int numVertices = oScene->mMeshes[m]->mNumVertices;
            // Stack allocations go brrrrr....
            Vertex m_vertices[numVertices];
            for (unsigned int v = 0; v < numVertices; v++) {
                m_vertices[v].iPosition.x = oScene->mMeshes[m]->mVertices[v].x * scale.x;
                m_vertices[v].iPosition.y = oScene->mMeshes[m]->mVertices[v].y * scale.y;
                m_vertices[v].iPosition.z = oScene->mMeshes[m]->mVertices[v].z * scale.z;

                if (oScene->mMeshes[m]->HasNormals()) {
                    m_vertices[v].iNormal.x = oScene->mMeshes[m]->mNormals[v].x * scale.x;
                    m_vertices[v].iNormal.y = oScene->mMeshes[m]->mNormals[v].y * scale.y;
                    m_vertices[v].iNormal.z = oScene->mMeshes[m]->mNormals[v].z * scale.z;
                } else {
                    m_vertices[v].iNormal.x = 0.0f;
                    m_vertices[v].iNormal.y = 0.0f;
                    m_vertices[v].iNormal.z = 0.0f;
                }

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
            for (unsigned int f = 0; f < oScene->mMeshes[m]->mNumFaces; f++) {
                aiFace face = oScene->mMeshes[m]->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; i++) {
                    m_indices.push_back(face.mIndices[i]);
                }
            }
            /*
            Vertex m_REALvertices[m_indices.size()];
            for (unsigned int i = 0; i < m_indices.size(); i++){
                m_REALvertices[i] = m_vertices[m_indices.at(i)];
            }
             */
            // Make it a heap allocation so we don't lose it
            meshes[m] = new DemonBase::b_Mesh(&m_vertices[0], numVertices, m_indices.data(), m_indices.size());
            aiMaterial *matSafe = oScene->mMaterials[oScene->mMeshes[m]->mMaterialIndex];

            if (matSafe != NULL) {
                aiString tex_loc;
                matSafe->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
                meshes[m]->setTextureDiffuse(tex_loc);
            }

            meshes[m]->setMaterial(matSafe);
        }

        // Allocate meshes
        *meshCountOut = oScene->mNumMeshes;

        delete oScene;

        return meshes;
    }
} // DemonIO