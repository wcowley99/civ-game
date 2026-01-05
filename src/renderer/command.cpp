#include "command.hpp"

#include "vkb.hpp"

CommandBuilder CommandBuilder::Create(vk::CommandBuffer cmd) {
  cmd.reset();

  auto begin_info = vk::CommandBufferBeginInfo(
      vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
  cmd.begin(begin_info);

  CommandBuilder builder;
  builder.m_Cmd = cmd;

  return builder;
}

void CommandBuilder::Present(vk::Image &image) { m_Cmd.end(); }

void CommandBuilder::Transition(vk::Image image, vk::ImageLayout prev,
                                vk::ImageLayout next) {
  auto aspect_mask = (next == vk::ImageLayout::eDepthAttachmentOptimal)
                         ? vk::ImageAspectFlagBits::eDepth
                         : vk::ImageAspectFlagBits::eColor;
  vk::ImageMemoryBarrier2 imageBarrier(
      vk::PipelineStageFlagBits2::eAllCommands,
      vk::AccessFlagBits2::eMemoryWrite,
      vk::PipelineStageFlagBits2::eAllCommands,
      vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
      prev, next, 0, 0, image,
      vk::ImageSubresourceRange(aspect_mask, 0, vk::RemainingMipLevels, 0,
                                vk::RemainingArrayLayers));

  vk::DependencyInfo dep_info;
  dep_info.imageMemoryBarrierCount = 1;
  dep_info.pImageMemoryBarriers = &imageBarrier;
  m_Cmd.pipelineBarrier2(dep_info);
}

void CommandBuilder::Clear(vk::Image &image, std::array<float, 4> color) {
  vk::ImageSubresourceRange clear_range(vk::ImageAspectFlagBits::eColor, 0,
                                        vk::RemainingMipLevels, 0,
                                        vk::RemainingArrayLayers);

  vk::ClearColorValue clear_color(color);
  m_Cmd.clearColorImage(image, vk::ImageLayout::eGeneral, &clear_color, 1,
                        &clear_range);
}
