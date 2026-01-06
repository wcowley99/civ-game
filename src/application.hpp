#pragma once

#include "renderer/renderer.hpp"

class Application {
public:
  bool Init();
  void Cleanup();

  void Run();

private:
  void HandleEvents();

private:
  Renderer m_Renderer;

  bool m_Quit;
};
