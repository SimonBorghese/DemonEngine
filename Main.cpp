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
#include <DemonPhysics/DP_PhysicsManager.h>
#include <DemonIO/DI_SceneLoader.h>

#define WIDTH 800
#define HEIGHT 600
int main(void)
{
    //int *good = (int*) 0x01;
    //*good = 4834534;
    //char owo[3];
    //scanf("%s", &owo[0]);

    DemonRender::DR_RenderManager renderManager;
    renderManager.createRenderer("OwO", WIDTH, HEIGHT);

    DemonRender::DR_Shader mainShader;
    mainShader.createProgram("DemonShaders/vertex_noAnim.glsl", "DemonShaders/frag_colourDebug.glsl");

    DemonRender::DR_Camera mainCamera(&mainShader);
    mainCamera.configureProjection(70.0f, (float)WIDTH/(float)HEIGHT , 0.1f, 100.0f);

    DemonGame::DG_Event mainEvents;
    DemonGame::DG_BasicCameraController basicCameraController(&mainEvents, &mainCamera, 0.5f);
    DemonGame::DG_Entity scp173(&renderManager, &mainShader);
    scp173.createEntityFromMesh("173.fbx");

    DemonPhysics::DP_PhysicsManager physics;
    physics.createPhysics(glm::vec3(0.0f, -9.81f, 0.0f));

    //physics.createRigidMesh(scp173.getMeshRenderer()->goodMesh);



    renderManager.setCamera(&mainCamera);

    mainEvents.pollEvents();
    while (!mainEvents.getCloseState()) {
        renderManager.newFrame();
        mainEvents.pollEvents();

        basicCameraController.updateCamera();
        scp173.rotate(glm::vec3(0.0f, 0.1f, 0.0f));

        renderManager.render();
    }


    mainShader.destroyProgram();
    scp173.destroyEntity();
    physics.closePhysics();
    renderManager.destroyRenderer();



    return 0;
}