#pragma once

#include <SDL3/SDL.h>

typedef struct {
    Sint32 index;
    float value;
    SDL_FRect rect;
    bool targeted;
} Block;