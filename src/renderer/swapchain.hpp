#pragma once

#include "vkb.hpp"

#include <vector>

class Swapchain {
public:
  void Init(vk::Device device, vk::PhysicalDevice physical_device,
            vk::SurfaceKHR surface);
  void Destroy();

  void Recreate();

  uint32_t NumImages() { return m_Images.size(); }

  vk::ResultValue<uint32_t> NextImageIndex(vk::Semaphore semaphore);
  vk::Image GetImage(uint32_t index);
  vk::ImageView GetImageView(uint32_t index);

  vk::PresentInfoKHR GetPresentInfo(vk::Semaphore *semaphore,
                                    uint32_t *image_index);

private:
  void Create();

private:
  vk::Device m_Device;
  vk::PhysicalDevice m_PhysicalDevice;
  vk::SurfaceKHR m_Surface;

  vk::Format m_Format;
  vk::SwapchainKHR m_Swapchain;

  std::vector<vk::Image> m_Images;
  std::vector<vk::ImageView> m_ImageViews;

  vk::Extent2D m_Extent;
};
