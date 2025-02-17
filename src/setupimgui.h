#pragma once

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>

ImGuiIO& setupDearImGui(SDL_Window* guiWindow, SDL_Renderer* guiRenderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(guiWindow, guiRenderer);
    ImGui_ImplSDLRenderer3_Init(guiRenderer);

    return io;
}
