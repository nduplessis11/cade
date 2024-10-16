#include "vulkan_frame.h"
#include "cade_assert.h"
#include "defines.h"
#include "logger.h"
#include "vulkan_types.h"
#include "vulkan_utils.h"
#include <vulkan/vulkan_core.h>

result_t frame_init_commands(vulkan_context_t *context) {
  result_t result = {.success = FALSE, .message = NULL};

  VkCommandPoolCreateInfo pool_create_info = {};
  pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  pool_create_info.queueFamilyIndex = context->queue_family_index;

  for (int i = 0; i < FRAME_OVERLAP; i++) {
    CADE_DEBUG("Frame #:%u", i);
    VkResult vk_result =
        vkCreateCommandPool(context->device, &pool_create_info, NULL,
                            &context->frames[i].command_pool);
    result = check_vk_result(vk_result);
    CADE_ASSERT_DEBUG(result.success);
    CADE_DEBUG("Vulkan Command Pool #%u Created:", i);

    // Allocate default command buffer
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = context->frames[i].command_pool;
    alloc_info.commandBufferCount = 1;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    vk_result = vkAllocateCommandBuffers(
        context->device, &alloc_info, &context->frames[i].main_command_buffer);
    result = check_vk_result(vk_result);
    CADE_ASSERT_DEBUG(result.success);
    CADE_DEBUG("Main Command Buffer allocated");
  }

  return result;
}

void frame_cleanup_commands(vulkan_context_t *context) {
  for (int i = 0; i < FRAME_OVERLAP; i++) {
    vkDestroyCommandPool(context->device, context->frames[i].command_pool,
                         NULL);
  }
}

result_t frame_init_sync_structures(vulkan_context_t *context) {
  result_t result = {.success = FALSE, .message = NULL};

  VkFenceCreateInfo fence_create_info = {};
  fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  VkSemaphoreCreateInfo semaphore_create_info = {};
  semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  for (int i = 0; i < FRAME_OVERLAP; i++) {
    VkResult vk_result = vkCreateFence(context->device, &fence_create_info, NULL, &context->frames[i].render_fence);
    result = check_vk_result(vk_result);
    CADE_ASSERT_DEBUG(result.success);
    CADE_DEBUG("Fence created for frame: %u", i);

    vk_result = vkCreateSemaphore(context->device, &semaphore_create_info, NULL, &context->frames[i].swapchain_semaphore);
    result = check_vk_result(vk_result);
    CADE_ASSERT_DEBUG(result.success);
    CADE_DEBUG("Swapchain semaphore created for frame: %u", i);

    vk_result = vkCreateSemaphore(context->device, &semaphore_create_info, NULL, &context->frames[i].render_semaphore);
    result = check_vk_result(vk_result);
    CADE_ASSERT_DEBUG(result.success);
    CADE_DEBUG("Render semaphore created for frame: %u", i);
  }

  return result;
}
