//
// Created by simon on 6/6/22.
//

#ifndef DEMONENGINE_B_MESH_H
#define DEMONENGINE_B_MESH_H
#include <vector>
#include <glm/glm.hpp>
#include <assimp/material.h>


typedef struct {
    glm::vec3 iPosition;
    glm::vec2 iTextCord;
} Vertex;

namespace DemonBase {

    class b_Mesh {
    public:
        b_Mesh(Vertex *vertices, unsigned int vertexLen, unsigned int *indices, unsigned int indexLen){
            std::copy(&vertices[0], &vertices[vertexLen], std::back_inserter(_vertices));
            if (indexLen > 0) {
                std::copy(&indices[0], &indices[indexLen], std::back_inserter(_indices));
            }
        }

        std::vector<Vertex> getVerticesVector() { return _vertices; }
        std::vector<unsigned int> getIndicesVector() { return _indices; }

        Vertex* getVertices(unsigned int *outputLen) {
            *outputLen = _vertices.size();
            return _vertices.data();
        }
        unsigned int* getIndices(unsigned int *outputLen) {
            *outputLen = _indices.size();
            return _indices.data();
        }
        void setMaterial(aiMaterial *mat) { _material = mat; }
        aiMaterial* getMaterial() { return _material; }
        aiString getTextureDiffuse() { return diffuse_fileName; }
        void setTextureDiffuse(aiString target) { diffuse_fileName = target; }

    protected:
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;
        aiMaterial *_material;
        aiString diffuse_fileName;
    };
}

#endif //DEMONENGINE_B_MESH_H
