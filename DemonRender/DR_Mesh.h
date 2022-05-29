//
// Created by simon on 5/28/22.
//

#ifndef DEMONENGINE_DR_MESH_H
#define DEMONENGINE_DR_MESH_H
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "DR_Shader.h"

namespace DemonRender {

    typedef union{
        glm::vec3 iPosition;
    } Vertex;

    class DR_Mesh {
    public:
        DR_Mesh() {}

        void createMesh(Vertex *vertices, uint32_t vertexCount, uint32_t *indices = NULL, uint32_t indexCount = 0); // Assuming vertices/indices are triangulated

        void renderMesh(DR_Shader *targetShader);

        void destroyMesh();
    private:
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;

        uint32_t _vertexCount = 0;
        uint32_t _indexCount = 0;

    };

} // DemonRender

#endif //DEMONENGINE_DR_MESH_H
