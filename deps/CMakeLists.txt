cmake_minimum_required(VERSION 3.20)

add_subdirectory(VulkanMemoryAllocator)

set(INCLUDE_DIRS
  ${VMA_SOURCE_DIR}/include
)

set (INCLUDE_DIRS
  ${INCLUDE_DIRS}
  PARENT_SCOPE
)
