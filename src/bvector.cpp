#pragma once

#include "bvector.h"

BVector::BVector() noexcept {
    initialize(MIN_BLOCKS_IN_VECTOR);
}

BVector::BVector(const Sint32 numberOfBlocks) {
    if(isInvalidNumberOfBlocks(numberOfBlocks))
        throw std::out_of_range("CONSTRUCTOR: OUT OF BOUNDS NUMBER OF BLOCKS");

    initialize(MIN_BLOCKS_IN_VECTOR);
}

BVector::BVector(const std::vector<Block> v) noexcept : vec(v) {
    toArray();
    bubbleSort();
}

Block BVector::getBlock(const Sint32 index) const {
    if (isInvalidIndex(index))
        throw std::out_of_range("GET BLOCK: OUT OF BOUNDS INDEX");

    return vec.at(index);
}

void BVector::setBlock(const Sint32 index, const Block block) {
    if (isInvalidIndex(index))
        throw std::out_of_range("SET BLOCK: OUT OF BOUNDS INDEX");

    vec.at(index) = block;
}

size_t BVector::getSize() const noexcept {
    return vec.size();
}

void BVector::resize(const Sint32 numberOfBlocks) {
    if (isInvalidNumberOfBlocks(numberOfBlocks))
        throw std::out_of_range("RESIZE: OUT OF BOUNDS NUMBER OF BLOCKS");
    
    vec.resize(static_cast<size_t>(numberOfBlocks));
    delete[] rects;
    rects = nullptr;

    while (!steps.empty()) {
        steps.pop();
    }

    initialize(numberOfBlocks);
}

bool BVector::isEmpty() const noexcept {
    return vec.size() == 0;
}

void BVector::toArray() noexcept {
    size_t vecSize = vec.size();
    rects = new SDL_FRect[vecSize];
    for (size_t i = 0; i < vecSize; i++) {
        rects[i] = vec.at(i).rect;
    }
}

void BVector::fillBlocks(const Sint32 numberOfBlocks) {
    if (isInvalidNumberOfBlocks(numberOfBlocks))
        throw std::out_of_range("FILL BLOCKS: OUT OF BOUNDS NUMBER OF BLOCKS");

    vec.clear();

    srand(static_cast<Uint32>(time(NULL)));

    for (Sint32 i = 0; i < numberOfBlocks; i++) {
        float blockValue = static_cast<float>((rand() % MAX_VALUE_IN_VECTOR) + 1);
        Block block = {
            i,
            blockValue,
            {
                i * (WINDOW_WIDTH / getSize()), WINDOW_HEIGHT - blockValue,
                WINDOW_WIDTH / getSize(), blockValue
            },
            false
        };

        vec.push_back(block);
    }
}

void BVector::setColor(const ImVec4 color, const Sint8 option) noexcept {
    switch (option) {
    case BLOCKS_COLOR: bColor = { color.x, color.y, color.z, 1.0f }; break;
    case BACKGROUND_COLOR: bgColor = { color.x, color.y, color.z, 1.0f }; break;
    case TARGETED_BLOCK_COLOR: tbColor = { color.x, color.y, color.z, 1.0f }; break;
    }
}

void BVector::renderBlocks(SDL_Renderer* renderer) noexcept {
    for (Uint32 i = 0; i < vec.size(); i++) {
        rects[i] = vec.at(i).rect;
        setBlockDimensions(vec.at(i), i);

        if (vec.at(i).targeted)
            SDL_SetRenderDrawColorFloat(renderer, tbColor.x, tbColor.y, tbColor.z, SDL_ALPHA_OPAQUE);
        else
            SDL_SetRenderDrawColorFloat(renderer, bColor.x, bColor.y, bColor.z, SDL_ALPHA_OPAQUE);

        SDL_RenderFillRect(renderer, &rects[i]);
    }
}

void BVector::renderSortSteps(SDL_Renderer* renderer, const Sint32 delay) noexcept {
    if (steps.empty()) {
        SDL_RenderPresent(renderer);
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, std::to_string(steps.size()).c_str());

    std::tuple<Uint32, Uint32> step = steps.front();

    Uint32 firstIndex = std::get<0>(step);
    Uint32 secondIndex = std::get<1>(step);

    switchColors(vec.at(firstIndex));
    switchColors(vec.at(secondIndex));

    std::swap(vec.at(firstIndex), vec.at(secondIndex));
    std::swap(rects[firstIndex], rects[secondIndex]);

    renderBlocks(renderer);

    SDL_RenderPresent(renderer);

    SDL_Delay(delay);

    switchColors(vec.at(firstIndex));
    switchColors(vec.at(secondIndex));

    steps.pop();
}

void BVector::bubbleSort() noexcept {
    std::vector<Block> vec_tmp = vec;
    size_t n = vec.size();
    bool swapped;

    for (Uint32 i = 0; i < n - 1; i++) {
        swapped = false;
        for (Uint32 j = 0; j < n - i - 1; j++) {
            if (vec_tmp.at(j).value > vec_tmp.at(j + 1).value) {
                std::swap(vec_tmp.at(j), vec_tmp.at(j + 1));

                steps.push(std::make_tuple(j, j + 1));

                swapped = true;
            }
        }

        if (!swapped) 
            break;
    }
}

bool BVector::isInvalidIndex(const Sint32 index) const noexcept {
    return index < 0 || index > vec.size() - 1;
}

bool BVector::isInvalidNumberOfBlocks(const Sint32 numberOfBlocks) const noexcept {
    return numberOfBlocks < MIN_BLOCKS_IN_VECTOR || numberOfBlocks > MAX_BLOCKS_IN_VECTOR;
}

void BVector::setBlockDimensions(Block& block, const Sint32 index) const noexcept {
    block.rect.h = block.value;
    block.rect.w = WINDOW_WIDTH / static_cast<float>(getSize());

    block.rect.x = index * block.rect.w;
    block.rect.y = WINDOW_HEIGHT - block.value;
}

void BVector::switchColors(Block& block) const noexcept {
    block.targeted = !block.targeted;
}

void BVector::initialize(const Sint32 numberOfBlocks) noexcept {
    fillBlocks(numberOfBlocks);
    toArray();
    bubbleSort();
}


