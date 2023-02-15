//
// Created by simon on 1/10/23.
//

#ifndef DEMONENGINE_MESH_H
#define DEMONENGINE_MESH_H
#include <glad/glad.h>
#include <DemonBase/b_Mesh.h>
#include <vector>
#include "Texture.h"

namespace DGL {

    class Mesh {
    public:
        explicit Mesh(DemonBase::b_Mesh mesh);
        Mesh(std::vector<Vertex> vertex, std::vector<uint32_t> indice, std::string diffuse, std::string normal = "");
        virtual ~Mesh();

        void createMesh();
        void renderMesh();
        void destroyMesh();

        Texture* getTexture() { return _primaryTexture; }
        GLuint getNumSamples() const { return numSamples; }
        static int _enableOcculusion;
        int _displaySamples = 0;

    private:
        DemonBase::b_Mesh _bMesh;

        std::vector<Vertex> _vertices;
        std::vector<uint32_t> _indices;

        Texture *_primaryTexture;

        GLuint VAO = 0;
        uint32_t _numElements = 0;

        GLuint numSamples = 1;
        

    };

} // DGL

#endif //DEMONENGINE_MESH_H
