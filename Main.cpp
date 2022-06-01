#include <stdlib.h>
#include <stdio.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include <string>
#include <DemonRender/DR_RenderManager.h>
#include <DemonRender/DR_Shader.h>
#include <DemonRender/DR_Mesh.h>
#include <DemonRender/DR_MeshRenderer.h>
#include <DemonRender/DR_Camera.h>
#include <DemonGame/Shared/DG_Event.h>
#include <DemonGame/Shared/DG_BasicCameraController.h>

#define WIDTH 800
#define HEIGHT 600
int main(void)
{
    DemonRender::DR_RenderManager renderManager;
    DemonRender::DR_Shader mainShader;
    DemonRender::DR_MeshRenderer triangleRenderer;
    DemonRender::DR_Camera mainCamera(&mainShader);
    DemonGame::DG_Event mainEvents;
    DemonGame::DG_BasicCameraController basicCameraController(&mainEvents, &mainCamera);

    renderManager.createRenderer("OwO", WIDTH, HEIGHT);
    mainShader.createProgram("DemonShaders/vertex_noAnim.glsl", "DemonShaders/frag_colourDebug.glsl");

    mainCamera.configureProjection(70.0f, (float)WIDTH/(float)HEIGHT , 0.1f, 100.0f);




    triangleRenderer.setShader(&mainShader);

    mainShader.useProgram();

    uint32_t modelLoc = mainShader.getUniformLocation("model");

    glm::mat4 target = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -50.0f));


    triangleRenderer.loadMeshFromFile("173.fbx");

    renderManager.setCamera(&mainCamera);
    renderManager.addMeshGroup(&triangleRenderer);



    mainEvents.pollEvents();
    while (!mainEvents.getCloseState()) {
        renderManager.newFrame();
        mainEvents.pollEvents();
        basicCameraController.updateCamera();

        target = glm::rotate(target, glm::radians(5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        mainShader.bindMatrix4f(modelLoc, target);
        renderManager.render();
    }


    mainShader.destroyProgram();
    triangleRenderer.destroyMeshes();
    renderManager.destroyRenderer();



    return 0;
}