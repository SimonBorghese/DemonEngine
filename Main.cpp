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

DemonEngine::Engine *engine;
DemonPhysics::DP_CharacterController *controller;
glm::vec3 FPSFront = glm::vec3(0.0f);

void keyDownCallback(int scancode){
    switch (scancode){
        case SDL_SCANCODE_E:
            engine->createWorldEntity()->createEntityFromMesh("block.obj", glm::vec3(0.0f));
            break;
        default:
            break;
    }
}

void keyCalback(int scancode){
    FPSFront = engine->getCamera()->getCameraFront();
    FPSFront.y = 0.0f;
    switch (scancode){
        case SDL_SCANCODE_W:
            controller->move(FPSFront);
            break;
        case SDL_SCANCODE_S:
            controller->move(-FPSFront);
            break;
        case SDL_SCANCODE_A:
            controller->move(-(glm::normalize(
                    glm::cross(FPSFront, engine->getCamera()->getCameraUp())) * 2.0f));
            break;
        case SDL_SCANCODE_D:
            controller->move((glm::normalize(
                    glm::cross(FPSFront, engine->getCamera()->getCameraUp())) * 2.0f));
            break;
        default:
            break;
    }
}

int main(void) {
    // Initialize the engine
    engine = new DemonEngine::Engine(WIDTH, HEIGHT);
    // Create the engine elements
    engine->createEngine();
    // Create a physics object (world entity) & static object (world object)
    engine->createWorldEntity()->createEntityFromMesh("173.fbx");
    engine->createWorldObject()->createEntityFromMesh("bloque.obj", glm::vec3(0.0f, -10.0f, 0.0f));
    // Create the FPS controller
    controller = engine->createFPSController(glm::vec3(0.0f), 5.0f, 1.0f);

    // Create a spotlight
    engine->createEasySpotLight(glm::vec3(0.0f, 5.0f, 0.0f),
                               glm::vec3(0.0f, -10.0f, 0.0f), 45.0f, 70.0f, 0.5f);
    // Set the key callback functions
    engine->getEvent()->setKeyDownCallback(keyDownCallback);
    engine->getEvent()->setKeyCallback(keyCalback);

    // Main game loop
    while (!engine->gameLoop()) {
        controller->move(glm::vec3(0.0f, -9.18f * (1.0f / 60.0f), 0.0f));
    }

    engine->destroyEngine();
    delete engine;

    return 0;
}