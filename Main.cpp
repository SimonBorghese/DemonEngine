#include <stdlib.h>
#include <stdio.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include <DemonRender/DR_Shader.h>

#include <DemonGame/Master/Engine.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <DemonRender/DemonLights/DR_DL_BasicLight.h>

#include <math.h>
#define WIDTH 800
#define HEIGHT 600
int main(void)
{
    DemonEngine::Engine engine(WIDTH,HEIGHT);
    engine.createEngine();
    auto *goodSCP = engine.createWorldEntity();
    auto *goodSCP2 = engine.createWorldObject();
    goodSCP->createEntityFromMesh("173.fbx");
    goodSCP2->createEntityFromMesh("bloque.obj", glm::vec3(0.0f, -10.0f, 0.0f));
    DemonPhysics::DP_CharacterController *controller = engine.createFPSController(glm::vec3(0.0f), 5.0f, 1.0f);
    DemonRender::DemonLight::DR_DL_BasicLight goodLight(engine.getObjectShader());
    goodLight.initLight(glm::vec3(1.0f, 1.0f, 1.0f));

    goodLight.renderLight();
    while (!engine.gameLoop()){
        goodLight.renderLight();
        if (engine.getEvent()->getKeyDown(SDL_SCANCODE_E)) {
            auto *goodSCP4 = engine.createWorldEntity();
            goodSCP4->createEntityFromMesh("block.obj", glm::vec3(0.0f));
        }

        glm::vec3 FPSFront = engine.getCamera()->getCameraFront();
        if (!engine.getEvent()->getKey(SDL_SCANCODE_SPACE)) {
            FPSFront.y = 0.0f;
        }

        if (engine.getEvent()->getKey(SDL_SCANCODE_W)){
            controller->move(FPSFront);
        }
        if (engine.getEvent()->getKey(SDL_SCANCODE_S)){
            controller->move(-FPSFront);
        }
        if (engine.getEvent()->getKey(SDL_SCANCODE_A)){
            controller->move(-(glm::normalize(
                    glm::cross(FPSFront, engine.getCamera()->getCameraUp())) * 2.0f));
        }
        if (engine.getEvent()->getKey(SDL_SCANCODE_D)){
            controller->move((glm::normalize(
                    glm::cross(FPSFront, engine.getCamera()->getCameraUp())) * 2.0f));
        }

        if (!engine.getEvent()->getKey(SDL_SCANCODE_SPACE)) {
            controller->move(glm::vec3(0.0f, -9.18f * (1.0f / 60.0f), 0.0f));
        }

    }

    engine.destroyEngine();



    return 0;
}