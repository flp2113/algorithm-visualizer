#include <SDL3/SDL.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "setupimgui.h"
#include "setupsdl.h"
#include "bvector.h"

/*****************************************/
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static SDL_Window* guiWindow = NULL;
static SDL_Renderer* guiRenderer = NULL;

static BVector bvector;
/*****************************************/

void mainLoop(ImGuiIO& io, bool* blockRendering, Sint32* numberOfBlocks, Sint32* delay) {
    bool done = false;

    while (!done) {   
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(guiWindow))
                done = true;
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        if (*blockRendering) {
            bvector.renderBlocks(renderer);
            bvector.renderSortSteps(renderer, *delay);
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // GUI Positions
        ImVec2 fixedPosition(GUI_X, GUI_Y);
        ImGui::SetNextWindowPos(fixedPosition, ImGuiCond_Always);
        ImVec2 fixedWindowSize(GUI_WIDTH, GUI_HEIGHT);
        ImGui::SetNextWindowSize(fixedWindowSize, ImGuiCond_Always);

        //if (showDemoWindow) ImGui::ShowDemoWindow(showDemoWindow);

        {
            ImGui::Begin(GUI_TITLE, nullptr, GUI_WINDOW_FLAGS);   

            ImGui::Checkbox("Start", blockRendering);
            
            if (ImGui::SliderInt("Number of blocks", numberOfBlocks, MIN_BLOCKS_IN_VECTOR, MAX_BLOCKS_IN_VECTOR))                
                bvector.resize(*numberOfBlocks);

            ImGui::SliderInt("Delay", delay, MIN_DELAY, MAX_DELAY);
                
            //ImGui::ColorEdit3("Block color", (float*)&blockColor);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        SDL_RenderClear(guiRenderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), guiRenderer);
        SDL_RenderPresent(guiRenderer);

        //SDL_RenderPresent(renderer);
    }
}

int main(int, char**)
{
    //SETUP SDL
    if (setupSDL(&window, &renderer, &guiWindow, &guiRenderer) == SDL_APP_FAILURE) {
        SDL_Log("ERROR WHILE SETTING UP SDL!");
        return SDL_APP_FAILURE;
    }

    //SETUP DEAR IMGUI, RETURN IO
    ImGuiIO& io = setupDearImGui(guiWindow, guiRenderer);

    // States
    Sint32 delay = 50;
    Sint32 numberOfBlocks = MIN_BLOCKS_IN_VECTOR;
    bool showDemoWindow = false;
    bool blockRendering = false;
    
    // Main Loop
    mainLoop(io, &blockRendering, &numberOfBlocks, &delay);

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(guiRenderer);
    SDL_DestroyWindow(guiWindow);

    SDL_Quit();

    return 0;
}

