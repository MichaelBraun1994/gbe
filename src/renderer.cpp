#include "renderer.hpp"
#include <stdexcept>

Renderer::Renderer(int width, int height) : width(width), height(height), framebuffer(width * height)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    throw std::runtime_error(SDL_GetError());
  }

  window = SDL_CreateWindow("CPU Framebuffer with SDL3", width, height, 0);
  if (!window)
    throw std::runtime_error(SDL_GetError());

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer)
    throw std::runtime_error(SDL_GetError());

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
  if (!texture)
    throw std::runtime_error(SDL_GetError());
}

Renderer::~Renderer()
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::SetPixel(int x, int y, uint32_t color)
{
  if (x >= 0 && x < width && y >= 0 && y < height)
    framebuffer[y * width + x] = color;
}

void Renderer::Update()
{
  SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * sizeof(uint32_t));
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void Renderer::Clear(uint32_t color)
{
  std::fill(framebuffer.begin(), framebuffer.end(), color);
}