#pragma once

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "../Screen/Screen.h"

class Gui {
public:
	Screen* screen;

	Gui(Screen*);

	ImGuiIO& setup(int);
};