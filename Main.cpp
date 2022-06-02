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
#include <DemonGame/Shared/DG_Entity.h>
#include <DemonWorld/DW_Transform.h>

#define WIDTH 800
#define HEIGHT 600
int main(void)
{
    DemonRender::DR_RenderManager renderManager;
    DemonRender::DR_Shader mainShader;
    DemonRender::DR_Camera mainCamera(&mainShader);
    DemonGame::DG_Event mainEvents;
    DemonGame::DG_BasicCameraController basicCameraController(&mainEvents, &mainCamera, 0.2f);

    DemonGame::DG_Entity scp173(&renderManager, &mainShader);

    renderManager.createRenderer("OwO", WIDTH, HEIGHT);
    mainShader.createProgram("DemonShaders/vertex_noAnim.glsl", "DemonShaders/frag_colourDebug.glsl");
    mainCamera.configureProjection(70.0f, (float)WIDTH/(float)HEIGHT , 0.1f, 100.0f);
    scp173.createEntityFromMesh("173.fbx");

    renderManager.setCamera(&mainCamera);

    mainEvents.pollEvents();
    while (!mainEvents.getCloseState()) {
        renderManager.newFrame();
        mainEvents.pollEvents();

        basicCameraController.updateCamera();
        scp173.getTransform()->rotate(glm::vec3(0.0f, 0.1f, 0.0f));

        renderManager.render();
    }


    mainShader.destroyProgram();
    scp173.destroyEntity();
    renderManager.destroyRenderer();



    return 0;
}