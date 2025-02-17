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

static int numberOfBlocks = MIN_BLOCKS_IN_VECTOR;
static GZBlock::BVector bvector(numberOfBlocks);
/*****************************************/

void mainLoop(ImGuiIO& io, bool* blockRendering, int* numberOfBlocks) {
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
            bvector.renderSortSteps(renderer);
        }
            
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImVec2 fixedPosition(GUI_X, GUI_Y);
        ImGui::SetNextWindowPos(fixedPosition, ImGuiCond_Always);
        ImVec2 fixedWindowSize(GUI_WIDTH, GUI_HEIGHT);
        ImGui::SetNextWindowSize(fixedWindowSize, ImGuiCond_Always);

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (show_demo_window)
            //ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin(GUI_TITLE, nullptr, GUI_WINDOW_FLAGS);   

            ImGui::Checkbox("Start", blockRendering);
            
            if (ImGui::SliderInt("Number of blocks", numberOfBlocks, MIN_BLOCKS_IN_VECTOR, MAX_BLOCKS_IN_VECTOR)) {                
                bvector.setSize(*numberOfBlocks);
            }

            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                //counter++;
            //ImGui::SameLine();
            //ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        //SDL_RenderSetScale(guiRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        //SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
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
    //bool show_demo_window = false;
    //bool show_another_window = false;
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool blockRendering = false;

    // Main Loop
    mainLoop(io, &blockRendering, &numberOfBlocks);

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