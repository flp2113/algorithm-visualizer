#pragma once

#include <SDL3/SDL.h>
#include "defs.h"

int setupSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Window** guiWindow, SDL_Renderer** guiRenderer) {
    SDL_SetAppMetadata(APP_TITLE, APP_VERSION, APP_IDENTIFIER);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS, window, renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(GUI_TITLE, GUI_WIDTH, GUI_HEIGHT, GUI_FLAGS, guiWindow, guiRenderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetWindowPosition(*window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    return 0;
}

