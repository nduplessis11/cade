#include "vulkan_command_buffer.h"
#include "defines.h"
#include <vulkan/vulkan_core.h>

result_t init_commands(vulkan_context_t *context) {
  result_t result = {.success = FALSE, .message = NULL};

  VkCommandPoolCreateInfo pool_create_info = {};
  pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  pool_create_info.queueFamilyIndex = context->queue_family_index;

  return result;
}
