#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <DemonRender/DR_RenderManager.h>
#include <DemonRender/DR_Shader.h>
#include <DemonRender/DR_Mesh.h>

int main(void)
{
    DemonRender::DR_RenderManager renderManager;
    DemonRender::DR_Mesh mainMesh;
    DemonRender::DR_Shader mainShader;
    renderManager.createRenderer("OwO", 800, 600);
    mainShader.createProgram("DemonShaders/vertex_noAnim.glsl", "DemonShaders/frag_colourDebug.glsl");

    DemonRender::Vertex vertices[3];
    vertices[0].iPosition.x = 0.0f;
    vertices[0].iPosition.y = 0.0f;
    vertices[0].iPosition.z = 0.0f;


    vertices[1].iPosition.x = 1.0f;
    vertices[1].iPosition.y = 0.0f;
    vertices[1].iPosition.z = 0.0f;

    vertices[2].iPosition.x = 0.5f;
    vertices[2].iPosition.y = 1.0f;
    vertices[2].iPosition.z = 0.0f;

    mainMesh.createMesh(&vertices[0], 3);

    renderManager.newFrame();
    mainMesh.renderMesh(&mainShader);
    renderManager.render();


    getchar();

    mainShader.destroyProgram();
    mainMesh.destroyMesh();
    renderManager.destroyRenderer();



    return 0;
}