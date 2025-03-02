#include "Gui.h"

Gui::Gui(Screen* screen) : screen(screen) { }

ImGuiIO& Gui::setup(int flags) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= flags;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(screen->window, screen->renderer);
    ImGui_ImplSDLRenderer3_Init(screen->renderer);

    return io;
}