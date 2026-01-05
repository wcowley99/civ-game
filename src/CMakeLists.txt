cmake_minimum_required(VERSION 3.20)

set(
  PROJECT_SOURCES
  "main.cpp"
  "application.cpp"

  "renderer/command.cpp"
  "renderer/frame.cpp"
  "renderer/renderer.cpp"
  "renderer/swapchain.cpp"
  "renderer/vkb.cpp"
)

add_executable(${PROJECT_NAME}
  ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL3::SDL3
  Vulkan::Headers
  GPUOpen::VulkanMemoryAllocator
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${INCLUDE_DIRS}
)
