#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>

enum class Button
{
  Left,
  Right,
  Up,
  Down,
  Select,
  Start,
  PowerOff
};

class Controls
{
  std::unordered_map<Button, SDL_Keycode> keyBindings;
  std::unordered_map<Button, bool> buttonStates;

public:
  Controls();

  void Update();
  [[nodiscard]] bool IsPressed(const Button button) const;
};