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


void drawLineH(int x0, int y0, int x1, int y1, int color, std::vector<uint32_t> &pixels) {
    if (x0 > x1) {
        int temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int dx = x1-x0;
    int dy = y1 - y0;

    int dir = (dy < 0) ? -1 : 1;
    dy *= dir;

    if (dx != 0) {
        int y = y0;
        int p = 2*dy - dx;
        for (int i = 0; i < dx+1; i++) {
            putPixel(x0+i, y, color, pixels);
            if (p >= 0) {
                y += dir;
                p -= 2*dx;
            }
            p += 2*dy;
        }
    }
}

void drawLineV(int x0, int y0, int x1, int y1, int color, std::vector<uint32_t> &pixels) {
    if (y0 > y1) {
        int temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int dx = x1-x0;
    int dy = y1-y0;

    int dir = (dx < 0) ? -1 : 1;
    dx *= dir;

    if (dy != 0) {
        int x = x0;
        int p = 2*dx - dy;
        for (int i = 0; i < dy+1; i++) {
            putPixel(x, y0+i, color, pixels);
            if (p >= 0) {
                x += dir;
                p -= 2*dy;
            }
            p += 2*dx;
        }
    }
}

void drawLine(int x0, int y0, int x1, int y1, int color, std::vector<uint32_t> &pixels) {
    if (std::abs(x1 - x0) > std::abs(y1 - y0)) {
        drawLineH(x0, y0, x1, y1, color, pixels);
    } else {
        drawLineV(x0, y0, x1, y1, color, pixels);
    }
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
        //drawLine(5, 5, 5, 5, 0xFFFFFFFF, pixels);
        //drawLine(0, 5, 10, 5, 0xFFFFFFFF, pixels);
        drawLine(0, 0, 30, -100, 0xFFFFFFFF, pixels);

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