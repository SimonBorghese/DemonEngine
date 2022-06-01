//
// Created by simon on 5/28/22.
//

#ifndef DEMONENGINE_DR_MESH_H
#define DEMONENGINE_DR_MESH_H
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "DR_Shader.h"
#include <STB_IMAGE/stb_image.h>
#include <fmt/core.h>
#include <fmt/printf.h>


namespace DemonRender {

    typedef struct {
        glm::vec3 iPosition;
        glm::vec2 iTextCord;
    } Vertex;

    class DR_Mesh {
    public:
        DR_Mesh() {}

        void createMesh(Vertex *vertices, uint32_t vertexCount, uint32_t *indices = NULL, uint32_t indexCount = 0); // Assuming vertices/indices are triangulated

        void renderMesh();

        void destroyMesh();

        void createTextureFromSTB(const char *fileName, bool allowAlpha);

        GLuint getTexture() { return TextureBuffer; }
        GLuint* getTexturePTR() { return &TextureBuffer; }
    private:
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;

        GLuint TextureBuffer = 0;

        uint32_t _vertexCount = 0;
        uint32_t _indexCount = 0;

    };

} // DemonRender

#endif //DEMONENGINE_DR_MESH_H
