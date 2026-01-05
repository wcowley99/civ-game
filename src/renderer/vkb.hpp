#pragma once

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include "vk_mem_alloc.h"
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.hpp>

#include <SDL3/SDL.h>

#define VK_ASSERT(x)                                                           \
  do {                                                                         \
    auto err = static_cast<VkResult>(x);                                       \
    if (err) {                                                                 \
      std::cout << "[" << __FILE__ << ":" << __LINE__ << "] "                  \
                << string_VkResult(err);                                       \
    }                                                                          \
  } while (false)

namespace vkb {

vk::Instance CreateInstance();

vk::DebugUtilsMessengerEXT CreateDebugMessenger(vk::Instance instance);

vk::SurfaceKHR CreateSurface(SDL_Window *window, vk::Instance instance);

vk::Device CreateDevice(vk::PhysicalDevice gpu, uint32_t queue_family);

bool SuitableVulkanVersion(vk::PhysicalDevice device);

bool RequiredFeatures(vk::PhysicalDevice device);
} // namespace vkb
