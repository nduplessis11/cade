#include <stdio.h>
#include <vulkan/vulkan_core.h>
#include "vulkan_device.h"
#include "vulkan_utils.h"

result_t get_vk_physical_device(vulkan_context_t *context) {
  VkResult vk_result = {0};
  result_t result = {.success = TRUE, NULL};

  u32 physical_device_count = 0;
  VkPhysicalDevice vk_physical_device = {0};
  VkPhysicalDeviceProperties vk_physical_device_properties = {0};

  vk_result = vkEnumeratePhysicalDevices(context->instance, &physical_device_count, NULL);
  result = check_vk_result(vk_result);

  fprintf(stdout, "Physical Device Count: %d\n", physical_device_count);

  vk_result = vkEnumeratePhysicalDevices(context->instance, &physical_device_count, &vk_physical_device);
  result = check_vk_result(vk_result);

  fprintf(stdout, "Physical Device Created\n");

  vkGetPhysicalDeviceProperties(vk_physical_device, &vk_physical_device_properties);
  fprintf(stdout, "Device Name: %s\nAPI Version: %u\nDriver Version:%u\nVendor ID: %u\nDevice ID: %u\n",
          vk_physical_device_properties.deviceName,
          vk_physical_device_properties.apiVersion,
          vk_physical_device_properties.driverVersion,
          vk_physical_device_properties.vendorID,
          vk_physical_device_properties.deviceID);

  return result;
} 
