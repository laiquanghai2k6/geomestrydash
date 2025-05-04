#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <vector>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include <SDL2/SDL_mixer.h>
#include "Map.h"
#include <filesystem>

using namespace std;
int const SCREEN_WIDTH = 1280;
int const SCREEN_HEIGHT = 720;
int const First_SPEED = 10.0f;
int const InitX = 50;
int const InitY = 510;

int main(int argc, char *argv[])
{
std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Window *window;
   
    if (TTF_Init() == -1)
    {
        std::cerr << "lỗi ttf " << TTF_GetError() << std::endl;
        return -1;
    }
    SDL_Rect source{InitX,
                    //  SCREEN_HEIGHT - 210
                    InitY, 70, 70};

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Lỗi khởi tạo SDL2_image: " << IMG_GetError() << endl;
        system("pause");
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError() << endl;
        return -1;
    }
    window = SDL_CreateWindow("geomestry dash", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *background = IMG_LoadTexture(renderer, "../../src/bg3.png");
    SDL_Texture *laserTexture = nullptr;
    vector<SDL_Rect> lasers;
    SDL_Texture *objectTexture = IMG_LoadTexture(renderer, "../../src/gameObject.png");
    SDL_Texture *flyTexture = IMG_LoadTexture(renderer, "../../src/fly.png");
    SDL_Texture *skullTexture = IMG_LoadTexture(renderer, "../../src/skull-close.png");
    TTF_Font *font = TTF_OpenFont("../../src/arial.ttf", 28);

    laserTexture = IMG_LoadTexture(renderer, "../../src/lazer.png");
    if (!laserTexture)
    {
        cerr << "Failed to load laser texture: " << IMG_GetError() << endl;
    }
    if (!skullTexture)
    {
        std::cerr << "Failed to load skull texture: " << IMG_GetError() << std::endl;
    }
    SDL_Rect skullRect = {10000, 210, 150, 300};
    Uint32 skullAppearTime = 0;
    bool shottingLaze = false;
    bool l1 = false, l2 = false, l3 = false, l4 = false;
    bool r1 = false, r2 = false, r3 = false, r4 = false;
    bool skullHasAppeared = false;
    if (!flyTexture)
    {
        std::cerr << "Failed to load fly texture: " << IMG_GetError() << std::endl;
        return -1;
    }
    if (!flyTexture)
    {
        std::cerr << "Failed to load fly texture: " << IMG_GetError() << std::endl;
        return -1;
    }
    if (!background)
    {
        std::cerr << "Failed to load fly texture: " << IMG_GetError() << std::endl;
        return -1;
    }
    Mix_Music *bgMusic = nullptr;
    Mix_Chunk *deathSound = nullptr;
    Mix_Chunk *winSound = nullptr;
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();
    float speed = First_SPEED;
    float jumpVelocity = -25.0f;
    float gravity = 1.0f;
    float ground = SCREEN_HEIGHT - 210;
    float velocityY = 9.0f;
    bool isJumping = false;
    float cameraCheckpoint = 500.0f;
    float cameraCheckpoint2 = 23000;
    bool isPlaying = false;
    float cameraSpeed = speed;
    float camera = 0;
    float angle = 0.0f;
    Uint8 playerAlpha = 255;
    bool isDead = false;
    bool isComplete = false;
    bool onBlock = false;
    Map map(renderer);

    bool cheat = false;
    bool isFly = false;
    bool isMouseOverRestart = false;
    SDL_Rect restartButton = {
        SCREEN_WIDTH / 2 - 75,
        SCREEN_HEIGHT / 2 + 100,
        150,
        50};
    SDL_Rect playButton = {
        SCREEN_WIDTH / 2 - 75,
        SCREEN_HEIGHT / 2 + 100,
        150,
        50};
    deathSound = Mix_LoadWAV("../../src/DeathSound.mp3");
    winSound = Mix_LoadWAV("../../src/winning.mp3");
    while (running)
    {

        if (isDead || !isPlaying || isComplete)
        {
            Mix_HaltMusic();
        }
        if (!font)
        {
            cerr << "Failed to load font: " << TTF_GetError() << endl;
            return -1;
        }
        float completionPercent = (source.x / 23000.0f) * 100.0f;
        if (completionPercent > 100.0f)
            completionPercent = 100.0f;
        stringstream ss;
        ss << "Progress: " << fixed << setprecision(1) << completionPercent << "%";
        string progressText = ss.str();
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface *textSurface = TTF_RenderText_Blended(font, progressText.c_str(), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {20, 20, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        if (!l1 || !l2 || !l3 || !l4)
        {

            skullRect.x = 10000 - camera;
        }
        if (skullRect.x <= SCREEN_WIDTH - skullRect.w)
        {
            if (!skullHasAppeared)
            {

                skullHasAppeared = true;
                skullAppearTime = SDL_GetTicks();
            }
            if (!l1 || !l2 || !l3 || !l4)
                skullRect.x = SCREEN_WIDTH - skullRect.w - 20;
        }
        if (skullHasAppeared && !isDead)
        {
            Uint32 currentTime = SDL_GetTicks();

            Uint32 time = currentTime - skullAppearTime;
            if (time <= 6000 && time >= 3000)
            {
                if (!l1)
                {

                    skullTexture = IMG_LoadTexture(renderer, "../../src/skull-open.png");
                    l1 = true;
                }
                SDL_Rect newLaser = {
                    skullRect.x - 10 - 1200,
                    skullRect.y + skullRect.h / 2 - 30,
                    1300,
                    150,
                };
                shottingLaze = true;
                lasers.push_back(newLaser);
            }
            if (time <= 7000 && time >= 6000)
            {
                if (!r1)
                {
                    r1 = true;
                    skullTexture = IMG_LoadTexture(renderer, "../../src/skull-close.png");
                }
                shottingLaze = false;
            }

            if (time >= 7000 && time <= 9000)
            {
                if (skullRect.y > 20)
                {
                    skullRect.y -= 5;
                }
                lasers.clear();
            }
            if (time >= 11000 && time <= 14000)
            {
                if (!l2)
                {

                    skullTexture = IMG_LoadTexture(renderer, "../../src/skull-open.png");
                    l2 = true;
                }

                SDL_Rect newLaser = {
                    skullRect.x - 10 - 1200,
                    skullRect.y + skullRect.h / 2 - 30,
                    1300,
                    150,
                };

                lasers.push_back(newLaser);
                shottingLaze = true;
            }
            if (time >= 14000 && time <= 15000)
            {
                if (!r2)
                {
                    r2 = true;
                    skullTexture = IMG_LoadTexture(renderer, "../../src/skull-close.png");
                }
                shottingLaze = false;
            }
            if (time >= 15000 && time <= 17000)
            {
                if (skullRect.y < 250)
                {
                    skullRect.y += 5;
                }
                lasers.clear();
            }
            if (time >= 19000 && time <= 22000)
            {
                if (!l3)
                {

                    skullTexture = IMG_LoadTexture(renderer, "../../src/skull-open.png");
                    l3 = true;
                }
                SDL_Rect newLaser = {
                    skullRect.x - 10 - 1200,
                    skullRect.y + skullRect.h / 2 - 30,
                    1300,
                    150,
                };

                lasers.push_back(newLaser);
                shottingLaze = true;
            }
            if (time >= 22000 && time <= 24000)
            {
                if (!l4)
                {

                    skullTexture = IMG_LoadTexture(renderer, "../../src/skull-close.png");
                    l4 = true;
                }
                shottingLaze = false;
                if (skullRect.x < SCREEN_WIDTH + 100)
                {
                    skullRect.x += 8;
                }
                else
                {

                    skullHasAppeared = false;
                }
            }
            if (time >= 24000)
            {
            }
        }
        if (deltaTime >= 4)
            deltaTime = 4;
        int bgX = (int)(-camera) % SCREEN_WIDTH;
        if (isFly)
            angle = 0;
        if (source.y <= 0 && isFly)
            source.y = 0;

        if (!isDead && !isComplete && isPlaying)
        {
            if (!Mix_PlayingMusic())
            {
                Mix_PlayMusic(bgMusic, -1);
            }
            velocityY += gravity * deltaTime;
            float MAX_FALL_SPEED = 12.0f;

            if (isFly)
            {
                MAX_FALL_SPEED = 5.0f;
            }
            if (velocityY > MAX_FALL_SPEED)
            {
                velocityY = MAX_FALL_SPEED;
            }
            if (speed * deltaTime <= speed)
            {

                source.x += speed * deltaTime;
            }
            else
            {
                source.x += speed;
            }
            if (velocityY * deltaTime <= MAX_FALL_SPEED)
            {

                source.y += velocityY * deltaTime;
            }
            else
            {
                source.y += MAX_FALL_SPEED;
            }

            if (source.y >= ground)
            {
                source.y = ground;
                velocityY = 0;
                angle = 0.0f;
                isJumping = false;
            }
            if (source.x > cameraCheckpoint)
            {
                float nextCamera = source.x - cameraCheckpoint;
                if (nextCamera <= 23000 - cameraCheckpoint)
                {
                    camera = nextCamera;
                }
                else
                {
                    camera = 23000 - cameraCheckpoint;
                }
            }
            if (isJumping && !isFly)
            {
                angle += 12.0f;
            }
            SDL_Rect playerRects = {(int)(source.x - camera), source.y, source.w - 20, source.h - 30};
            for (const auto &spike : map.getSpikes())
            {
                SDL_Rect spikeRect = {spike.x - (int)camera, spike.y, spike.w - 30, spike.h - 30};
                if (SDL_HasIntersection(&playerRects, &spikeRect))
                {

                    isDead = true;
                    Mix_PlayChannel(-1, deathSound, 0);
                }
            }
            for (const auto &spike : map.getReverse())
            {
                SDL_Rect spikeRect = {spike.x - (int)camera, spike.y, spike.w - 30, spike.h - 30};
                if (SDL_HasIntersection(&playerRects, &spikeRect))
                {

                    isDead = true;
                    Mix_PlayChannel(-1, deathSound, 0);
                }
            }
            if (shottingLaze)
            {
                for (auto &laser : lasers)
                {
                    SDL_Rect laserRect = {laser.x, laser.y, laser.w, laser.h};
                    SDL_Rect playerRect = {(int)(source.x - camera), source.y, source.w, source.h};

                    if (SDL_HasIntersection(&playerRect, &laserRect) && shottingLaze)
                    {
                        isDead = true;
                        Mix_PlayChannel(-1, deathSound, 0);
                    }
                }
            }

            for (const auto &square : map.getSquares())
            {
                SDL_Rect squareRect = {square.x - (int)camera, square.y, square.w, square.h};

                if (SDL_HasIntersection(&playerRects, &squareRect))
                {

                    if (source.y >= square.y - 30 && source.x + 60 <= square.x + 20)
                    {
                        isDead = true;
                        Mix_PlayChannel(-1, deathSound, 0);
                        cout << source.y << " " << square.y << " " << source.x << " " << square.x + 20 << endl;
                        speed = 0;
                    }
                    else if (source.y + source.h - 30 <= square.y + 10)
                    {
                        isJumping = false;
                        source.y = square.y - source.h + 25;
                        velocityY = 0;
                        onBlock = true;
                        angle = 0;
                    }
                }
            }
            SDL_Rect portalRect = {map.getPortal().x - (int)camera,
                                   map.getPortal().y,
                                   map.getPortal().w,
                                   map.getPortal().h};
            SDL_Rect portalEndRect = {map.getPortalEnd().x - (int)camera,
                                      map.getPortalEnd().y,
                                      map.getPortalEnd().w,
                                      map.getPortalEnd().h};
            if (SDL_HasIntersection(&playerRects, &portalRect))
            {
                objectTexture = flyTexture;
                isFly = true;
            }
            if (SDL_HasIntersection(&playerRects, &portalEndRect))
            {
                isComplete = true;
                Mix_PlayChannel(-1, winSound, 0);
            }
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                return 0;
            }
            if ((isDead || isComplete) && event.type == SDL_MOUSEMOTION)
            {

                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                isMouseOverRestart = (mouseX >= restartButton.x &&
                                      mouseX <= restartButton.x + restartButton.w &&
                                      mouseY >= restartButton.y &&
                                      mouseY <= restartButton.y + restartButton.h);
            }
            if (!isPlaying && event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (mouseX >= playButton.x &&
                        mouseX <= playButton.x + playButton.w &&
                        mouseY >= playButton.y &&
                        mouseY <= playButton.y + playButton.h)
                    {
                        isPlaying = true;
                        bgMusic = Mix_LoadMUS("../../src/BackOnTrack.mp3");
                        if (!bgMusic)
                        {
                            cerr << "background music loi " << Mix_GetError() << endl;
                        }
                    }
                }
            }
            if ((isDead || isComplete) && event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (mouseX >= restartButton.x &&
                        mouseX <= restartButton.x + restartButton.w &&
                        mouseY >= restartButton.y &&
                        mouseY <= restartButton.y + restartButton.h)
                    {

                        objectTexture = IMG_LoadTexture(renderer, "../../src/gameObject.png");
                        skullTexture = IMG_LoadTexture(renderer, "../../src/skull-close.png");
                        lastTime = SDL_GetTicks();
                        speed = First_SPEED;
                        jumpVelocity = -25.0f;
                        gravity = 1.0f;
                        l1 = false;
                        l2 = false;
                        l3 = false;
                        l4 = false;
                        lasers.clear();
                        isFly = false;
                        skullAppearTime = 0;
                        shottingLaze = false;
                        skullHasAppeared = false;
                        skullRect = {10000, 210, 150, 300};
                        velocityY = 9.0f;
                        isJumping = false;
                        cameraCheckpoint = 500.0f;
                        cameraSpeed = speed;
                        camera = 0;
                        angle = 0.0f;
                        playerAlpha = 255;
                        isDead = false;
                        isComplete = false;
                        source.x = InitX;
                        source.y = InitY;
                    }
                }
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:

                    if (!isJumping && angle == 0 && !isFly)
                    {
                        velocityY = jumpVelocity;
                        isJumping = true;
                    }
                    if (isFly)
                    {
                        source.y -= 75;
                    }

                    break;
          
                }
            }
        }

        SDL_RenderClear(renderer);

        SDL_Rect bgRect = {bgX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect bgRect2 = {bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, background, NULL, &bgRect);
        SDL_RenderCopy(renderer, background, NULL, &bgRect2);
        map.render(renderer, camera);
        SDL_Rect playerRect = {(int)(source.x - camera), source.y, source.w, source.h};
        SDL_Point center = {source.w / 2, source.h / 2};
        SDL_RenderCopyEx(renderer, objectTexture, NULL, &playerRect, angle, &center, SDL_FLIP_NONE);
        if (camera >= 10000 - SCREEN_WIDTH)
        {
            SDL_RenderCopy(renderer, skullTexture, NULL, &skullRect);
        }
        if (shottingLaze)
        {

            for (auto &laser : lasers)
            {
                SDL_RenderCopy(renderer, laserTexture, NULL, &laser);
            }
        }
        if (!isPlaying)
        {

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &overlay);

            string message = "Press to play";
            SDL_Color textColor = {255, 255, 255, 255};

            TTF_Font *largeFont = TTF_OpenFont("../../src/arial.ttf", 48);
            SDL_Surface *msgSurface = TTF_RenderText_Blended(largeFont, message.c_str(), textColor);
            SDL_Texture *msgTexture = SDL_CreateTextureFromSurface(renderer, msgSurface);
            SDL_Rect msgRect = {
                SCREEN_WIDTH / 2 - msgSurface->w / 2,
                SCREEN_HEIGHT / 2 - 100,
                msgSurface->w,
                msgSurface->h};
            SDL_RenderCopy(renderer, msgTexture, NULL, &msgRect);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_RenderFillRect(renderer, &playButton);

            SDL_Surface *btnSurface = TTF_RenderText_Blended(font, "Play", {0, 0, 0, 255});
            SDL_Texture *btnTexture = SDL_CreateTextureFromSurface(renderer, btnSurface);
            SDL_Rect btnTextRect = {
                SCREEN_WIDTH / 2 - btnSurface->w / 2,
                SCREEN_HEIGHT / 2 + 115,
                btnSurface->w,
                btnSurface->h};
            SDL_RenderCopy(renderer, btnTexture, NULL, &btnTextRect);
        }
        if (isDead || isComplete)
        {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &overlay);

            string message = isComplete ? "Level Complete!" : "Game Over!";
            SDL_Color textColor = {255, 255, 255, 255};

            TTF_Font *largeFont = TTF_OpenFont("../../src/arial.ttf", 48);
            SDL_Surface *msgSurface = TTF_RenderText_Blended(largeFont, message.c_str(), textColor);
            SDL_Texture *msgTexture = SDL_CreateTextureFromSurface(renderer, msgSurface);
            SDL_Rect msgRect = {
                SCREEN_WIDTH / 2 - msgSurface->w / 2,
                SCREEN_HEIGHT / 2 - 100,
                msgSurface->w,
                msgSurface->h};
            SDL_RenderCopy(renderer, msgTexture, NULL, &msgRect);

            string instruction = "Press to Restart";
            SDL_Surface *instSurface = TTF_RenderText_Blended(font, instruction.c_str(), textColor);
            SDL_Texture *instTexture = SDL_CreateTextureFromSurface(renderer, instSurface);
            SDL_Rect instRect = {
                SCREEN_WIDTH / 2 - instSurface->w / 2,
                SCREEN_HEIGHT / 2 + 50,
                instSurface->w,
                instSurface->h};
            SDL_RenderCopy(renderer, instTexture, NULL, &instRect);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_RenderFillRect(renderer, &restartButton);

            SDL_Surface *btnSurface = TTF_RenderText_Blended(font, "Restart", {0, 0, 0, 255});
            SDL_Texture *btnTexture = SDL_CreateTextureFromSurface(renderer, btnSurface);
            SDL_Rect btnTextRect = {
                SCREEN_WIDTH / 2 - btnSurface->w / 2,
                SCREEN_HEIGHT / 2 + 115,
                btnSurface->w,
                btnSurface->h};
            SDL_RenderCopy(renderer, btnTexture, NULL, &btnTextRect);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(background);
    SDL_DestroyTexture(objectTexture);
    SDL_DestroyTexture(flyTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(skullTexture);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}