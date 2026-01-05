#include "swapchain.hpp"

void Swapchain::Init(vk::Device device, vk::PhysicalDevice physical_device,
                     vk::SurfaceKHR surface) {
  m_Device = device;
  m_PhysicalDevice = physical_device;
  m_Surface = surface;

  m_Format = vk::Format::eB8G8R8A8Srgb;
  m_Extent = vk::Extent2D(1280, 720);

  Create();
}

void Swapchain::Destroy() {
  for (auto image_view : m_ImageViews) {
    m_Device.destroyImageView(image_view);
  }

  m_Device.destroySwapchainKHR(m_Swapchain);

  m_Images.clear();
  m_ImageViews.clear();
}

void Swapchain::Recreate() {
  m_Device.waitIdle();

  Destroy();

  Create();
}

vk::ResultValue<uint32_t> Swapchain::NextImageIndex(vk::Semaphore semaphore) {
  return m_Device.acquireNextImageKHR(m_Swapchain, 1000000000, semaphore);
}

vk::Image Swapchain::GetImage(uint32_t index) { return m_Images[index]; }

vk::ImageView Swapchain::GetImageView(uint32_t index) {
  return m_ImageViews[index];
}

vk::PresentInfoKHR Swapchain::GetPresentInfo(vk::Semaphore *semaphore,
                                             uint32_t *image_index) {

  return vk::PresentInfoKHR(1, semaphore, 1, &m_Swapchain, image_index);
}

void Swapchain::Create() {
  auto capabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(m_Surface);
  auto formats = m_PhysicalDevice.getSurfaceFormatsKHR(m_Surface);

  uint32_t num_images = std::max(capabilities.minImageCount + 1, 3u);

  vk::SwapchainCreateInfoKHR swapchainCreateInfo(
      {}, m_Surface, num_images, m_Format, vk::ColorSpaceKHR::eSrgbNonlinear,
      m_Extent, 1,
      vk::ImageUsageFlagBits::eColorAttachment |
          vk::ImageUsageFlagBits::eTransferDst,
      vk::SharingMode::eExclusive, 0, nullptr,
      vk::SurfaceTransformFlagBitsKHR::eIdentity,
      vk::CompositeAlphaFlagBitsKHR::eOpaque, vk::PresentModeKHR::eFifo, true,
      nullptr);
  m_Swapchain = m_Device.createSwapchainKHR(swapchainCreateInfo);
  m_Images = m_Device.getSwapchainImagesKHR(m_Swapchain);

  for (auto image : m_Images) {
    auto create_info = vk::ImageViewCreateInfo(
        {}, image, vk::ImageViewType::e2D, m_Format,
        vk::ComponentMapping{vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG,
                             vk::ComponentSwizzle::eB,
                             vk::ComponentSwizzle::eA},
        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

    m_ImageViews.push_back(m_Device.createImageView(create_info));
  }
}
