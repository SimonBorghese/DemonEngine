//
// Created by simon on 6/1/22.
//

#ifndef DEMONENGINE_DG_EVENT_H
#define DEMONENGINE_DG_EVENT_H

#include <SDL2/SDL.h>
#include <array>
#include <functional>

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

        int addKeyUpCallback( std::function<void(int)> target) { _keyUpFunc.push_back(target); return _keyUpFunc.size() - 1;}
        int addKeyDownCallback( std::function<void(int)> target) { _keyDownFunc.push_back(target); return _keyDownFunc.size() - 1;}
        int addKeyCallback( std::function<void(int)> target) { _keyFunc.push_back(target); return _keyFunc.size() - 1;}
        void removeKeyUpCallback(int index) { _keyUpFunc.erase(_keyUpFunc.begin() + index); }
        void removeKeyDownCallback(int index) { _keyDownFunc.erase(_keyDownFunc.begin() + index); }
        void removeKeyCallback(int index) { _keyFunc.erase(_keyFunc.begin() + index); }

        SDL_Event* getEvent() { return &e; }

        std::function<bool(SDL_Event*)> callBack;

        void toggleMouseMovements(){
            _enableMouseMovement = !_enableMouseMovement;
        }
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

        std::vector<std::function<void(int)>> _keyUpFunc;
        std::vector<std::function<void(int)>> _keyDownFunc;
        std::vector<std::function<void(int)>> _keyFunc;

        int _enableMouseMovement = 1;

    };

} // DemonGame

#endif //DEMONENGINE_DG_EVENT_H
