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
#include <DemonPhysics/DP_RigidActor.h>
#include <DemonPhysics/DP_PhysicsMaterial.h>
#include <DemonPhysics/DP_RigidMesh.h>
#include <DemonPhysics/DP_RigidPhysicsActor.h>
#include <DemonIO/DI_SceneLoader.h>

#define WIDTH 800
#define HEIGHT 600
int main(void)
{

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


    DemonPhysics::DP_PhysicsManager physicsManager;
    physicsManager.createPhysics(glm::vec3(0.0f, -9.81f, 0.0f));

    unsigned int outLen;
    DemonBase::b_Mesh **normalMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile("173.fbx", &outLen);
    std::vector<float> copied = normalMesh[0]->getRawVerticesVector();


    DemonPhysics::DP_RigidMesh rigidMesh(normalMesh[0]);
    DemonPhysics::DP_RigidPhysicsActor rigidActor(&rigidMesh);
    DemonPhysics::DP_PhysicsMaterial matgood;

    physicsManager.cookMesh(&rigidMesh);
    physicsManager.cookMaterial(&matgood);
    physicsManager.cookActor(&rigidActor, &matgood);


    physicsManager.addActor(&rigidActor);

    renderManager.setCamera(&mainCamera);

    mainEvents.pollEvents();

    while (!mainEvents.getCloseState()) {

        renderManager.newFrame();
        mainEvents.pollEvents();

        basicCameraController.updateCamera();


        glm::vec3 goodPos = rigidActor.getTransform()->getPosition();
        glm::quat goodRot = rigidActor.getTransform()->getRotation();
        scp173.getTransform()->setPosition(goodPos);
        scp173.getTransform()->setRotation(goodRot);
        printf("Pos: %f %f %f\n", goodPos.x, goodPos.y, goodPos.z);

        renderManager.render();
        physicsManager.simulate(1.0f/60.0f);
        rigidActor.updateActor();
    }


    mainShader.destroyProgram();
    scp173.destroyEntity();
    physicsManager.removeActor(&rigidActor);
    rigidMesh.destroyMesh();
    matgood.destroyMaterial();
    rigidActor.destroyActor();
    physicsManager.closePhysics();
    renderManager.destroyRenderer();



    return 0;
}