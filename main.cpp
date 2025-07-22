#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <ctime>
#include <iostream>
#include <string>
struct Button {
    SDL_Rect rect;
    SDL_Color color;
    std::string text;
};
// Game states
enum GameState { MENU, PLAYING, PAUSED, GAME_OVER, VICTORY };

GameState gameState = MENU;
int score = 0;
int lives = 3;
SDL_Texture* heartTex = nullptr;

char targetKey = 'A';

char GetRandomKey() {
    const char keys[] = { 'W','S','A','D' };
    return keys[rand() % 4];
}

Uint32 gameStartTime = 0;
int timeLimit = 30;

bool Init(SDL_Window** window, SDL_Renderer** renderer, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() == -1) return false;

    *window = SDL_CreateWindow("Etapa 8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    return *window && *renderer;
}

SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Error loading texture: " << path << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* RenderText(const std::string& msg, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, msg.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void RenderHUD(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Texture* scoreTex = RenderText("Score: " + std::to_string(score), font, white, renderer);
    SDL_Rect dest = { 20, 20, 0, 0 };
    SDL_QueryTexture(scoreTex, nullptr, nullptr, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, scoreTex, nullptr, &dest);
    SDL_DestroyTexture(scoreTex);
    for (int i = 0; i < lives; i++) {
        SDL_Rect heartRect = { 20 + i * 40, 60, 32, 32 };
        SDL_RenderCopy(renderer, heartTex, nullptr, &heartRect);
    }
    if (gameState == PLAYING) {
        Uint32 now = SDL_GetTicks();
        int timeLeft = timeLimit - (now - gameStartTime) / 1000;
        if (timeLeft < 0) timeLeft = 0;

        SDL_Texture* timeTex = RenderText("Time: " + std::to_string(timeLeft), font, white, renderer);
        SDL_Rect tRect = { 700,20,0,0 };
        SDL_QueryTexture(timeTex, nullptr, nullptr, &tRect.w, &tRect.h);
        SDL_RenderCopy(renderer, timeTex, nullptr, &tRect);
        SDL_DestroyTexture(timeTex);
    }
}

