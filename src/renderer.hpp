#pragma once
#include <SDL3/SDL.h>
#include <vector>

class Renderer
{
public:
  Renderer(int width, int height);
  ~Renderer();

  void SetPixel(int x, int y, uint32_t color);
  void Update();
  void Clear(uint32_t color);
  bool ProcessEvents(); // Returns false if quit

private:
  int width, height;
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Texture* texture = nullptr;
  std::vector<uint32_t> framebuffer;
};
