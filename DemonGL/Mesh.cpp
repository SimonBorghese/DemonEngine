//
// Created by simon on 1/10/23.
//

#include "Mesh.h"

namespace DGL {
    int Mesh::_enableOcculusion = 0;
    Mesh::Mesh(DemonBase::b_Mesh mesh) : _bMesh(mesh){
        _primaryTexture = new Texture(mesh.getTextureDiffuse().C_Str(), mesh.getTextureNormal().C_Str());
    }
    Mesh::Mesh(std::vector<Vertex> vertex, std::vector<uint32_t> indice, std::string diffuse, std::string normal) :
            _bMesh(vertex.data(), vertex.size(), indice.data(), indice.size()){
        _primaryTexture = new Texture(diffuse.c_str(), normal.c_str());
    }
    Mesh::~Mesh() = default;

    void Mesh::createMesh(){
        GLuint VBO, EBO;
        _numElements = _bMesh.getIndexLength();
        glCreateVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        /*
         * typedef struct {
    glm::vec3 iPosition;
    glm::vec3 iNormal;
    glm::vec2 iTextCord;
    glm::ivec4 iBoneIds;
    glm::vec4 weights;
    int animationBody;
} Vertex;
         */

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, (long) sizeof(Vertex) * _bMesh.getVertexLength(), _bMesh.getVertices(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) sizeof(uint32_t) * _bMesh.getIndexLength(), _bMesh.getIndices(), GL_STATIC_DRAW);


        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(float) * 3));
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(float) * 6));
        glVertexAttribIPointer(3, 4, GL_INT,             sizeof(Vertex), (void *) (sizeof(float) * 8));
        glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) ((sizeof(float) * 8) + (sizeof(int) * 4)));
        glVertexAttribIPointer(5, 1, GL_INT,             sizeof(Vertex), (void *) ((sizeof(float) * 12) + (sizeof(int) * 4)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);

        _primaryTexture->createTexture();

    }
    void Mesh::renderMesh(){
        /*
        GLuint query[1];
        if (_enableOcculusion){
            glGenQueries(1, query);
            glBeginQuery(GL_SAMPLES_PASSED, query[0]);
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (int) _numElements, GL_UNSIGNED_INT, nullptr);
        if (_enableOcculusion){
            glEndQuery(GL_SAMPLES_PASSED);
            glGetQueryObjectuiv(query[0], GL_QUERY_RESULT, &numSamples);
            if (_displaySamples){
                printf("Num samples: %d\n", numSamples);
            }

        }
         */
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (int) _numElements, GL_UNSIGNED_INT, nullptr);
        _displaySamples = 1;
    }
    void Mesh::destroyMesh(){
        glDeleteVertexArrays(1, &VAO);
    }
} // DGL