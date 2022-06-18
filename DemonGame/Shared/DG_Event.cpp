//
// Created by simon on 6/1/22.
//

#include "DG_Event.h"

namespace DemonGame {

    DG_Event::DG_Event() {
        keysDown.fill(0);
        keysActive.fill(0);
        keysUp.fill(0);
    }

    DG_Event::~DG_Event() {}

    void DG_Event::pollEvents() {
        keysDown.fill(0);
        keysUp.fill(0);
        getKillState = 0;
        offsetMouseY = 0;
        offsetMouseX = 0;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    getKillState = 1;
                    break;
                case SDL_MOUSEMOTION:
                    mouseX = e.motion.x;
                    mouseY = e.motion.y;
                    offsetMouseX = e.motion.xrel;
                    offsetMouseY = e.motion.yrel;
                    break;
                case SDL_KEYUP:
                    keysActive[e.key.keysym.scancode] = 0;
                    keysUp[e.key.keysym.scancode] = 1;
                    break;
                case SDL_KEYDOWN:
                    keysActive[e.key.keysym.scancode] = 1;
                    keysDown[e.key.keysym.scancode] = 1;
                    break;
                default:
                    break;
            }
        }
    }

    int DG_Event::getKeyDown(uint32_t scancode) {
        return keysDown[scancode];
    }

    int DG_Event::getKey(uint32_t scancode) {
        return keysActive[scancode];
    }

    int DG_Event::getKeyUp(uint32_t scancode) {
        return keysUp[scancode];
    }

    int DG_Event::getMouseX() {
        return mouseX;
    }

    int DG_Event::getMouseY() {
        return mouseY;
    }

    int DG_Event::getMouseXOffset() {
        return offsetMouseX;
    }

    int DG_Event::getMouseYOffset() {
        return offsetMouseY;
    }

    int DG_Event::getCloseState() {
        return getKillState;
    }
} // DemonGame