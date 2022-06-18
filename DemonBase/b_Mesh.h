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
} Vertex;

namespace DemonBase {

    class b_Mesh {
    public:
        b_Mesh(Vertex *vertices, unsigned int vertexLen, unsigned int *indices, unsigned int indexLen) {
            std::copy(&vertices[0], &vertices[vertexLen], std::back_inserter(_vertices));
            if (indexLen > 0) {
                std::copy(&indices[0], &indices[indexLen], std::back_inserter(_indices));
            }

            // Make the raw vertices
            // Copy 3 floats, then skip 2
            uint8_t *vertexEs = (uint8_t *) vertices;
            //float good[vertexLen * 3];
            _rawVerticesPTR = (float *) malloc(sizeof(float) * (vertexLen * 3));

            for (unsigned int v = 0; v < vertexLen; v++) {
                memcpy(&_rawVerticesPTR[v * 3], vertexEs + (v * sizeof(Vertex)), sizeof(float) * 3);
            }


            _rawVertices.insert(_rawVertices.end(), _rawVerticesPTR, _rawVerticesPTR + (vertexLen * 3));
        }

        ~b_Mesh() {
            free(_rawVerticesPTR);
        }

        std::vector<Vertex> getVerticesVector() { return _vertices; }

        std::vector<float> getRawVerticesVector() { return _rawVertices; }

        std::vector<unsigned int> getIndicesVector() { return _indices; }

        Vertex *getVertices(unsigned int *outputLen) {
            *outputLen = _vertices.size();
            return _vertices.data();
        }

        float *getRawVertices(unsigned int *outputLen) {
            *outputLen = _rawVertices.size();
            return _rawVerticesPTR;
        }

        unsigned int *getIndices(unsigned int *outputLen) {
            *outputLen = _indices.size();
            return _indices.data();
        }

        void setMaterial(aiMaterial *mat) { _material = mat; }

        aiMaterial *getMaterial() { return _material; }

        aiString getTextureDiffuse() { return diffuse_fileName; }

        void setTextureDiffuse(aiString target) { diffuse_fileName = target; }

    protected:
        std::vector<Vertex> _vertices;
        std::vector<float> _rawVertices;
        float *_rawVerticesPTR;
        std::vector<unsigned int> _indices;
        aiMaterial *_material;
        aiString diffuse_fileName;
    };
}

#endif //DEMONENGINE_B_MESH_H
