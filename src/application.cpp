#include "application.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>

bool Application::Init() {
  if (!m_Renderer.Init()) {
    return false;
  }

  m_Quit = false;
  return true;
}

void Application::Shutdown() { m_Renderer.Shutdown(); }

void Application::Run() {
  while (!m_Quit) {
    HandleEvents();

    m_Renderer.Draw();
  }
}

void Application::HandleEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
      m_Quit = true;
    }

    if (e.type == SDL_EVENT_QUIT) {
      m_Quit = true;
    }
  }
}
