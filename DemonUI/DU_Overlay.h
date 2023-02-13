//
// Created by simon on 10/27/22.
//

#ifndef DEMONENGINE_DU_OVERLAY_H
#define DEMONENGINE_DU_OVERLAY_H

#include <SDL2/SDL.h>
#include <string>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdlrenderer.h>

namespace DemonUI {
    class DU_Overlay {
    public:
        DU_Overlay(SDL_Window *win, SDL_GLContext con);

        virtual ~DU_Overlay();

        int updateEvents(SDL_Event *e);

        void newFrame();

        void render();

        // Render functions:
        void beginWindow(std::string title, bool *target) {
            ImGui::Begin(title.c_str(), target, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
        }

        void endWindow() {
            ImGui::End();
        }


    protected:

    private:
        SDL_Window *uWin;
        SDL_GLContext uContext;
    };
}

#endif //DEMONENGINE_DU_OVERLAY_H