void RenderButton(SDL_Renderer* renderer, TTF_Font* font, Button btn) {
    SDL_SetRenderDrawColor(renderer, btn.color.r, btn.color.g, btn.color.b, 255);
    SDL_RenderFillRect(renderer, &btn.rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &btn.rect);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Texture* tex = RenderText(btn.text, font, white, renderer);
    int texW = 0, texH = 0;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    SDL_Rect textRect{
        btn.rect.x + (btn.rect.w - texW)/2,
        btn.rect.y + (btn.rect.h - texH)/2,
        texW, texH
    };
    SDL_RenderCopy(renderer, tex, nullptr, &textRect);
    SDL_DestroyTexture(tex);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!Init(&window, &renderer, 800, 600)) return 1;

    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font\n";
        return 1;
    }
    Button playButton = { {300,250,200,60}, {0,120,255}, "PLAY"};
    Button restartButton = { {300,330,200,60}, {0,200,100}, "RESTART"};
    bool running = true;
    SDL_Event event;
    Uint32 lastTick = SDL_GetTicks();
    
    //SDL_Surface* heartSurface = IMG_Load("assets/heart.png");
    //if (!heartSurface) return 1;
    heartTex = LoadTexture("assets/heart.png", renderer);
    if (!heartTex) return 1;
    
    srand(static_cast<unsigned>(time(nullptr)));
    targetKey = GetRandomKey();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                SDL_Point clickPoint = { x,y };

                switch (gameState) {
                case MENU:
                    if (SDL_PointInRect(&clickPoint, &playButton.rect)) {
                        gameState = PLAYING;
                        score = 0;
                        lives = 3;
                        targetKey = GetRandomKey();
                        gameStartTime = SDL_GetTicks();
                    }
                    break;
                case GAME_OVER:
                    if (SDL_PointInRect(&clickPoint, &restartButton.rect)) {
                        score = 0;
                        lives = 3;
                        gameState = MENU;
                    }
                    break;
                case VICTORY:
                    if (SDL_PointInRect(&clickPoint, &restartButton.rect)) {
                        score = 0;
                        lives = 3;
                        gameState = MENU;
                    }
                    break;
                }
            }
            if (event.type == SDL_KEYDOWN) {
                switch (gameState) {
                case PLAYING: {
                    /*if (event.key.keysym.sym == SDLK_SPACE) {
                        score += 10;
                        if (score >= 100) gameState = VICTORY;
                    }
                    if (event.key.keysym.sym == SDLK_d) {
                        lives--;
                        if (lives <= 0) gameState = GAME_OVER;
                    }*/
                    
                    SDL_Keycode pressed = event.key.keysym.sym;
                    if (pressed >= SDLK_a && pressed <= SDLK_z) {
                        if (toupper(pressed) == targetKey) {
                            score += 10;
                            targetKey = GetRandomKey();
                            if (score >= 100) gameState = VICTORY;
                        }
                        else {
                            lives--;
                            if (lives <= 0) gameState = GAME_OVER;
                        }
                    }
                    Uint32 now = SDL_GetTicks();
                    int timeLeft = timeLimit - (now - gameStartTime) / 1000;
                    if (timeLeft <= 0) {
                        gameState = GAME_OVER;
                    }
                    if (event.key.keysym.sym == SDLK_p) {
                        gameState = PAUSED;
                    }
                    break;
                }
                case PAUSED:
                    if (event.key.keysym.sym == SDLK_p) {
                        gameState = PLAYING;
                    }
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        gameState = MENU;
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        switch (gameState) {
            case MENU: {
                /*SDL_Color c = {255, 255, 0, 255};
                SDL_Texture* t = RenderText("Press ENTER to Start", font, c, renderer);
                SDL_Rect r = { 200, 250, 0, 0 };
                SDL_QueryTexture(t, nullptr, nullptr, &r.w, &r.h);
                SDL_RenderCopy(renderer, t, nullptr, &r);
                SDL_DestroyTexture(t);*/
                RenderButton(renderer, font, playButton);
                break;
            }
            case PLAYING: {
                RenderHUD(renderer, font);
                std::string msg = "Press: ";
                msg += targetKey;
                SDL_Color c = { 0, 255, 255, 255 };
                SDL_Texture* t = RenderText(msg, font, c, renderer);
                SDL_Rect r = { 300,100,0,0 };
                SDL_QueryTexture(t, nullptr, nullptr, &r.w, &r.h);
                SDL_RenderCopy(renderer, t, nullptr, &r);
                SDL_DestroyTexture(t);
                break;
            }
            case VICTORY: {
                SDL_Color c = { 0, 255, 0, 255 };
                SDL_Texture* t = RenderText("You Win!", font, c, renderer);
                SDL_Rect r = { 150, 250, 0, 0 };
                SDL_QueryTexture(t, nullptr, nullptr, &r.w, &r.h);
                SDL_RenderCopy(renderer, t, nullptr, &r);
                SDL_DestroyTexture(t);
                RenderButton(renderer, font, restartButton);
                break;
            }
            case GAME_OVER: {
                SDL_Color c = { 255, 0, 0, 255 };
                SDL_Texture* t = RenderText("Game Over", font, c, renderer);
                SDL_Rect r = { 150, 250, 0, 0 };
                SDL_QueryTexture(t, nullptr, nullptr, &r.w, &r.h);
                SDL_RenderCopy(renderer, t, nullptr, &r);
                SDL_DestroyTexture(t);
                RenderButton(renderer, font, restartButton);
                break;
            }
            case PAUSED: {
                SDL_Color c = { 255, 255, 255, 255 };
                SDL_Texture* t = RenderText("Game Paused. Press P to Resume or ESC to Menu", font, c, renderer);
                SDL_Rect r = { 100, 250, 0, 0 };
                SDL_QueryTexture(t, nullptr, nullptr, &r.w, &r.h);
                SDL_RenderCopy(renderer, t, nullptr, &r);
                SDL_DestroyTexture(t);
                break;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(heartTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}