#include <stdio.h>
#include <vulkan/vulkan_core.h>
#include "vulkan_device.h"

result_t get_vk_physical_device(vulkan_context_t *context) {
  VkResult vk_result = {0};
  result_t result = {.success = TRUE, NULL};

  u32 physical_device_count = 0;
  vk_result = vkEnumeratePhysicalDevices(context->instance, &physical_device_count, NULL);
  result = check_vk_result(vk_result);

  fprintf(stdout, "Physical Device Count: %d\n", physical_device_count);

  return result;
} 
