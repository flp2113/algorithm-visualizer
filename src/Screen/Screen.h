#pragma once

#include <SDL3/SDL.h>

#include <stdexcept>

#include "../defs.h"

class Screen {
public:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Screen();
	Screen(const char*, int, int, int, int, SDL_WindowFlags);
	~Screen();

	void setup(const char*, int, int, int, int, SDL_WindowFlags);
};
