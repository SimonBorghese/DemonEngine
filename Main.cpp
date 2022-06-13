#include <stdlib.h>
#include <stdio.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include <DemonRender/DR_Shader.h>

#include <DemonGame/Master/Engine.h>

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

    while (!engine.gameLoop()){
    }

    engine.destroyEngine();



    return 0;
}