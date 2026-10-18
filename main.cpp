#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <cstdint>
#include "math/Math.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;

void putPixel(float x, float y, uint32_t color, std::vector<uint32_t> &pixels) {
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        throw std::invalid_argument("Invalid Bounds");
    }
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    pixels[(int(iy) * WIDTH) + int(ix)] = color;
}

void putPixel(const math::Vec2 pos, uint32_t color, std::vector<uint32_t> &pixels) {
    float x = pos.x;
    float y = pos.y;
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        throw std::invalid_argument("Invalid Bounds");
    }
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    pixels[(int(iy) * WIDTH) + int(ix)] = color;
}


void drawLineH(math::Vec2 vec0, math::Vec2 vec1, int color, std::vector<uint32_t> &pixels) {
    float x0 = vec0.x;
    float y0 = vec0.y;
    float x1 = vec1.x;
    float y1 = vec1.y;
    if (x0 > x1) {
        float temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    float dx = x1-x0;
    float dy = y1 - y0;

    float dir = (dy < 0) ? -1 : 1;
    dy *= dir;

    if (dx != 0) {
        float y = y0;
        float p = 2*dy - dx;
        for (float i = 0; i < dx+1; i++) {
            putPixel(x0+i, y, color, pixels);
            if (p >= 0) {
                y += dir;
                p -= 2*dx;
            }
            p += 2*dy;
        }
    }
}

void drawLineV(math::Vec2 vec0, math::Vec2 vec1, int color, std::vector<uint32_t> &pixels) {
    float x0 = vec0.x;
    float y0 = vec0.y;
    float x1 = vec1.x;
    float y1 = vec1.y;
    if (y0 > y1) {
        int temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    float dx = x1-x0;
    float dy = y1-y0;

    float dir = (dx < 0) ? -1 : 1;
    dx *= dir;

    if (dy != 0) {
        float x = x0;
        float p = 2*dx - dy;
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

void drawLine(math::Vec2 vec0, math::Vec2 vec1, int color, std::vector<uint32_t> &pixels) {
    float x0 = vec0.x;
    float y0 = vec0.y;
    float x1 = vec1.x;
    float y1 = vec1.y;
    if (std::abs(x1 - x0) > std::abs(y1 - y0)) {
        drawLineH(vec0, vec1, color, pixels);
    } else {
        drawLineV(vec0, vec1, color, pixels);
    }
}

void drawWireFrameTriangle(math::Vec2 vec0, math::Vec2 vec1, math::Vec2 vec2, int color, std::vector<uint32_t> &pixels) {
    drawLine(vec0, vec1, color, pixels);
    drawLine(vec1, vec2, color, pixels);
    drawLine(vec2, vec0, color, pixels);
}

float edge_cross(math::Vec2 a, math::Vec2 b, math::Vec2 p) {
    math::Vec2 ab = b - a;
    math::Vec2 ap = p - a;
    return ab.cross(ap);
}

bool is_top_left(math::Vec2 start, math::Vec2 end) {
    math::Vec2 edge = end - start;

    bool top = edge.y == 0 && edge.x > 0;
    bool left = edge.y < 0;

    return top || left;
}

void triangleFill(math::Vec2 vec0, math::Vec2 vec1, math::Vec2 vec2, int color, std::vector<uint32_t> &pixels) {
    int xMin = std::min(vec0.x, std::min(vec1.x, vec2.x));
    int yMin = std::min(vec0.y, std::min(vec1.y, vec2.y));
    int xMax = std::max(vec0.x, std::max(vec1.x, vec2.x));
    int yMax = std::max(vec0.y, std::max(vec1.y, vec2.y));

    int bias0 = is_top_left(vec1, vec2) ? 0 : -1;
    int bias1 = is_top_left(vec2, vec0) ? 0 : -1;
    int bias2 = is_top_left(vec0, vec1) ? 0 : -1;

    for (int y = yMin; y <= yMax; y++){
        for (int x = xMin; x<= xMax; x++) {
            math::Vec2 p = math::Vec2(x, y);

            int w0 = edge_cross(vec0, vec1, p) + bias0;
            int w1 = edge_cross(vec2, vec0, p) + bias1;
            int w2 = edge_cross(vec0, vec1, p) + bias2;

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                putPixel(x, y, color, pixels);
            }
        }
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
        
        math::Vec2 v0 = math::Vec2(300, 300);
        math::Vec2 v1 = math::Vec2(100, 500);
        math::Vec2 v2 = math::Vec2(500, 500);

        //drawWireFrameTriangle(v0, v1, v2, 0xFFFFFFFF, pixels);
        triangleFill(v0, v2, v1, 0xFFFFFFFF, pixels);
        

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