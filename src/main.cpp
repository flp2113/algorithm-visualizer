#include <SDL3/SDL.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "./BVector/BVector.h"
#include "./Screen/Screen.h"
#include "./Gui/Gui.h"

void mainLoop(Screen mainScreen, Screen guiScreen, BVector bvector, ImGuiIO& io, bool* blockRendering, Sint32* numberOfBlocks, Color targetedBlockColor,
            Color backgroundColor, Color blocksColor, Sint32* delay) {
    bool done = false;

    while (!done) {   
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(mainScreen.window))
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(guiScreen.window))
                done = true;
        }

        if (SDL_GetWindowFlags(mainScreen.window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }

        // Rendering Algorithm
        SDL_SetRenderDrawColorFloat(mainScreen.renderer, backgroundColor.x, backgroundColor.y, backgroundColor.z, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(mainScreen.renderer);

        if (*blockRendering) {
            bvector.renderBlocks(mainScreen.renderer);
            bvector.renderSortSteps(mainScreen.renderer);
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

        {
            ImGui::Begin(GUI_TITLE, nullptr, GUI_WINDOW_FLAGS);   

            ImGui::Checkbox("Start", blockRendering);
            
            if (ImGui::SliderInt("Number of blocks", numberOfBlocks, MIN_BLOCKS_IN_VECTOR, MAX_BLOCKS_IN_VECTOR))                
                bvector.resize(*numberOfBlocks);

            ImGui::SliderInt("Delay", delay, MIN_DELAY, MAX_DELAY);
                
            if (ImGui::ColorEdit3("Block color", (float*)&blocksColor))
                bvector.setColor(blocksColor, BLOCKS_COLOR);

            if (ImGui::ColorEdit3("Background color", (float*)&backgroundColor))
                bvector.setColor(backgroundColor, BACKGROUND_COLOR);

            if (ImGui::ColorEdit3("Targeted block color", (float*)&targetedBlockColor))
                bvector.setColor(targetedBlockColor, TARGETED_BLOCK_COLOR);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendering GUI
        ImGui::Render();
        SDL_RenderClear(guiScreen.renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), guiScreen.renderer);
        SDL_RenderPresent(guiScreen.renderer);

        SDL_Delay(*delay);
    }
}

int main(int, char**)
{
    // INIT IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // INIT SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // SETUP SCREENS
    Screen mainScreen = Screen(
        WINDOW_TITLE,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_FLAGS
    );

    Screen guiScreen = Screen(
        GUI_TITLE,
        GUI_WIDTH,
        GUI_HEIGHT,
        GUI_WINDOW_X,
        GUI_WINDOW_Y,
        GUI_FLAGS
    );

    // SETUP GUI
    Gui gui = Gui(&guiScreen);
    ImGuiIO& io = gui.setup(GUI_WINDOW_FLAGS);

    // STATES
    Sint32 delay = MIN_DELAY;
    Sint32 numberOfBlocks = MIN_BLOCKS_IN_VECTOR;
    Color blocksColor = Color(210.0f/255, 180.0f/255, 160.0f/255, 1.0f);
    Color backgroundColor = Color(60.0f/255, 75.0f/255, 75.0f/255, 1.0f);
    Color targetedBlockColor = Color(255.0f/255, 0.0f/255, 0.0f/255, 1.0f);
    bool blockRendering = false;
    
    // SETUP BVECTOR
    BVector bvector;

    // MAIN LOOP
    mainLoop(mainScreen, guiScreen, bvector, io, &blockRendering, &numberOfBlocks, targetedBlockColor, backgroundColor, blocksColor, &delay);

    // CLEANUP
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_Quit();

    return 0;
}

