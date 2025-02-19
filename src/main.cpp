#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>

#include <vector>
using namespace  std;
int const SCREEN_WIDTH = 1280;
int const SCREEN_HEIGHT = 720;
int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Window* window;
    SDL_Rect source{50,SCREEN_HEIGHT-800,70,70};
    // SDL_Rect dest{10,10,Window_H-20,Window_W-20};
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "Lỗi khởi tạo SDL2_image: " << IMG_GetError() << endl;
        system("pause");
        return -1;
    }
    window = SDL_CreateWindow("snakeio",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH ,SCREEN_HEIGHT,0);
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window,-1,0);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    cout << "Lỗi SDL_image: " << IMG_GetError() << endl;
    return -1;
}
// SDL_Rect rect = {50, 50, 100, 100}; 


// cout << 5 << endl;
    SDL_Texture* background = IMG_LoadTexture(renderer, "C:/Users/ADMIN/Desktop/games/src/bg.png");
    SDL_Texture* objectTexture = IMG_LoadTexture(renderer, "C:/Users/ADMIN/Desktop/games/src/gameObject.png");


    
    SDL_Event event;
    // Vòng lặp game
    Uint32 lastTime = SDL_GetTicks();
    float speed = 5.0f;
    float jumpVelocity = -30.0f; 
    float gravity = 2.0f;
    float ground = SCREEN_HEIGHT-300;
    float velocityY = 10.0f;
    bool isJumping = false;
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
        velocityY+=gravity*deltaTime;
        source.x += speed * deltaTime;
        source.y += velocityY * deltaTime;
        if(source.y >= ground){
            source.y = ground;
            velocityY = 0;
            isJumping = false;
        }
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; 
                return 0;
            }
            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                    source.y-=5;
                    if (!isJumping) {  
                        velocityY = jumpVelocity;
                        isJumping = true;
                    }
                    break;
                }
            }
        }
        
        SDL_RenderClear(renderer);
    
        
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
      
        SDL_RenderCopy(renderer, background, NULL, &bgRect);
        SDL_RenderCopy(renderer, objectTexture, NULL, &source);

        // Vẽ các đối tượng khác ở trên nền
        SDL_RenderPresent(renderer);
       
        SDL_Delay(16);
    }
    return 0;
}
