#pragma once

#include "frame.hpp"
#include "swapchain.hpp"
#include "vkb.hpp"

#include <SDL3/SDL.h>

class Renderer {
public:
  bool Init();
  void Shutdown();

  void Draw();

private:
  void InitDevice();

private:
  SDL_Window *m_Window;

  vk::Instance m_Instance;
  vk::SurfaceKHR m_Surface;
  vk::PhysicalDevice m_PhysicalDevice;
  vk::Device m_Device;

  vk::Queue m_Queue;
  uint32_t m_QueueFamilyIndex;

  Swapchain m_Swapchain;

  FrameQueue m_FrameQueue;

  vk::DebugUtilsMessengerEXT m_DebugMessenger;
};
