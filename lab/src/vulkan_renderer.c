#include "vulkan_renderer.h"
#include "cade_assert.h"
#include "defines.h"
#include "logger.h"
#include "vulkan_frame.h"
#include "vulkan_image.h"
#include "vulkan_types.h"
#include "vulkan_utils.h"
#include <math.h>
#include <vulkan/vulkan_core.h>

void renderer_draw(vulkan_context_t *context) {
  // Wait until GPU finished rendering last frame. 1 second timeout.
  u32 current_frame = context->frame_number % FRAME_OVERLAP;
  CADE_DEBUG("Waiting for fences at frame: %u", current_frame);
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
  if (vk_result == VK_ERROR_OUT_OF_DATE_KHR) {
    context->resize_requested = TRUE;
    return;
  }
  CADE_DEBUG("Frame %u: Next image at index %u aquired from swapchain.",
             current_frame, swapchain_image_index);

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

  // Clear image. Flash with a 120 frame period.
  VkClearColorValue clear_value;
  f32 flash = fabsf(sinf(context->frame_number / 120.0f));
  clear_value.float32[0] = 0.0f;
  clear_value.float32[1] = 0.0f;
  clear_value.float32[2] = flash;
  clear_value.float32[3] = 1.0f;
  VkImageSubresourceRange clear_range =
      image_subresource_range(VK_IMAGE_ASPECT_COLOR_BIT);
  vkCmdClearColorImage(cmd, context->swapchain_images[swapchain_image_index],
                       VK_IMAGE_LAYOUT_GENERAL, &clear_value, 1, &clear_range);

  // Transition swapchain image into presentable mode
  image_transition(cmd, context->swapchain_images[swapchain_image_index],
                   VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

  vk_result = vkEndCommandBuffer(cmd);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Command buffer stopped recording.");

  // Submit command buffer to queue
  result = frame_submit_queue(context);
  CADE_ASSERT_DEBUG(result.success);

  // Present the queue
  VkPresentInfoKHR present_info = {0};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.pSwapchains = &context->swapchain;
  present_info.swapchainCount = 1;
  present_info.pWaitSemaphores =
      &context->frames[current_frame].render_semaphore;
  present_info.waitSemaphoreCount = 1;
  present_info.pImageIndices = &swapchain_image_index;

  CADE_DEBUG("Presenting queue...");
  vk_result = vkQueuePresentKHR(context->queue, &present_info);
  if (vk_result == VK_ERROR_OUT_OF_DATE_KHR) {
    context->resize_requested = TRUE;
    return;
  }
  CADE_DEBUG("vkQueuePresentKHR returned: %d", vk_result);
  CADE_DEBUG("Presented queue.");

  context->frame_number++;
}
