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
#include <DemonGame/Shared/DG_RigidEntity.h>
#include <DemonGame/Shared/DG_PhysicsObject.h>

#include <DemonGame/Master/Engine.h>

#define WIDTH 800
#define HEIGHT 600
int main(void)
{
    DemonEngine::Engine engine(WIDTH,HEIGHT);
    engine.createEngine();

    DemonGame::DG_RigidEntity goodSCP(engine.getRenderingManager(), engine.getObjectShader(), engine.getPhysicsManager());
    goodSCP.createEntityFromMesh("bloque.obj", glm::vec3(0.0f, -20.0f, 0.0f));

    DemonGame::DG_PhysicsObject goodSCP2(engine.getRenderingManager(), engine.getObjectShader(), engine.getPhysicsManager());
    goodSCP2.createEntityFromMesh("173.fbx", glm::vec3(0.0f, 0.0f, 0.0f));


    while (!engine.gameLoop()){
        goodSCP.update();
        goodSCP2.update();
    }

    engine.destroyEngine();



    return 0;
}