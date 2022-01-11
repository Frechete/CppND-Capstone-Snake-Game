#include "renderer.h"

#include <complex.h>

#include <cmath>
#include <iostream>
#include <random>
#include <string>

template <typename R>
R _random(R range_from, R range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<R> distr(range_from, range_to);
  return distr(generator);
}

// Adapted from
// https://github.com/lucas-santoni/mandelbrot-c-sdl2/blob/master/headers/myFractal.h
SDL_Texture *draw_mandelbrot(SDL_Renderer *sdl_renderer, SDL_Surface *surface,
                             int size) {
  typedef struct Complex {
    // Real part, imaginary part and a backup
    double r;
    double i;
    double b;
  } Complex;

  typedef struct Fractal {
    // See myFractal.h
    double xMove;
    double yMove;
    double zoom;
    float iMax;
  } Fractal;
#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 640
  int i;
  Fractal fractal;
  // Used to move camera
  fractal.xMove = _random<int>(-1, 1);
  fractal.yMove = _random<int>(-1, 1);
  (fractal.xMove > 0) ? (fractal.xMove -= 0.1) : (fractal.xMove += 0.1);
  (fractal.yMove > 0) ? (fractal.yMove -= 0.1) : (fractal.yMove += 0.1);
  // fractal.yMove = _random<int>(-5, 5) * size;
  int color = _random<int>(0, 255);
  // Used to change the zoom and precision
  fractal.zoom = static_cast<float>(_random<int>(1, 50) / 10.0);
  fractal.iMax = _random<int>(0 + size, 200);
  int xFrame = WINDOW_WIDTH;
  int yFrame = WINDOW_HEIGHT;

  // Formula is Z(n+1) = Z(n)^2 + C
  // https://en.wikipedia.org/wiki/Mandelbrot_set
  Complex c;
  Complex z;

  // Coordonate of each point
  int x;
  int y;
  // Calculate all the y for every x
  for (x = 0; x < xFrame; x++) {
    c.r = ((x + color - xFrame / 2) / (0.5 * xFrame * fractal.zoom)) -
          fractal.xMove;

    for (y = 0; y < yFrame; y++) {
      c.i = ((y - yFrame / 2) / (0.5 * yFrame * fractal.zoom)) - fractal.yMove;

      z.r = 0;
      z.i = 0;

      i = 0;

      // Iterate in order to know if a certain point is in the set or not
      do {
        z.b = z.r;
        z.r = z.r * z.r - z.i * z.i + c.r;
        z.i = 2 * z.i * z.b + c.i;
        i++;
      } while (i < fractal.iMax && (z.r * z.r + z.i * z.i < 4 + fractal.zoom));
      // We don't use square root in order to reduce calculation time

      if (i >= fractal.iMax) {
        // In the set
        reinterpret_cast<Uint32 *>(
            surface->pixels)[(x * surface->w) + y + size] = color;
      } else {
        // Not in the set
        reinterpret_cast<Uint32 *>(surface->pixels)[(y * surface->w) + x] =
            SDL_MapRGB(
                surface->format,
                (1 + sin(i * ((255 - color) / fractal.iMax) * 0.27 + 5)) *
                    127.0,
                (1 + cos(i * ((color) / fractal.iMax) * 0.85)) * 127.0,
                (1 + sin(i * ((255 * size) / fractal.iMax) * 0.15)) * 127.0);
      }
    }
  }
  return SDL_CreateTextureFromSurface(sdl_renderer, surface);
}

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      lastmod(1) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  SDL_Surface *surface = SDL_GetWindowSurface(sdl_window);
  pixelsTexture = draw_mandelbrot(sdl_renderer, surface, 0);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const &snake, SDL_Point const &food,
                      SDL_Point const &blockade) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  if (snake.body.size() != lastmod) {
    lastmod = snake.body.size();
    SDL_Surface *surface = SDL_GetWindowSurface(sdl_window);
    pixelsTexture = draw_mandelbrot(sdl_renderer, surface, lastmod);
  }

  SDL_RenderCopy(sdl_renderer, pixelsTexture, nullptr, nullptr);
  SDL_RenderPresent(sdl_renderer);

  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;

  SDL_RenderFillRect(sdl_renderer, &block);

  SDL_SetRenderDrawColor(sdl_renderer, 0x33 - lastmod, 0xCC + lastmod, 0xAA + lastmod, 0x33);
  block.x = blockade.x * block.w;
  block.y = blockade.y * block.h;

  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) +
                    " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
