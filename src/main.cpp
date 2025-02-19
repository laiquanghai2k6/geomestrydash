#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <vector>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

using namespace  std;
int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_VIDEO) > 0){
        cout << "sdl fail video" << SDL_GetError()<<endl;
    }
    if(!(IMG_Init(IMG_INIT_PNG))){
        cout << "sdl fail image" << SDL_GetError()<<endl;
    }
    if(!(SDL_Init(SDL_INIT_AUDIO)) < 0){
        return 1;
        cout << "sdl fail audio" << SDL_GetError()<<endl;
    }
    if(TTF_Init() == -1){
        cout << "ttf error"<<endl;
        return 1;
    }
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(window,-1,0);
    SDL_Delay(10000);
   
}