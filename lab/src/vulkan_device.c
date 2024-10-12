#include "vulkan_device.h"
#include "vulkan_utils.h"
#include <stdio.h>
#include <vulkan/vulkan_core.h>

result_t get_vk_physical_device(vulkan_context_t *context) {
  VkResult vk_result = {0};
  result_t result = {.success = TRUE, NULL};

  u32 physical_device_count = 0;
  VkPhysicalDevice physical_device = {0};
  VkPhysicalDeviceProperties physical_device_props = {0};
  VkQueueFamilyProperties queue_family_props[32];
  u32 queue_family_prop_count = 0;
  VkDevice device = {0};

  vk_result = vkEnumeratePhysicalDevices(context->instance,
                                         &physical_device_count, NULL);
  result = check_vk_result(vk_result);

  fprintf(stdout, "Physical Device Count: %d\n", physical_device_count);

  vk_result = vkEnumeratePhysicalDevices(
      context->instance, &physical_device_count, &physical_device);
  result = check_vk_result(vk_result);

  fprintf(stdout, "Physical Device Found\n");

  vkGetPhysicalDeviceProperties(physical_device, &physical_device_props);
  fprintf(stdout,
          "Device Name: %s\nAPI Version: %u\nDriver Version:%u\nVendor ID: "
          "%u\nDevice ID: %u\n",
          physical_device_props.deviceName, physical_device_props.apiVersion,
          physical_device_props.driverVersion, physical_device_props.vendorID,
          physical_device_props.deviceID);

  vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
                                           &queue_family_prop_count, NULL);

  fprintf(stdout, "Queue Family Property Count: %u\n", queue_family_prop_count);

  vkGetPhysicalDeviceQueueFamilyProperties(
      physical_device, &queue_family_prop_count, queue_family_props);

  u8 queue_family_index;
  for (int i = 0; i < queue_family_prop_count; i++) {
    if (queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      fprintf(stdout, "Found graphics queue family at index: %u\n", i);
      queue_family_index = i;

      fprintf(stdout, "Queue Family Index %u Properties: %u\n", i,
              queue_family_props[i].queueCount);
      break;
    }
  }

  float queue_priority = 1.0f;
  VkDeviceQueueCreateInfo queue_create_info = {};
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.queueFamilyIndex =
      UINT32_MAX; // queue_family_index; (Failing on purpose to test debug messenger + validation)
  queue_create_info.queueCount = 1;
  queue_create_info.pQueuePriorities = &queue_priority;

  VkDeviceCreateInfo device_create_info = {};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.queueCreateInfoCount = 1;
  device_create_info.pQueueCreateInfos = &queue_create_info;

  vk_result =
      vkCreateDevice(physical_device, &device_create_info, NULL, &device);
  result = check_vk_result(vk_result);

  fprintf(stdout, "Device Created\n");
  context->device = device;

  return result;
}
