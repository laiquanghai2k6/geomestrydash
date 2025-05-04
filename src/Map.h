// #pragma once
// #include <vector>
// #include <SDL2/SDL.h>

// void setupMap(std::vector<SDL_Rect>& spikes, std::vector<SDL_Rect>& squares);


#include <SDL2/SDL.h>
#include <vector>
class Map {
private:
    std::vector<SDL_Rect> spikes;
    std::vector<SDL_Rect> spikesReverse;

    std::vector<SDL_Rect> squares;
    
    SDL_Texture* thornTexture;
    SDL_Texture* squareTexture;
    SDL_Texture* portalTexture;
    SDL_Texture* portalEndTexture;

    SDL_Texture* thornReverseTexture;


    
    SDL_Rect portal;
    SDL_Rect portalEnd;

    SDL_Rect skull;
    
    void spikeWith(int x, int n);
    void spikeNgang(int x, int n);
    void squareWith(int x, int n);
    void squareNgang(int x, int h, int w);
    void squareHead(int x,int h,int w);
    void spikeHead(int x,int n);
public:
    Map(SDL_Renderer* renderer);
    ~Map();
    
    void render(SDL_Renderer* renderer, float cameraX);
    
    const std::vector<SDL_Rect>& getSpikes() const;
    const std::vector<SDL_Rect>& getReverse() const;

    const std::vector<SDL_Rect>& getSquares() const;
    const SDL_Rect& getPortal() const;
    const SDL_Rect& getPortalEnd() const;


};

