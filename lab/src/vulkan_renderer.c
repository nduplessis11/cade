#include "vulkan_renderer.h"
#include "cade_assert.h"
#include "defines.h"
#include "logger.h"
#include "vulkan_image.h"
#include "vulkan_types.h"
#include "vulkan_utils.h"
#include <vulkan/vulkan_core.h>

void renderer_draw(vulkan_context_t *context) {
  // Wait until GPU finished rendering last frame. 1 second timeout.
  u32 current_frame = context->frame_number % FRAME_OVERLAP;
  VkResult vk_result = vkWaitForFences(
      context->device, 1, &context->frames[current_frame].render_fence, VK_TRUE,
      1000000000);
  result_t result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Frame %u render_fence ready", current_frame);
  vk_result = vkResetFences(context->device, 1,
                            &context->frames[current_frame].render_fence);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Frame %u render_fence reset", current_frame);

  // Request swapchain image
  u32 swapchain_image_index;
  vk_result =
      vkAcquireNextImageKHR(context->device, context->swapchain, 1000000000,
                            context->frames[current_frame].swapchain_semaphore,
                            NULL, &swapchain_image_index);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Frame %u: Next image at index %u aquired from swapchain.",
             current_frame, swapchain_image_index);

  // TEST
  PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2Func =
      (PFN_vkCmdPipelineBarrier2)vkGetDeviceProcAddr(context->device,
                                                     "vkCmdPipelineBarrier2");

  if (vkCmdPipelineBarrier2Func == NULL) {
    CADE_WARN("vkCmdPipelineBarrier2 is NULL. Function not available.");
  }

  // Record command command buffer
  VkCommandBufferBeginInfo begin_info = {0};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  VkCommandBuffer cmd = context->frames[current_frame].main_command_buffer;
  vk_result = vkResetCommandBuffer(cmd, 0);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Command Buffer reset.");
  vk_result = vkBeginCommandBuffer(cmd, &begin_info);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Command buffer recording...");

  CADE_DEBUG("context->swapchain_images[%u] = %u", swapchain_image_index,
             context->swapchain_images[swapchain_image_index]);
  image_transition(cmd, context->swapchain_images[swapchain_image_index],
                   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

  vk_result = vkEndCommandBuffer(cmd);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Command buffer stopped recording.");
}
