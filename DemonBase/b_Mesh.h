//
// Created by simon on 6/6/22.
//

#ifndef DEMONENGINE_B_MESH_H
#define DEMONENGINE_B_MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <assimp/material.h>

#include <iostream>
#include <chrono>
#include <math.h>

typedef struct {
    glm::vec3 iPosition;
    glm::vec3 iNormal;
    glm::vec2 iTextCord;
    glm::ivec4 iBoneIds;
    glm::vec4 weights;
    int animationBody;
} Vertex;

namespace DemonBase {

    class b_Mesh {
    public:
        // NOTE 2 SELF: INDICES-less MESH IS OBSOLETE AND NO LONGER SUPPORTED IN NEW RENDERER
        b_Mesh(Vertex *vertices, unsigned int vertexLen, unsigned int *indices, unsigned int indexLen) :
        numVertex(vertexLen), numIndex(indexLen){
            //_vertices.reserve(vertexLen);
            std::copy(&vertices[0], &vertices[vertexLen], std::back_inserter(_vertices));
            if (indexLen > 0) {
                //_indices.reserve(indexLen);
                std::copy(&indices[0], &indices[indexLen], std::back_inserter(_indices));
            }

            // Make the raw vertices
            // Copy 3 floats, then skip 2
            uint8_t *vertexEs = (uint8_t *) vertices;
            //float good[vertexLen * 3];
            _rawVerticesPTR = (float *) malloc(sizeof(float) * (vertexLen * 3));

            for (unsigned int v = 0; v < vertexLen; v++) {
                memcpy(&_rawVerticesPTR[v * 3], vertexEs + (v * sizeof(Vertex)), sizeof(float) * 3);

                _glmVertices.push_back(glm::vec3(0.0f));
                memcpy(&_glmVertices.at(_glmVertices.size() -1), vertexEs + (v * sizeof(Vertex)), sizeof(float) * 3);
            }


            _rawVertices.insert(_rawVertices.end(), _rawVerticesPTR, _rawVerticesPTR + (vertexLen * 3));
        }

        ~b_Mesh() {
            //free(_rawVerticesPTR);
        }

        std::vector<Vertex> getVerticesVector() { return _vertices; }

        std::vector<glm::vec3> getGLMVertices() { return _glmVertices; }

        std::vector<float> getRawVerticesVector() { return _rawVertices; }

        std::vector<unsigned int> getIndicesVector() { return _indices; }

        Vertex *getVertices() {
            return _vertices.data();
        }

        float *getRawVertices(unsigned int *outputLen) {
            *outputLen = _rawVertices.size();
            return _rawVerticesPTR;
        }

        unsigned int *getIndices() {
            return _indices.data();
        }

        unsigned int getVertexLength(){
            return numVertex;
        }
        unsigned int getIndexLength(){
            return numIndex;
        }

        void setMaterial(aiMaterial *mat) { _material = mat; }

        aiMaterial *getMaterial() { return _material; }

        aiString getTextureDiffuse() { return diffuse_fileName; }
        aiString getTextureNormal() { return normal_fileName; }

        void setTextureDiffuse(aiString target) { diffuse_fileName = target; }
        void setTextureNormal(aiString target) { normal_fileName = target; }

        void setIndexOffset(uint64_t offset){
            _indexOffset = offset;
        }

        void setModelName(aiString *target){
            model_name = std::string(target->C_Str());
            //printf("Name: %s\n", target->C_Str());
        }
        void setModelName(std::string target){
            model_name = target;
        }
        const char *getModelName() {
            return model_name.c_str();
        }

    protected:
        std::vector<Vertex> _vertices;
        unsigned int numVertex;
        std::vector<glm::vec3> _glmVertices;
        std::vector<float> _rawVertices;
        float *_rawVerticesPTR;
        std::vector<unsigned int> _indices;
        unsigned int numIndex;
        aiMaterial *_material;
        aiString diffuse_fileName;
        aiString normal_fileName = aiString("");
        std::string model_name;

        uint64_t _indexOffset = 0;
    };
}

#endif //DEMONENGINE_B_MESH_H
