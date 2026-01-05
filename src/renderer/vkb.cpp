#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1

#include "vkb.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vector>

VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT messageType,
               const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
               void *pUserData) {
  SDL_Log("validation layer: %s", pCallbackData->pMessage);
  return VK_FALSE;
}

vk::Instance vkb::CreateInstance() {
  vk::ApplicationInfo app_info("Hello, World", VK_MAKE_VERSION(1, 0, 0),
                               "No Engine", VK_MAKE_VERSION(1, 0, 0),
                               VK_API_VERSION_1_3);
  uint32_t count = 0;
  auto sdl_extensions = SDL_Vulkan_GetInstanceExtensions(&count);
  auto instance_extensions =
      std::vector<const char *>(sdl_extensions, sdl_extensions + count);

  instance_extensions.push_back("VK_EXT_debug_utils");
  instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

  auto available_layers = vk::enumerateInstanceLayerProperties();
  for (auto layer : available_layers) {
    SDL_Log("%s\n", layer.layerName);
  }

  auto layers = std::vector<const char *>{"VK_LAYER_KHRONOS_validation"};

  auto instance = vk::createInstance(vk::InstanceCreateInfo{
      vk::InstanceCreateFlags{VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR},
      &app_info, (uint32_t)layers.size(), layers.data(),
      (uint32_t)instance_extensions.size(), instance_extensions.data()});

  VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

  return instance;
}

vk::DebugUtilsMessengerEXT vkb::CreateDebugMessenger(vk::Instance instance) {
  return instance.createDebugUtilsMessengerEXT(
      vk::DebugUtilsMessengerCreateInfoEXT{
          {},
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
              vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
              vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
              vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
          vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
              vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
              vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
          vk::PFN_DebugUtilsMessengerCallbackEXT(debug_callback)},
      nullptr);
}

vk::SurfaceKHR vkb::CreateSurface(SDL_Window *window, vk::Instance instance) {
  VkSurfaceKHR surface;
  if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
    throw std::runtime_error(
        std::string("SDL could not create window! SDL error: ") +
        SDL_GetError());
  }

  return surface;
}

vk::Device vkb::CreateDevice(vk::PhysicalDevice gpu, uint32_t queue_family) {
  std::vector<const char *> extensions = {vk::KHRSwapchainExtensionName};

  vk::StructureChain<vk::PhysicalDeviceFeatures2,
                     vk::PhysicalDeviceVulkan12Features,
                     vk::PhysicalDeviceVulkan13Features>
      c;

  auto &features12 = c.get<vk::PhysicalDeviceVulkan12Features>();

  features12.setBufferDeviceAddress(vk::True);
  features12.setDescriptorIndexing(vk::True);

  auto &features13 = c.get<vk::PhysicalDeviceVulkan13Features>();

  features13.setDynamicRendering(vk::True);
  features13.setSynchronization2(vk::True);

  auto features = c.get();

  float priority = 1.0f;
  auto queue_create_info =
      vk::DeviceQueueCreateInfo({}, queue_family, 1, &priority);

  return gpu.createDevice(vk::DeviceCreateInfo({}, 1, &queue_create_info, 0,
                                               nullptr, extensions.size(),
                                               extensions.data())
                              .setPNext(&features));
}

bool vkb::SuitableVulkanVersion(vk::PhysicalDevice device) {
  auto props = device.getProperties();

  return props.apiVersion >= VK_API_VERSION_1_3;
}

bool vkb::RequiredFeatures(vk::PhysicalDevice device) {
  auto features = device.getFeatures2<vk::PhysicalDeviceFeatures2,
                                      vk::PhysicalDeviceVulkan12Features,
                                      vk::PhysicalDeviceVulkan13Features>();
  auto features12 = features.get<vk::PhysicalDeviceVulkan12Features>();
  auto features13 = features.get<vk::PhysicalDeviceVulkan13Features>();

  return features12.bufferDeviceAddress && features12.descriptorIndexing &&
         features13.dynamicRendering && features13.synchronization2;
}
