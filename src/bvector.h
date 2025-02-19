#pragma once

#include <SDL3/SDL.h>

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <ctime>

#include "defs.h"

typedef struct {
    Sint32 index;
    float value;
    SDL_FRect rect;
    bool targeted;
} Block;

typedef struct {
    float r, g, b, a;
} Color;

class BVector {
public:
    SDL_FRect* rects;
    std::vector<Block> vec;
    std::queue<std::tuple<Sint32, Sint32>> steps;

    BVector                 ()                              noexcept;
    explicit BVector        (const Sint32);
    explicit BVector        (const std::vector<Block>)      noexcept;

    void resize             (const size_t);
    void fillBlocks         (const Sint32);
    void renderBlocks       (SDL_Renderer*)                 noexcept;
    void renderSortSteps    (SDL_Renderer*, const Sint32)   noexcept;
private:
    size_t getSize                  ()                              const noexcept;
    bool isEmpty                    ()                              const noexcept;
    bool isInvalidIndex             (const Sint32)                  const noexcept;
    bool isInvalidNumberOfBlocks    (const Sint32)                  const noexcept;
    void setBlockDimensions         (Block&, const Sint32)          const noexcept;
    void switchColors               (Block&)                        const noexcept;
    void initialize                 (const Sint32)                  noexcept;
    void toArray                    ()                              noexcept;
    void bubbleSort                 ()                              noexcept;
    Block getBlock                  (const Sint32)                  const;
    void setBlock                   (const Sint32, const Block);
};

