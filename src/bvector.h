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

#define DEBUG

namespace GZBlock {
    typedef struct {
        Uint32 index;
        float value;
        SDL_FRect rect;
        bool targeted;
    } Block;

    class BVector {
    public:
        SDL_FRect* rects;
        std::vector<Block> vec;
        std::queue<std::tuple<Uint32, Uint32>> steps;
        
        BVector(Uint32 numberOfBlocks) {
            fillBlocks(numberOfBlocks);
            toArray();
            bubbleSort();
        }

        BVector(std::vector<Block> v) : vec(v) {
            toArray();
            bubbleSort();
        }

        Block getBlock(Uint32 index) {
            if (isInvalidIndex(index))
                throw std::out_of_range("GET BLOCK: OUT OF BOUNDS INDEX");

            Block block = vec.at(index);

            #ifdef DEBUG
                std::cout << "(DEBUG) GET BLOCK: "
                    << "[" << block.index << ", "
                    << block.value << ", "
                    << "{ " << block.rect.x << ", " << block.rect.y << ", " << block.rect.w << ", " << block.rect.h << " }, "
                    << (block.targeted ? "true" : "false") << "]" << std::endl;
            #endif

            return block;
        }

        void setBlock(Uint32 index, Block block) {
            if (isInvalidIndex(index))
                throw std::out_of_range("SET BLOCK: OUT OF BOUNDS INDEX");

            Block toChange = vec.at(index);

            #ifdef DEBUG
                std::cout << "(DEBUG) SET BLOCK: "
                    << "[" << toChange.index << ", "
                    << toChange.value << ", "
                    << "{ " << toChange.rect.x << ", " << toChange.rect.y << ", " << toChange.rect.w << ", " << toChange.rect.h << " }, "
                    << (toChange.targeted ? "true" : "false") << "] ===> "
                    << "[" << block.index << ", "
                    << block.value << ", "
                    << "{ " << block.rect.x << ", " << block.rect.y << ", " << block.rect.w << ", " << block.rect.h << " }, "
                    << (block.targeted ? "true" : "false") << "]" << std::endl;
            #endif

            vec.at(index) = block;
        }

        size_t getSize() {
            return vec.size();
        }

        void setSize(size_t size) {
            if (size > UINT32_MAX)
                throw std::out_of_range("SET SIZE: INVALID SIZE");

            vec.resize(size);
            delete[] rects;
            rects = nullptr;

            while (!steps.empty()) {
                steps.pop();
            }

            fillBlocks(size);
            toArray();
            bubbleSort();
        }

        bool isEmpty() {
            return vec.size() == 0;
        }

        void renderBlocks(SDL_Renderer* renderer) {
            for (Uint32 i = 0; i < vec.size(); i++) {
                rects[i] = vec.at(i).rect;
                setBlockDimensions(vec.at(i), i);
                
                if (vec.at(i).targeted)
                    SDL_SetRenderDrawColorFloat(renderer, 1.0f, 0.0f, 0.0f, SDL_ALPHA_OPAQUE);
                else
                    SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 1.0f, SDL_ALPHA_OPAQUE);

                SDL_RenderFillRect(renderer, &rects[i]);
            }
        }

        void toArray() {
            size_t vecSize = vec.size();
            rects = new SDL_FRect[vecSize];
            for (size_t i = 0; i < vecSize; i++) {
                rects[i] = vec.at(i).rect;
            }
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
            std::swap(rects[firstIndex], rects[secondIndex]);

            renderBlocks(renderer);

            SDL_RenderPresent(renderer);

            SDL_Delay(DELAY_VECTOR_CHANGES);

            switchColors(vec.at(firstIndex));
            switchColors(vec.at(secondIndex));

            steps.pop();
        }

        void fillBlocks(Uint32 numberOfBlocks) {
            vec.clear();

            srand(time(NULL));

            for (Uint32 i = 0; i < numberOfBlocks; i++) {
                float blockValue = (rand() % MAX_VALUE_IN_VECTOR) + 1;
                Block block = { // { index, value, SDL_FRect, targeted }
                    i,
                    blockValue,
                    {//xywh
                        i * (WINDOW_WIDTH / getSize()), WINDOW_HEIGHT - blockValue,
                        WINDOW_WIDTH / getSize(), blockValue
                    },
                    false
                };

                vec.push_back(block);
            }
        }

        private:
            bool isInvalidIndex(Uint32 index) {
                return index < 0 || index > vec.size() - 1;
            }

            void setBlockDimensions(Block& block, Uint32 index) {
                block.rect.h = block.value;
                block.rect.w = WINDOW_WIDTH / static_cast<float>(getSize());

                block.rect.x = index * block.rect.w;
                block.rect.y = WINDOW_HEIGHT - block.value;
            }

            void switchColors(Block& block) {
                block.targeted = !block.targeted;
            }
    };
}
