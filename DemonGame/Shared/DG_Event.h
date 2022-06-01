//
// Created by simon on 6/1/22.
//

#ifndef DEMONENGINE_DG_EVENT_H
#define DEMONENGINE_DG_EVENT_H
#include <SDL2/SDL.h>
#include <array>
#define INPUT_KEYS_LEN 255

namespace DemonGame {
    class DG_Event {
    public:
        DG_Event();
        virtual ~DG_Event();

        void pollEvents();

        int getKeyDown(uint32_t scancode);
        int getKey(uint32_t scancode);
        int getKeyUp(uint32_t scancode);

        int getMouseX();
        int getMouseY();
        int getMouseXOffset();
        int getMouseYOffset();

        int getCloseState();
    private:
        SDL_Event e;

        std::array<uint8_t, INPUT_KEYS_LEN> keysDown;
        std::array<uint8_t, INPUT_KEYS_LEN> keysActive;
        std::array<uint8_t, INPUT_KEYS_LEN> keysUp;

        int getKillState;

        int mouseX = 0;
        int mouseY = 0;
        int offsetMouseX = 0;
        int offsetMouseY = 0;
    };

} // DemonGame

#endif //DEMONENGINE_DG_EVENT_H
