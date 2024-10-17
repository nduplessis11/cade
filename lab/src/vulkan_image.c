#include "vulkan_image.h"
#include <vulkan/vulkan_core.h>

void image_transition(VkCommandBuffer cmd, VkImage image,
                      VkImageLayout current_layout, VkImageLayout new_layout) {
  VkImageMemoryBarrier2 image_barrier = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2
  };
  image_barrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
  image_barrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
  image_barrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
  image_barrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;
  image_barrier.oldLayout = current_layout;
  image_barrier.newLayout = new_layout;

  VkImageAspectFlags aspect_mask = (new_layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
}
