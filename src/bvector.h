#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "defs.h"

#include <vector>
#include <queue>
#include <tuple>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>

namespace GZBlock {
    typedef struct {
        Uint32 index;
        Uint32 value;
        SDL_FRect rect;
        bool targeted;
    } Block;

    class BVector {
    protected:
        std::vector<Block> vec;
        std::queue<std::tuple<Uint32, Uint32>> steps;

    public:
        BVector(Uint32 numberOfBlocks) {
            fillBlocks(numberOfBlocks);
            bubbleSort();
        }

        BVector(std::vector<Block> v) : vec(v) {
            bubbleSort();
        }

        Block getBlock(Uint32 index) {
            if (isInvalidIndex(index))
                throw std::out_of_range("GET BLOCK: OUT OF BOUNDS INDEX");

            return vec.at(index);
        }

        void setBlock(Uint32 index, Block block) {
            if (isInvalidIndex(index))
                throw std::out_of_range("SET BLOCK: OUT OF BOUNDS INDEX");

            vec.at(index) = block;
        }

        size_t getSize() {
            return vec.size();
        }

        void setSize(size_t size) {
            if (size < 0)
                throw std::out_of_range("SET SIZE: INVALID SIZE");

            vec.resize(size);
        }

        bool isEmpty() {
            return vec.size() == 0;
        }

        bool isInvalidIndex(Uint32 index) {
            return index < 0 || index > vec.size() - 1;
        }

        void setBlockDimensions(Block& block, Uint32 index) {
            block.rect.h = static_cast<float>(block.value);
            block.rect.w = WINDOW_WIDTH / static_cast<float>(getSize());

            block.rect.x = index * block.rect.w;
            block.rect.y = WINDOW_HEIGHT - static_cast<float>(block.value);
        }

        void renderBlocks(SDL_Renderer* renderer) {
            SDL_FRect* rects = toArray();
            for (Uint32 i = 0; i < vec.size(); i++) {
                setBlockDimensions(vec.at(i), i);

                if (vec.at(i).targeted)
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
                else
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

                SDL_RenderFillRect(renderer, &rects[i]);
            }
        }

        void fillBlocks(Uint32 numberOfBlocks) {
            srand(time(NULL));

            for (Uint32 i = 0; i < numberOfBlocks; i++) {
                Block block = { // { index, value, SDL_FRect, targeted }
                    i,
                    (rand() % MAX_VALUE_IN_VECTOR) + 1,
                    { 0.0f, 0.0f, 0.0f, 0.0f },
                    false
                };

                vec.push_back(block);
            }
        }

        void switchColors(Block& block) {
            block.targeted = !block.targeted;
        }

        SDL_FRect* toArray() {
            size_t vecSize = vec.size();

            SDL_FRect* rects = new SDL_FRect[vecSize];
            for (size_t i = 0; i < vecSize; i++) {
                rects[i] = vec.at(i).rect;
            }

            return rects;
        }

        void bubbleSort() {
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

                if (!swapped) break;
            }
        }

        void renderSortSteps(SDL_Renderer* renderer) {
            if (steps.empty()) {
                SDL_RenderPresent(renderer);
                return;
            }

            //std::cout << steps.size() << '\n'; //DEBUG STEPS

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); 
            SDL_RenderDebugText(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, std::to_string(steps.size()).c_str());

            std::tuple<Uint32, Uint32> step = steps.front();

            Uint32 firstIndex = std::get<0>(step);
            Uint32 secondIndex = std::get<1>(step);

            switchColors(vec.at(firstIndex));
            switchColors(vec.at(secondIndex));

            std::swap(vec.at(firstIndex), vec.at(secondIndex));

            renderBlocks(renderer);

            SDL_RenderPresent(renderer);

            SDL_Delay(DELAY_VECTOR_CHANGES);

            switchColors(vec.at(firstIndex));
            switchColors(vec.at(secondIndex));

            steps.pop();
        }
    };
}
