//
// Created by simon on 5/28/22.
//

#define STB_IMAGE_IMPLEMENTATION
#include "DR_Mesh.h"

namespace DemonRender {

    DR_Mesh::DR_Mesh(Vertex *vertices, unsigned int vertexLen, unsigned int *indices, unsigned int indexLen) : DemonBase::b_Mesh(vertices, vertexLen, indices, indexLen) {
        //DemonBase::b_Mesh(vertices, vertexLen, indices, indexLen);
        assert( !((indexLen == 0 && vertexLen % 3 != 0) || (indexLen != 0 && indexLen % 3 != 0)) ); // Not Triangulated

        glCreateVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertexLen * sizeof(Vertex), vertices, GL_STATIC_DRAW);

        if (indexLen != 0) {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLen * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(float)*3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        _vertexCount = vertexLen;
        _indexCount = indexLen;


    }

    DR_Mesh::DR_Mesh(DemonBase::b_Mesh *targetMesh) :
    DemonBase::b_Mesh(targetMesh->getVerticesVector().data(),
                      targetMesh->getVerticesVector().size(),
                      targetMesh->getIndicesVector().data(),
                      targetMesh->getIndicesVector().size()){
        unsigned int indexLen = targetMesh->getIndicesVector().size();
        unsigned int vertexLen = targetMesh->getVerticesVector().size();
        setMaterial(targetMesh->getMaterial());
        setTextureDiffuse(targetMesh->getTextureDiffuse());
        assert( !((indexLen == 0 && vertexLen % 3 != 0) || (indexLen != 0 && indexLen % 3 != 0)) ); // Not Triangulated

        glCreateVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, (vertexLen) * sizeof(Vertex), targetMesh->getVerticesVector().data(), GL_STATIC_DRAW);

        if (indexLen != 0) {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLen * sizeof(uint32_t), targetMesh->getIndicesVector().data(), GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(float)*3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        _vertexCount = vertexLen;
        _indexCount = indexLen;
    }
    /*
    void DR_Mesh::createMesh(Vertex *vertices, uint32_t vertexCount, uint32_t *indices, uint32_t indexCount){
        assert( !((indexCount == 0 && vertexCount % 3 != 0) || (indexCount != 0 && indexCount % 3 != 0)) ); // Not Triangulated

        glCreateVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

        if (indexCount != 0) {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(float)*3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        _vertexCount = vertexCount;
        _indexCount = indexCount;

    }
    */

    void DR_Mesh::renderMesh(){
        glBindVertexArray(VAO);

        if (EBO != 0){
            glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
        }
        else{
            glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
        }



    }

    void DR_Mesh::createTextureFromSTB(const char *fileName, bool allowAlpha) {
        glGenTextures(1, &TextureBuffer);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureBuffer);


        stbi_set_flip_vertically_on_load(1);

        int width, height, nrChannels;
        fmt::print("Found file: {}\n", fileName);
        void* data = stbi_load(fileName, &width, &height, &nrChannels, 0);

        GLenum targetFormat = GL_RGB;
        if (allowAlpha){
            switch (nrChannels){
                case 3:
                    targetFormat = GL_RGB;
                    break;
                case 4:
                    targetFormat = GL_RGBA;
                    break;
                default:
                    targetFormat = GL_RGB;
                    break;
            }
        }


        if (!data){
            (fmt::print("Failed to load image: {}\n", fileName));
            data = stbi_load("data/missing.png", &width, &height, &nrChannels, 3);
            if (!data) {
                fmt::print("CRITICAL ERROR: UN-ABLE TO LOAD ESSENTIAL FILE\n");
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, targetFormat, width, height, 0, targetFormat, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    void DR_Mesh::destroyMesh(){
        if (TextureBuffer != 0){
            glDeleteTextures(1, &TextureBuffer);
        }
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
} // DemonRender