#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <vector>

#include <vector>
using namespace std;
int const SCREEN_WIDTH = 1280;
int const SCREEN_HEIGHT = 720;

void squareWith(vector<SDL_Rect> &squares,int x,int n){
    for(int i = 0 ; i < n;i++){
        squares.push_back({x,(int)SCREEN_HEIGHT - 210-i*70,100,100});
    }
    
}
void spikeWith(vector<SDL_Rect> &spikes,int x,int n){
    for(int i = 0 ; i < n;i++){
        
        if(n == 1){
            spikes.push_back({x ,(int)SCREEN_HEIGHT - 210, 100, 100});

        }else{
         spikes.push_back({x+i*60 ,(int)SCREEN_HEIGHT - 210, 90, 90});

        }

    }
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Window *window;
    // (50,510)
    SDL_Rect source{50, SCREEN_HEIGHT - 210, 70, 70};

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Lỗi khởi tạo SDL2_image: " << IMG_GetError() << endl;
        system("pause");
        return -1;
    }
    window = SDL_CreateWindow("geomestry dash", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *background = IMG_LoadTexture(renderer, "C:/Users/ADMIN/Desktop/games/src/bg3.png");
    SDL_Texture *objectTexture = IMG_LoadTexture(renderer, "C:/Users/ADMIN/Desktop/games/src/gameObject.png");
    SDL_Event event;
    // Vòng lặp game
    Uint32 lastTime = SDL_GetTicks();
    float speed = 7.0f;
    float jumpVelocity = -25.0f;
    float gravity = 1.0f;
    float ground = SCREEN_HEIGHT - 210;
    float velocityY = 9.0f;
    bool isJumping = false;
    float cameraCheckpoint = 500.0f;
    float cameraSpeed = speed;
    float camera = 0;
    float angle = 0.0f;
    Uint8 playerAlpha = 255;
    bool isDead = false;
    SDL_Texture *thornTexture = IMG_LoadTexture(renderer, "C:/Users/ADMIN/Desktop/games/src/thorn1.png");
    SDL_Texture *squareTexture = IMG_LoadTexture(renderer, "C:/Users/ADMIN/Desktop/games/src/square1.png");
    vector<SDL_Rect> spikes;
    
    
        // spikes.push_back({500 + 0 * 1500, (int)ground, 100, 100});
        // spikes.push_back({500 + 1 * 1500, (int)ground, 90, 90});
        // spikes.push_back({500 + 1 * 1500+60, (int)ground, 90, 90});
        spikeWith(spikes,500 + 0 * 1500,1);
        spikeWith(spikes,500 + 1*1500,2);

        

    
    vector<SDL_Rect> squares;
        squareWith(squares,600 + 1 * 800,2);
        squareWith(squares,600 + 1 * 800+150,1);
        squareWith(squares,600 + 1 * 800+150*2,3);
     


    // }

    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        // velocityY += gravity * ;
        if (deltaTime >= 4)
            deltaTime = 4;
        int bgX = (int)(-camera) % SCREEN_WIDTH;
        // cout << source.y + source.h << endl;
        // cout << source.x << endl;
        if (!isDead)
        {
            velocityY += gravity * deltaTime;
            const float MAX_FALL_SPEED = 10.0f;

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
                angle = 0.0f;
                // lastTime = currentTime-500;
                isJumping = false;
            }
            if (source.x > cameraCheckpoint)
            {
                camera = source.x - cameraCheckpoint;
            }
            if (isJumping)
            {
                angle += 12.0f;
            }
            SDL_Rect playerRects = {(int)(source.x - camera), source.y, source.w-20, source.h - 30};
            for (const auto &spike : spikes)
            {
                SDL_Rect spikeRect = {spike.x - (int)camera, spike.y, spike.w - 30, spike.h - 30};
                if (SDL_HasIntersection(&playerRects, &spikeRect))
                {
            
                     isDead = true;
                }
            }
            for (const auto &square : squares)
            {
                SDL_Rect squareRect = {square.x - (int)camera, square.y, square.w, square.h+10};

                if (SDL_HasIntersection(&playerRects, &squareRect))
                {
                    
                    if(source.y <=square.y+30 && source.x+60 <= square.x+20 ){
                        isDead= true;
                        cout << source.y <<" "<<square.y+30 <<" " <<source.x <<" "<<square.x+20 << endl;
                        speed = 0;
                        
                    }
                    else if (source.y + source.h-60  <= square.y + 10) 
                    {
                        source.y = square.y - source.h + 25;
                        velocityY = 0;
                        isJumping = false;
                        angle = 0;
                    }
                    
                    
                }
            }
        }
        else
        {
            if (playerAlpha > 0)
            {
        
            }
            else
            {
                running = false;
            }
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
                case SDLK_r:
                    if(isDead){
                        lastTime = SDL_GetTicks();
                        speed = 7.0f;
                        jumpVelocity = -25.0f;
                        gravity = 1.0f;
                        ground = SCREEN_HEIGHT - 210;
                        velocityY = 9.0f;
                        bool isJumping = false;
                        cameraCheckpoint = 500.0f;
                        cameraSpeed = speed;
                        camera = 0;
                        angle = 0.0f;
                        playerAlpha = 255;
                        isDead = false;
                        source.x = 50;
                        source.y = SCREEN_HEIGHT - 210;
                    }
                    
                }
            }
            // if (event.type == SDL_MOUSEMOTION) {
            //     int mouseX = event.motion.x;
            //     int mouseY = event.motion.y;
            //     cout << "Mouse Position: (" << mouseX << ", " << mouseY << ")" << endl;
            // }
            
        }

        SDL_RenderClear(renderer);

        // SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect bgRect = {bgX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect bgRect2 = {bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, background, NULL, &bgRect);
        SDL_RenderCopy(renderer, background, NULL, &bgRect2);

        SDL_Rect playerRect = {(int)(source.x - camera), source.y, source.w, source.h};
        SDL_Point center = {source.w / 2, source.h / 2};
        SDL_RenderCopyEx(renderer, objectTexture, NULL, &playerRect, angle, &center, SDL_FLIP_NONE);

        for (const auto &spike : spikes)
        {
            SDL_Rect spikeRect = {spike.x - (int)camera, spike.y, spike.w, spike.h};
            SDL_RenderCopy(renderer, thornTexture, NULL, &spikeRect);
        }
        for (const auto &square : squares)
        {
            SDL_Rect squareRect = {square.x - (int)camera, square.y, square.w, square.h};
            SDL_RenderCopy(renderer, squareTexture, NULL, &squareRect);
        }
        SDL_RenderPresent(renderer);


        SDL_Delay(16);
    }
    return 0;
}