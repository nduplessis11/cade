#include "vulkan_image.h"
#include <vulkan/vulkan_core.h>

VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspect_mask);

void image_transition(VkCommandBuffer cmd, VkImage image,
                      VkImageLayout current_layout, VkImageLayout new_layout) {
  VkImageMemoryBarrier2 image_barrier = {0};
  image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
  image_barrier.pNext = NULL;
  image_barrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
  image_barrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
  image_barrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
  image_barrier.dstAccessMask =
      VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;
  image_barrier.oldLayout = current_layout;
  image_barrier.newLayout = new_layout;

  VkImageAspectFlags aspect_mask =
      (new_layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR)
          ? VK_IMAGE_ASPECT_DEPTH_BIT
          : VK_IMAGE_ASPECT_COLOR_BIT;
  image_barrier.subresourceRange = image_subresource_range(aspect_mask);
  image_barrier.image = image;

  VkDependencyInfo dep_info = {0};
  dep_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
  dep_info.pNext = NULL;
  dep_info.imageMemoryBarrierCount = 1;
  dep_info.pImageMemoryBarriers = &image_barrier;

  vkCmdPipelineBarrier2(cmd, &dep_info);
}

VkImageSubresourceRange
image_subresource_range(VkImageAspectFlags aspect_mask) {
  VkImageSubresourceRange sub_image = {0};
  sub_image.aspectMask = aspect_mask;
  sub_image.baseMipLevel = 0;
  sub_image.levelCount = VK_REMAINING_MIP_LEVELS;
  sub_image.baseArrayLayer = 0;
  sub_image.layerCount = VK_REMAINING_ARRAY_LAYERS;

  return sub_image;
}
