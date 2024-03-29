//
// Created by simon on 7/7/22.
//
#define CBSP_IMPLEMNT
#include "DI_BSPLoader.h"

namespace DemonIO {
    DemonBase::b_Mesh **DI_BSPLoader::loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale) {
        CBSP *map = CBSP_loadBSP(fileName);

        DemonBase::b_Mesh **meshes = (DemonBase::b_Mesh **) malloc(sizeof(DemonBase::b_Mesh *) * map->nModels);
        *meshCountOut = map->nModels;


        std::vector<Vertex> vertices;
        for (int v = 0; v < map->nVertexs; v++) {
            Vertex vertex;
            vertex.iPosition.x = map->mVertices[v].pos[0];
            vertex.iPosition.y = map->mVertices[v].pos[2];
            vertex.iPosition.z = -map->mVertices[v].pos[1];
            vertex.iPosition *= scale;
            vertex.iNormal.x = map->mVertices[v].normal[0];
            vertex.iNormal.y = map->mVertices[v].normal[2];
            vertex.iNormal.z = map->mVertices[v].normal[1];
            //vertex.iNormal *= scale;
            vertex.iTextCord.x = map->mVertices[v].texcord[0][0];
            vertex.iTextCord.y = -map->mVertices[v].texcord[0][1];

            //vertex.iTextCord *= glm::vec2(scale.x, scale.y );
            vertex.animationBody = 0;
            vertices.push_back(vertex);
        }

        for (int m = 0; m < map->nModels; m++) {
            std::vector<unsigned int> indices;

            CBSPModel model = map->mModels[m];
            for (int f = model.face; f < model.face + model.nfaces; f++) {
                CBSPFace face = map->mFaces[f];
                for (int fm = face.mesh; fm < face.mesh + face.nMesh; fm++) {
                    CBSPMesh mesh = map->mMeshes[fm];
                    indices.push_back(face.vertex + mesh.offset);
                }
            }
            meshes[m] = (new DemonBase::b_Mesh(vertices.data(),
                                               vertices.size(),
                                               indices.data(),
                                               indices.size()));
            //printf("Sunset: %s\n", map->mTextures[map->mFaces[model.face].texture].name);
            meshes[m]->setTextureDiffuse(
                    aiString(fmt::format("bsp/textures/{}.jpg", map->mTextures[map->mFaces[model.face].texture].name).c_str()));

        }
        return meshes;
    }



    DemonBase::b_Mesh **DI_BSPLoader::loadMeshesFromCBSP(CBSP *map, unsigned int *meshCountOut, glm::vec3 scale){
        DemonBase::b_Mesh **meshes = (DemonBase::b_Mesh **) malloc(sizeof(DemonBase::b_Mesh *) * map->nModels);
        *meshCountOut = map->nModels;


        std::vector<Vertex> vertices;
        for (int v = 0; v < map->nVertexs; v++){
            Vertex vertex;
            vertex.iPosition.x = map->mVertices[v].pos[0];
            vertex.iPosition.y = map->mVertices[v].pos[2];
            vertex.iPosition.z = -map->mVertices[v].pos[1];
            vertex.iPosition *= scale;
            vertex.iNormal.x = map->mVertices[v].normal[0];
            vertex.iNormal.y = map->mVertices[v].normal[2];
            vertex.iNormal.z = map->mVertices[v].normal[1];
            //vertex.iNormal *= scale;
            vertex.iTextCord.x = map->mVertices[v].texcord[0][0];
            vertex.iTextCord.y = -map->mVertices[v].texcord[0][1];
            //vertex.iTextCord *= glm::vec2(scale.x, scale.y);
            vertex.animationBody = 0;
            vertices.push_back(vertex);

        }

        for (int m = 0; m < map->nModels; m++){
            std::vector<unsigned int> indices;

            CBSPModel model = map->mModels[m];
            for (int f = model.face; f < model.face + model.nfaces; f++){
                CBSPFace face = map->mFaces[f];
                for (int fm = face.mesh; fm < face.mesh + face.nMesh; fm++){
                    CBSPMesh mesh = map->mMeshes[fm];
                    indices.push_back(face.vertex + mesh.offset);
                }
            }
            meshes[m] = (new DemonBase::b_Mesh(vertices.data(),
                                               vertices.size(),
                                               indices.data(),
                                               indices.size()));
            //printf("Sunset: %s\n", map->mTextures[map->mFaces[model.face].texture].name);
        }
        return meshes;
    }

    DemonBase::b_Mesh **DI_BSPLoader::loadFacesFromCBSP(CBSP *map, unsigned int *meshCountOut, glm::vec3 scale){

        DemonBase::b_Mesh **meshes = (DemonBase::b_Mesh **) malloc(sizeof(DemonBase::b_Mesh *) * map->nFaces);
        //DemonBase::b_Mesh **meshes = (DemonBase::b_Mesh **) malloc(sizeof(DemonBase::b_Mesh *) * map->nFaces);
        *meshCountOut = map->nFaces;
        //*meshCountOut = map->nFaces;


        std::vector<Vertex> vertices;
        for (int v = 0; v < map->nVertexs; v++){
            Vertex vertex;
            vertex.iPosition.x = map->mVertices[v].pos[0];
            vertex.iPosition.y = map->mVertices[v].pos[2];
            vertex.iPosition.z = -map->mVertices[v].pos[1];
            vertex.iPosition *= scale;
            vertex.iNormal.x = map->mVertices[v].normal[0];
            vertex.iNormal.y = map->mVertices[v].normal[2];
            vertex.iNormal.z = -map->mVertices[v].normal[1];
            //vertex.iNormal *= scale;
            vertex.iTextCord.x = map->mVertices[v].texcord[0][0] * 0.4f;
            vertex.iTextCord.y = -map->mVertices[v].texcord[0][1] * 0.4f;
            printf("Text: %f %f\n", vertex.iTextCord.x, vertex.iTextCord.y);
            //vertex.iTextCord /= glm::vec2(scale.x, scale.y);
            vertex.animationBody = 0;
            vertices.push_back(vertex);

            ///printf("Vertex: %f %f %f\n", vertex.iPosition.x, vertex.iPosition.y, vertex.iPosition.z);
        }
        unsigned int currentFace = 0;
        std::map<int, std::vector<unsigned int>> indices;
        for (int t = 0; t < map->nFaces; t++){
            indices.insert(std::pair<int, std::vector<unsigned int>>(t, std::vector<unsigned  int>()));
        }
        for (int m = 0; m < map->nModels; m++){
            //printf("[BSP] Found %d models\n", map->nModels);

            CBSPModel model = map->mModels[m];
            //int currentTexture = map->mFaces[model.face].texture;
            for (int f = model.face; f < model.face + model.nfaces; f++){
                CBSPFace face = map->mFaces[f];
                    for (int fm = face.mesh; fm < face.mesh + face.nMesh; fm++) {
                        CBSPMesh mesh = map->mMeshes[fm];
                        indices.at(f).push_back(face.vertex + mesh.offset);
                    }
                currentFace++;
            }

        }

        for (int t = 0; t < map->nFaces; t++) {
            meshes[t] = (new DemonBase::b_Mesh(vertices.data(),
                                               vertices.size(),
                                               indices.at(t).data(),
                                               indices.at(t).size()));
            meshes[t]->setTextureDiffuse(
                    aiString(fmt::format("bsp/{}.jpg",
                                         map->mTextures[map->mFaces[t].texture].name).c_str()));

        }
        return meshes;
    }
    CBSP *DI_BSPLoader::createBSP(const char *filename){
        return CBSP_loadBSP(filename);
    }

    glm::vec3 DI_BSPLoader::getPosFromEntity(CBSP_Entity *entity){
        glm::vec3 heapAlloc = glm::vec3(0.0f);
        CBSP_getOriginFromEntity(entity, (float*) &heapAlloc);

        return heapAlloc * glm::vec3(1.0f/8.0f);
        // Fuck you C++
    }
} // DemonIO