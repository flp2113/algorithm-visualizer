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

#include "imgui.h"
#include "defs.h"
#include "./Block/block.h"

enum BlockColor {
    BLOCKS_COLOR,
    BACKGROUND_COLOR,
    TARGETED_BLOCK_COLOR
};

class BVector {
public:
    Color bColor = { 210.0f / 255, 180.0f / 255, 160.0f / 255, 1.0f };
    Color bgColor = { 60.0f / 255, 75.0f / 255, 75.0f / 255, 1.0f };
    Color tbColor = { 170.0f / 255, 85.0f / 255, 80.0f / 255, 1.0f };
    SDL_FRect* rects;
    std::vector<Block> vec;
    std::queue<std::tuple<Sint32, Sint32>> steps;

    BVector()                                           noexcept;
    explicit BVector(const Sint32);
    explicit BVector(const std::vector<Block>)          noexcept;

    void resize(const Sint32);
    void fillBlocks(const Sint32);
    void setColor(const Color, const Sint8)             noexcept;
    void renderBlocks(SDL_Renderer*)                    noexcept;
    void renderSortSteps(SDL_Renderer*)                 noexcept;
private:
    size_t getSize()                                    const noexcept;
    bool isEmpty()                                      const noexcept;
    bool isInvalidIndex(const Sint32)                   const noexcept;
    bool isInvalidNumberOfBlocks(const Sint32)          const noexcept;
    void setBlockDimensions(Block&, const Sint32)       const noexcept;
    void switchColors(Block&)                           const noexcept;
    void initialize(const Sint32)                       noexcept;
    void toArray()                                      noexcept;
    void bubbleSort()                                   noexcept;
    Block getBlock(const Sint32)                        const;
    void setBlock(const Sint32, const Block);
};

