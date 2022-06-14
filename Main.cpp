#include <stdlib.h>
#include <stdio.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include <DemonRender/DR_Shader.h>

#include <DemonGame/Master/Engine.h>
#include <DemonPhysics/DP_CharacterController.h>

#define WIDTH 800
#define HEIGHT 600
int main(void)
{
    DemonEngine::Engine engine(WIDTH,HEIGHT);
    engine.createEngine();
    auto *goodSCP = engine.createWorldEntity();
    auto *goodSCP2 = engine.createWorldObject();
    goodSCP->createEntityFromMesh("173.fbx");
    goodSCP2->createEntityFromMesh("bloque.obj", glm::vec3(0.0f, -20.0f, 0.0f));
    DemonPhysics::DP_PhysicsMaterial mat;
    mat.createMaterial(engine.getPhysicsManager()->getPhysics());
    DemonPhysics::DP_CharacterController controller(glm::vec3(0.0f), mat.getMaterial(), 1.5f, 1.0f, engine.getPhysicsManager()->getControllerManager());

    while (!engine.gameLoop()){
        glm::vec3 goody = glm::vec3(0.0f);
        controller.getPosition(&goody);
        printf("%f %f %f\n", goody.x, goody.y, goody.z);

        controller.move(glm::vec3(0.0f, -0.0981f, 0.0f));

    }

    engine.destroyEngine();



    return 0;
}