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


    DemonPhysics::DP_PhysicsManager physicsManager;
    physicsManager.createPhysics(glm::vec3(0.0f, -9.81f, 0.0f));

    unsigned int outLen;
    DemonBase::b_Mesh **normalMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile("173.fbx", &outLen);
    std::vector<float> copied = normalMesh[0]->getRawVerticesVector();


    DemonPhysics::DP_RigidMesh rigidMesh(normalMesh[0]);
    physicsManager.createRigidMesh(&rigidMesh);

    DemonPhysics::DP_RigidActor rigidActor(&rigidMesh);

    DemonPhysics::DP_PhysicsMaterial matgood;
    matgood.createMaterial(physicsManager.getPhysics());


    rigidActor.createActor(physicsManager.getPhysics(), matgood.getMaterial());

    physicsManager.addActor(&rigidActor);
    //copied.data() = normalMesh[0]->getVerticesVector().data();


    /*
    for (unsigned int v = 0; v < normalMesh[0]->getVerticesVector().size(); v++){
        copied.push_back(normalMesh[0]->getVerticesVector().at(v).iPosition.x);
        copied.push_back(normalMesh[0]->getVerticesVector().at(v).iPosition.y);
        copied.push_back(normalMesh[0]->getVerticesVector().at(v).iPosition.z);
    }
     */

    //physics.addPhysicsObject(&goodrigid);



    renderManager.setCamera(&mainCamera);

    mainEvents.pollEvents();

    //rigidActor.getActor()->setGlobalPose(physx::PxTransform(physx::PxVec3(0.0f, -10.0f, 0.0f)));
    while (!mainEvents.getCloseState()) {
        //glm::vec3 good(0.0f, -10.0f, 0.0f);
        //goodrigid.setVelocity((float*)&good, 10.0f);
        renderManager.newFrame();
        mainEvents.pollEvents();

        basicCameraController.updateCamera();
        //scp173.rotate(glm::vec3(0.0f, 0.1f, 0.0f));
        //float poop[3];
        //goodrigid.getPosition(&poop[0]);
        /*
        printf("Found pos: %f %f %f\n",
               poop[0],
               poop[1],
               poop[2]);
               */
        //glm::vec3 posFound;
        //goodrigid.getPosition((float*) &posFound);
        //printf("G: %f %f %f\n", posFound.x, posFound.y, posFound.z);
        physx::PxVec3 goodPos = rigidActor.getActor()->getGlobalPose().p;
        printf("Pos: %f %f %f\n", goodPos.x, goodPos.y, goodPos.z);

        renderManager.render();
        physicsManager.simulate(1.0f/60.0f);
    }


    mainShader.destroyProgram();
    scp173.destroyEntity();
    rigidMesh.destroyMesh();
    physicsManager.removeActor(&rigidActor);
    rigidActor.destroyActor();
    matgood.destroyMaterial();
    physicsManager.closePhysics();
    renderManager.destroyRenderer();



    return 0;
}