#include <stdlib.h>
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext.hpp>

#include <DemonRender/DR_Shader.h>

#include <DemonGame/Master/Engine.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <DemonRender/DemonLights/DR_DL_BasicLight.h>
#include <DemonGame/Shared/External/DG_EXT_PhysicsCallback.h>
#include <DemonIO/DI_BSPLoader.h>

#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define SPEED 30
#define JUMP_HEIGHT 0.4f
#define DEFAULT_GRAVITY -9.81f
float gravity = DEFAULT_GRAVITY;

DemonEngine::Engine *engine;
DemonPhysics::DP_CharacterController *controller;
glm::vec3 FPSFront = glm::vec3(0.0f);

void keyDownCallback(int scancode){
    switch (scancode){
        case SDL_SCANCODE_E:
            engine->createWorldEntity()->createEntityFromMesh("block.obj", glm::vec3(0.0f));
            break;
        case SDL_SCANCODE_SPACE:
            if (controller->onGround()){
                gravity = DEFAULT_GRAVITY * -JUMP_HEIGHT;
            }
            break;
        default:
            break;
    }
}

void keyCalback(int scancode){
    FPSFront = engine->getCamera()->getCameraFront();
    //FPSFront.y = 0.0f;
    switch (scancode){
        case SDL_SCANCODE_W:
            controller->move(FPSFront * engine->getDeltaTime() * SPEED);
            break;
        case SDL_SCANCODE_S:
            controller->move(-FPSFront * engine->getDeltaTime() * SPEED);
            break;
        case SDL_SCANCODE_A:
            controller->move(-(glm::normalize(
                    glm::cross(FPSFront, engine->getCamera()->getCameraUp())) * 2.0f) * engine->getDeltaTime() * SPEED);
            break;
        case SDL_SCANCODE_D:
            controller->move((glm::normalize(
                    glm::cross(FPSFront, engine->getCamera()->getCameraUp())) * 2.0f) * engine->getDeltaTime() * SPEED);
            break;
        default:
            break;
    }
}

void SCPCollideCallback(DemonGame::DG_PhysicsObject* other){
    printf("173 just touched: %s\n", other->getName().c_str());
    //other->getActor()->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
    other->getActor()->applyForce(glm::vec3(0.0f, 20.0f, 0.0f));
}

int main(void) {
    // Initialize the engine
    engine = new DemonEngine::Engine(WIDTH, HEIGHT);
    // Create the engine elements
    engine->createEngine();

    auto map = engine->createVisualEntity();
    map->createEntityFromMesh("test.bsp", glm::vec3(10.0f, -4.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f/8.0f));

    // Create a physics object (world entity) & static object (world object)
    auto scpthing = engine->createWorldEntity();
    scpthing->createEntityFromMesh("173.fbx");
    // Set the name of the object
    scpthing->setName("173");
    // Set the collision callback function
    scpthing->setContactCallback(SCPCollideCallback);
    scpthing->setMass(75.0f);
    scpthing->setSpaceMassInertiaTensor(glm::vec3(2000.0f));
    scpthing->updateMassInertia(5.0f);

    // Create a world static object
    engine->createWorldObject()->createEntityFromMesh("bloque.obj", glm::vec3(0.0f, -10.0f, 0.0f));

    // Create the FPS controller
    controller = engine->createFPSController(glm::vec3(0.0f), 5.0f, 1.0f);
    

    // Create a light
    DemonRender::DemonLight::DR_DL_BasicLight *light1 = engine->createEasyPointLight(glm::vec3(0.0f, -2.0f, 0.0f), 500.0f, 1.0f);

    // Set the key callback functions
    engine->getEvent()->setKeyDownCallback(keyDownCallback);
    engine->getEvent()->setKeyCallback(keyCalback);


    // Main game loop
    while (!engine->gameLoop()) {
        //controller->move(glm::vec3(0.0f, gravity * engine->getDeltaTime(), 0.0f));
        if (controller->onGround() && gravity != DEFAULT_GRAVITY){
            gravity = DEFAULT_GRAVITY;
        }
        else if (gravity > DEFAULT_GRAVITY){
            gravity += DEFAULT_GRAVITY * engine->getDeltaTime();
        }
        light1->getLightInfo()->position = controller->getPosition();
    }

    engine->destroyEngine();
    delete engine;

    return 0;
}