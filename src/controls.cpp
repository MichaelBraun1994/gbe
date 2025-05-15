#include "controls.hpp"

Controls::Controls()
{
  keyBindings[Button::Left] = SDLK_LEFT;
  keyBindings[Button::Right] = SDLK_RIGHT;
  keyBindings[Button::Up] = SDLK_UP;
  keyBindings[Button::Down] = SDLK_DOWN;
  keyBindings[Button::Select] = SDLK_BACKSPACE;
  keyBindings[Button::Start] = SDLK_RETURN;
  keyBindings[Button::PowerOff] = SDLK_ESCAPE;

  for (const auto& [action, _] : keyBindings)
  {
    buttonStates[action] = false;
  }
}

void Controls::Update()
{
  SDL_Event event;

  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
    {
      bool pressed = (event.type == SDL_EVENT_KEY_DOWN);
      SDL_Keycode scancode = event.key.key;

      for (const auto& [action, boundKey] : keyBindings)
      {
        if (scancode == boundKey)
        {
          buttonStates[action] = pressed;
        }
      }
    }
  }
}

bool Controls::IsPressed(const Button button) const
{
  auto it = buttonStates.find(button);
  return (it != buttonStates.end()) ? it->second : false;
}