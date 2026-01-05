#pragma once

#include "swapchain.hpp"
#include "vkb.hpp"

#include <vector>

struct FrameContext {
  vk::Device m_Device;

  vk::CommandPool m_Pool;
  vk::CommandBuffer m_Cmd;

  vk::Semaphore m_ImageAvailable;
  vk::Semaphore m_RenderFinished;

  vk::Fence m_GpuDone;

  void Init(vk::Device device, uint32_t queue_family_index);

  void Destroy();

  vk::Result Show(Swapchain &swapchain, vk::Queue graphics_queue,
                  uint32_t image_index);
};

class FrameQueue {
public:
  void Init(vk::Device device, uint32_t queue_family_index, uint32_t count);

  void Destroy();

  FrameContext &Next();

private:
  std::vector<FrameContext> m_Frames;
  uint32_t m_Index;
};
