//
// Created by simon on 5/28/22.
//

#include "DR_Mesh.h"

namespace DemonRender {
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        glEnableVertexAttribArray(0);

        _vertexCount = vertexCount;
        _indexCount = indexCount;

    }

    void DR_Mesh::renderMesh(DR_Shader *targetShader){
        targetShader->useProgram();
        glBindVertexArray(VAO);

        if (EBO != 0){
            glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
        }
        else{

            glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
        }



    }

    void DR_Mesh::destroyMesh(){
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
} // DemonRender