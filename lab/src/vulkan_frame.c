#include "vulkan_frame.h"
#include "cade_assert.h"
#include "defines.h"
#include "logger.h"
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
    VkResult vk_result = vkCreateCommandPool(context->device, &pool_create_info, NULL, &context->frames[i].command_pool);
    result = check_vk_result(vk_result);
    CADE_ASSERT_DEBUG(result.success);
    CADE_DEBUG("Vulkan Command Pool #%u Created:", i);
  }

  return result;
}
