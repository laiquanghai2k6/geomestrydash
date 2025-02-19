#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>

#include <vector>
using namespace std;
int const SCREEN_WIDTH = 1280;
int const SCREEN_HEIGHT = 720;
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Window *window;
    SDL_Rect source{50, SCREEN_HEIGHT - 300, 70, 70};
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Lỗi khởi tạo SDL2_image: " << IMG_GetError() << endl;
        system("pause");
        return -1;
    }
    window = SDL_CreateWindow("geomestry dash", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Lỗi SDL_image: " << IMG_GetError() << endl;
        return -1;
    }
    // SDL_Rect rect = {50, 50, 100, 100};  

    // cout << 5 << endl;
    SDL_Texture *background = IMG_LoadTexture(renderer, "C:/Users/ADMIN/Desktop/games/src/bg.png");
    SDL_Texture *objectTexture = IMG_LoadTexture(renderer, "C:/Users/ADMIN/Desktop/games/src/gameObject.png");

    SDL_Event event;
    // Vòng lặp game
    Uint32 lastTime = SDL_GetTicks();
    float speed = 7.0f;
    float jumpVelocity = -25.0f;
    float gravity = 1.0f;
    float ground = SCREEN_HEIGHT - 300;
    float velocityY = 9.0f;
    bool isJumping = false;
    float cameraCheckpoint = 500.0f;
    float cameraSpeed = speed;
    float camera;
    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        // velocityY += gravity * ;
        if(deltaTime >=4) deltaTime = 4;
        velocityY += gravity*deltaTime;
        const float MAX_FALL_SPEED = 10.0f;
        int bgX = (int)(-camera) % SCREEN_WIDTH;

        if (velocityY > MAX_FALL_SPEED)
        {
            velocityY = MAX_FALL_SPEED;
        }
        if (speed * deltaTime <= 7)
        {

            source.x += speed * deltaTime;
        }
        else
        {
            source.x += 7;
        }
        if (velocityY * deltaTime <= 10)
        {

            source.y += velocityY * deltaTime;
        }
        else
        {
            source.y += 10;
        }

        if (source.y >= ground)
        {
            source.y = ground;
            velocityY = 0;
            // lastTime = currentTime-500;
            isJumping = false;
        }
        if (source.x > cameraCheckpoint)
        {
            camera = source.x - cameraCheckpoint;
        }
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false; 
                return 0;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:

                    if (!isJumping)
                    {
                        velocityY = jumpVelocity;
                        isJumping = true;
                    }
                    break;
                }
            }
        }

        SDL_RenderClear(renderer);

        // SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect bgRect = {bgX , 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect bgRect2 = { bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, background, NULL, &bgRect);
        SDL_RenderCopy(renderer, background, NULL, &bgRect2);

        SDL_Rect playerRect = {(int)(source.x - camera), source.y, source.w, source.h};
        SDL_RenderCopy(renderer, objectTexture, NULL, &playerRect);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
    return 0;
}