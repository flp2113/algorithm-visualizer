#include "Screen.h"

Screen::Screen() : window(NULL), renderer(NULL) {}

Screen::Screen(const char* title, int width, int height,
               int posX, int posY, SDL_WindowFlags windowFlags)
              : window(NULL), renderer(NULL) {
	setup(title, width, height, posX, posY, windowFlags);
}

Screen::~Screen() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Screen::setup(const char* title, int width, int height, 
                   int posX, int posY, SDL_WindowFlags windowFlags) {

    if (window != NULL || renderer != NULL)
        throw std::runtime_error("WINDOW/RENDERER ALREADY SET UP!");


    if (!SDL_CreateWindowAndRenderer(title, width, height, windowFlags, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return;
    }

    SDL_SetWindowPosition(window, posX, posY);
}
