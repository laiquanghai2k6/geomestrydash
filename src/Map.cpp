#include "Map.h"
#include <bits/stdc++.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <unistd.h>

Map::Map(SDL_Renderer* renderer) {
    
    thornTexture = IMG_LoadTexture(renderer, "../../src/thorn1.png");
    thornReverseTexture = IMG_LoadTexture(renderer, "../../src/thorn-nguoc.png");

    squareTexture = IMG_LoadTexture(renderer, "../../src/square1.png");
    portalTexture = IMG_LoadTexture(renderer, "../../src/portal.png");
    portalEndTexture = IMG_LoadTexture(renderer, "../../src/portalEnd.png");
    if (!portalEndTexture)
    {
        std::cerr << "loi cong " << IMG_GetError() << std::endl;
    }
    skull={9000,300,200,200};
    portal = {4700, 200, 103, 138};
    portalEnd={23740,0,80,720};

    spikeWith(500 + 0 * 1500, 1);
    spikeWith(500 + 1 * 1800, 2);
    squareWith(600 + 1 * 800, 2);
    squareWith(600 + 1 * 800 + 150, 1);
    squareWith(600 + 1 * 800 + 150 * 2, 3);
    squareWith(2800 + 150 * 1, 1);
    squareWith(2800 + 150 * 2, 3);
    squareWith(2800 + 150 * 3, 5);
    squareWith(2800 + 150*3+64*1, 5);
    squareWith(2800 + 150*3+64*2, 5);
    squareWith(2800 + 150*3+64*3, 5);
    squareWith(2800 + 150 * 5, 3);
    squareWith(2800 + 150 * 6, 1);
    squareNgang(2800 + 150 * 9, 2, 10);
    spikeNgang(4800, 400);
    squareHead(5500,5,2);
    squareHead(6300,3,2);
    squareHead(7000,4,5);
    squareNgang(7900,4,5);
    spikeHead(9000,300);
}

Map::~Map() {
    SDL_DestroyTexture(thornTexture);
    SDL_DestroyTexture(squareTexture);
    SDL_DestroyTexture(portalTexture);
}

void Map::spikeWith(int x, int n) {
    for(int i = 0; i < n; i++) {
        if(n == 1) {
            spikes.push_back({x, 720 - 210, 100, 100});
        } else {
            spikes.push_back({x + i * 60, 720 - 210, 90, 90});
        }
    }
}

void Map::spikeNgang(int x, int n) {
    for(int i = 0; i < n; i++) {
        spikes.push_back({x + i * 60, 720 - 210, 90, 90});
    }
}
void Map::spikeHead(int x,int n){
    for(int i = 0; i < n; i++) {
        spikesReverse.push_back({x + i * 60, -20, 90, 90});
    }
}
void Map::squareWith(int x, int n) {
    for(int i = 0; i < n; i++) {
        squares.push_back({x, 720 - 210 - i * 70, 100, 100});
    }
}

void Map::squareNgang(int x, int h, int w) {
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            squares.push_back({x + 70 * i, 700 - 210 - j * 70, 100, 100});
        }
    }
}
void Map::squareHead(int x,int h,int w){
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            squares.push_back({x + 70 * i, -20+j * 70, 100, 100});
        }
    }
}


void Map::render(SDL_Renderer* renderer, float cameraX) {
 
    SDL_Rect portalRect = {portal.x - (int)cameraX, portal.y, portal.w, portal.h};
    SDL_RenderCopy(renderer, portalTexture, NULL, &portalRect);
    SDL_Rect portalEndRect = {portalEnd.x - (int)cameraX, portalEnd.y, portalEnd.w, portalEnd.h};
    SDL_RenderCopy(renderer, portalEndTexture, NULL, &portalEndRect);
   
    for(const auto& spike : spikes) {
        SDL_Rect spikeRect = {spike.x - (int)cameraX, spike.y, spike.w, spike.h};
        SDL_RenderCopy(renderer, thornTexture, NULL, &spikeRect);
    }
    
  
    for(const auto& square : squares) {
        SDL_Rect squareRect = {square.x - (int)cameraX, square.y, square.w, square.h};
        SDL_RenderCopy(renderer, squareTexture, NULL, &squareRect);
    }
    for(const auto& spikeReverse : spikesReverse) {
        SDL_Rect spikeRect = {spikeReverse.x - (int)cameraX, spikeReverse.y, spikeReverse.w, spikeReverse.h};
        SDL_RenderCopy(renderer, thornReverseTexture, NULL, &spikeRect);
    }
}

const std::vector<SDL_Rect>& Map::getSpikes() const {
    return spikes;
}
const std::vector<SDL_Rect>& Map::getReverse() const {
    return spikesReverse;
}

const std::vector<SDL_Rect>& Map::getSquares() const {
    return squares;
}

const SDL_Rect& Map::getPortal() const {
    return portal;
}
const SDL_Rect& Map::getPortalEnd() const {
    return portalEnd;
}