#include "renderer.hpp"
#include "command.hpp"
#include "vkb.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>

bool Renderer::Init() {
  VULKAN_HPP_DEFAULT_DISPATCHER.init();

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return false;
  }

  m_Window = SDL_CreateWindow("Civ Game", 1280, 720, SDL_WINDOW_VULKAN);
  if (!m_Window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  m_Instance = vkb::CreateInstance();
  m_DebugMessenger = vkb::CreateDebugMessenger(m_Instance);

  m_Surface = vkb::CreateSurface(m_Window, m_Instance);

  InitDevice();

  m_Swapchain.Init(m_Device, m_PhysicalDevice, m_Surface);

  m_FrameQueue.Init(m_Device, m_QueueFamilyIndex, m_Swapchain.NumImages());

  return true;
}

void Renderer::InitDevice() {
  auto devices = m_Instance.enumeratePhysicalDevices();
  for (auto device : devices) {
    if (!vkb::SuitableVulkanVersion(device) || !vkb::RequiredFeatures(device)) {
      continue;
    }

    auto props = device.getQueueFamilyProperties();
    uint32_t queue_family_index = UINT32_MAX;

    for (uint32_t i = 0; i < props.size(); i += 1) {
      auto prop = props[i];

      vk::Bool32 supported;
      VK_ASSERT(device.getSurfaceSupportKHR(i, m_Surface, &supported));

      if (prop.queueFlags & vk::QueueFlagBits::eGraphics && supported) {
        queue_family_index = i;
        break;
      }
    }

    if (queue_family_index != UINT32_MAX) {
      std::cerr << "selected device: " << device.getProperties().deviceName
                << std::endl;

      m_PhysicalDevice = device;
      m_QueueFamilyIndex = queue_family_index;

      m_Device = vkb::CreateDevice(m_PhysicalDevice, m_QueueFamilyIndex);
      VULKAN_HPP_DEFAULT_DISPATCHER.init(m_Device);

      m_Queue = m_Device.getQueue(m_QueueFamilyIndex, 0);
      return;
    }
  }
}

void Renderer::Shutdown() {
  m_Device.waitIdle();

  m_FrameQueue.Destroy();
  m_Swapchain.Destroy();

  m_Device.destroy();
  m_Instance.destroySurfaceKHR(m_Surface);
  m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger);
  m_Instance.destroy();

  SDL_DestroyWindow(m_Window);
  SDL_Quit();
}

void Renderer::Draw() {
  auto &current_frame = m_FrameQueue.Next();
  VK_ASSERT(m_Device.waitForFences(1, &current_frame.m_GpuDone, vk::True,
                                   1000000000));

  auto result = m_Swapchain.NextImageIndex(current_frame.m_ImageAvailable);

  if (result.result == vk::Result::eErrorOutOfDateKHR) {
    m_Swapchain.Recreate();
    return;
  }

  VK_ASSERT(m_Device.resetFences(1, &current_frame.m_GpuDone));

  if (result.result != vk::Result::eSuccess &&
      result.result != vk::Result::eSuboptimalKHR) {
    std::cerr << result.result << std::endl;
    throw std::runtime_error("Failed to acquire Next Image");
  }
  auto image_index = result.value;
  auto swapchain_image = m_Swapchain.GetImage(image_index);
  std::array<float, 4> color = {1.0, 1.0, 0.0, 1.0};

  CommandBuilder cmd = CommandBuilder::Create(current_frame.m_Cmd);

  cmd.Transition(swapchain_image, vk::ImageLayout::eUndefined,
                 vk::ImageLayout::eGeneral);
  cmd.Clear(swapchain_image, color);
  cmd.Transition(swapchain_image, vk::ImageLayout::eGeneral,
                 vk::ImageLayout::ePresentSrcKHR);
  cmd.Present(swapchain_image);

  auto present = current_frame.Show(m_Swapchain, m_Queue, image_index);

  if (present == vk::Result::eErrorOutOfDateKHR ||
      present == vk::Result::eSuboptimalKHR) {
    m_Swapchain.Recreate();
  }

  SDL_UpdateWindowSurface(m_Window);
}
