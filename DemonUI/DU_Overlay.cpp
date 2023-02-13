//
// Created by simon on 10/27/22.
//

#include "DU_Overlay.h"

namespace DemonUI {
    DU_Overlay::DU_Overlay(SDL_Window *win, SDL_GLContext con) : uWin(win), uContext(con)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        ImGui_ImplSDL2_InitForOpenGL(uWin, uContext);
        //SDL_Renderer* newRenderer = SDL_CreateRenderer(uWin, 0,  SDL_RENDERER_SOFTWARE);
        //printf("%s\n", SDL_GetError());
        //assert(newRenderer);
        //ImGui_ImplSDL2_InitForSDLRenderer(uWin, newRenderer);
        ImGui_ImplOpenGL3_Init("#version 460 core");
        //ImGui_ImplSDLRenderer_Init(newRenderer);

    }

    DU_Overlay::~DU_Overlay()
    {
        //dtor
    }

    int DU_Overlay::updateEvents(SDL_Event *e){
        return ImGui_ImplSDL2_ProcessEvent(e);
    }


    void DU_Overlay::newFrame(){
        ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

    }
    void DU_Overlay::render(){
        ImGui::Render();
        //ImGui_ImplSDL
        //ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

} // DemonUI