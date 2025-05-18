#pragma once
#include <SDL3/SDL.h>
#include <vector>

class PPU
{
public:
  PPU(int width, int height);
  ~PPU();

  void SetPixel(int x, int y, uint32_t color);
  void Update();
  void Clear(uint32_t color);

private:
  int width, height;
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Texture* texture = nullptr;
  std::vector<uint32_t> framebuffer;
};
