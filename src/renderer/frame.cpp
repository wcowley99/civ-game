#include "frame.hpp"
#include "swapchain.hpp"
#include "vkb.hpp"

void FrameContext::Init(vk::Device device, uint32_t queue_family_index) {
  m_Device = device;

  vk::CommandPoolCreateInfo command_pool_info(
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queue_family_index);

  vk::FenceCreateInfo fence_info(vk::FenceCreateFlagBits::eSignaled);
  vk::SemaphoreCreateInfo semaphore_info;

  m_Pool = device.createCommandPool(command_pool_info);
  vk::CommandBufferAllocateInfo alloc_info(m_Pool,
                                           vk::CommandBufferLevel::ePrimary, 1);
  m_Cmd = device.allocateCommandBuffers(alloc_info)[0];
  m_GpuDone = device.createFence(fence_info);
  m_ImageAvailable = device.createSemaphore(semaphore_info);
  m_RenderFinished = device.createSemaphore(semaphore_info);
}

void FrameContext::Destroy() {
  m_Device.destroyCommandPool(m_Pool);

  m_Device.destroyFence(m_GpuDone);
  m_Device.destroySemaphore(m_ImageAvailable);
  m_Device.destroySemaphore(m_RenderFinished);
}

vk::Result FrameContext::Show(Swapchain &swapchain, vk::Queue graphics_queue,
                              uint32_t image_index) {
  vk::CommandBufferSubmitInfo submit_info(m_Cmd);
  vk::SemaphoreSubmitInfo wait_info(
      m_ImageAvailable, 1, vk::PipelineStageFlagBits2::eColorAttachmentOutput);
  vk::SemaphoreSubmitInfo signal_info(m_RenderFinished, 1,
                                      vk::PipelineStageFlagBits2::eAllGraphics);
  vk::SubmitInfo2 submit({}, 1, &wait_info, 1, &submit_info, 1, &signal_info);
  if (graphics_queue.submit2(1, &submit, m_GpuDone) != vk::Result::eSuccess) {
    throw std::runtime_error("Failed Submit2");
  }

  auto present = swapchain.GetPresentInfo(&m_RenderFinished, &image_index);
  return graphics_queue.presentKHR(present);
}

void FrameQueue::Init(vk::Device device, uint32_t queue_family_index,
                      uint32_t count) {
  m_Frames.resize(count);
  m_Index = 0;

  for (auto &frame : m_Frames) {
    frame.Init(device, queue_family_index);
  }
}

void FrameQueue::Destroy() {
  for (auto frame : m_Frames) {
    frame.Destroy();
  }
}

FrameContext &FrameQueue::Next() {
  auto &frame = m_Frames[m_Index];
  m_Index = (m_Index + 1) % m_Frames.size();

  return frame;
}
