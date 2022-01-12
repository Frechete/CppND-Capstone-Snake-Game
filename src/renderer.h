#ifndef SRC_RENDERER_H_
#define SRC_RENDERER_H_

#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &snake, SDL_Point const &food,
              SDL_Point const &blockade);
  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  SDL_Texture *pixelsTexture;
  TTF_Font *font;

  SDL_Texture *draw_mandelbrot(SDL_Renderer *sdl_renderer, SDL_Surface *surface,
                               int size);
  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  int lastmod;
};

#endif  // SRC_RENDERER_H_
