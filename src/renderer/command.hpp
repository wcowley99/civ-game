#pragma once

#include "vkb.hpp"

class CommandBuilder {
public:
  static CommandBuilder Create(vk::CommandBuffer cmd);

  void Present(vk::Image &image);

  void Transition(vk::Image image, vk::ImageLayout prev, vk::ImageLayout next);

  void Clear(vk::Image &image, std::array<float, 4> color);

private:
  vk::CommandBuffer m_Cmd;
};
