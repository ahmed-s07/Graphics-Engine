#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <cstdint>

const int WIDTH = 800;
const int HEIGHT = 600;

void putPixel(int x, int y, int color, std::vector<uint32_t> &pixels) {
    x += WIDTH/2;
    y = HEIGHT/2 - y; 
    pixels[(y * WIDTH) + x] = color;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Renderer",
        WIDTH, HEIGHT, 
        SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    std::vector<uint32_t> pixels(WIDTH * HEIGHT, 0x000000FF);

    bool running = true;
    SDL_Event event;


    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        std::fill(pixels.begin(), pixels.end(), 0x000000FF);
        for (int i = 0; i < 32; i++){
            for (int j = 0; j < 32; j++) {
                putPixel(i, j, 0xFF0000FF, pixels);
            }
        }

        SDL_UpdateTexture(texture, nullptr, pixels.data(), WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}