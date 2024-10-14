#include "vulkan_device.h"
#include "logger.h"
#include "vulkan_utils.h"
#include <stdio.h>
#include <vulkan/vulkan_core.h>

b8 is_device_supported(VkPhysicalDevice device,
                       const char *required_extension[]);

result_t get_vk_physical_device(vulkan_context_t *context) {
  VkResult vk_result = {0};
  result_t result = {.success = TRUE, NULL};

  u32 physical_device_count = 0;
  VkPhysicalDevice physical_device[32];
  VkPhysicalDeviceProperties physical_device_props = {0};
  VkQueueFamilyProperties queue_family_props[32];
  u32 queue_family_prop_count = 0;
  VkDevice device = {0};

  vk_result = vkEnumeratePhysicalDevices(context->instance,
                                         &physical_device_count, NULL);
  result = check_vk_result(vk_result);

  CADE_DEBUG("Physical Device Count: %u", physical_device_count);

  vk_result = vkEnumeratePhysicalDevices(
      context->instance, &physical_device_count, physical_device);
  result = check_vk_result(vk_result);

  const char *required_extensions[32];
  required_extensions[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  if (!is_device_supported(physical_device[0], required_extensions)) {
    result.success = FALSE;
    result.message = "Physical device not supported";
    return result;
  }
  CADE_INFO("Physical device selected");
  context->physical_device = physical_device[0];
  vkGetPhysicalDeviceProperties(physical_device[0], &physical_device_props);
  CADE_INFO("Device Name: %s\n\tAPI Version: %u\n\tDriver Version:%u\n\tVendor ID: "
            "%u\n\tDevice ID: %u",
            physical_device_props.deviceName, physical_device_props.apiVersion,
            physical_device_props.driverVersion, physical_device_props.vendorID,
            physical_device_props.deviceID);

  vkGetPhysicalDeviceQueueFamilyProperties(physical_device[0],
                                           &queue_family_prop_count, NULL);

  CADE_DEBUG("Queue Family Property Count: %u", queue_family_prop_count);

  vkGetPhysicalDeviceQueueFamilyProperties(
      physical_device[0], &queue_family_prop_count, queue_family_props);

  u8 queue_family_index;
  for (int i = 0; i < queue_family_prop_count; i++) {
    if (queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      CADE_DEBUG("Found graphics queue family at index: %u", i);
      queue_family_index = i;

      CADE_DEBUG("Queue Family Index %u Properties: %u", i,
              queue_family_props[i].queueCount);
      break;
    }
  }

  float queue_priority = 1.0f;
  VkDeviceQueueCreateInfo queue_create_info = {};
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.queueFamilyIndex = queue_family_index;
  queue_create_info.queueCount = 1;
  queue_create_info.pQueuePriorities = &queue_priority;

  VkDeviceCreateInfo device_create_info = {};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.queueCreateInfoCount = 1;
  device_create_info.pQueueCreateInfos = &queue_create_info;
  device_create_info.enabledExtensionCount = 1;
  device_create_info.ppEnabledExtensionNames = required_extensions;

  vk_result =
      vkCreateDevice(physical_device[0], &device_create_info, NULL, &device);
  result = check_vk_result(vk_result);

  CADE_INFO("Device Created");
  context->device = device;

  return result;
}

b8 is_device_supported(VkPhysicalDevice device,
                       const char *required_extensions[]) {
  b8 is_supported = FALSE;

  u32 prop_count;
  VkExtensionProperties ext_props[512]; // TODO: Dynamic allocate when memory
                                        // allocator implemented
  vkEnumerateDeviceExtensionProperties(device, NULL, &prop_count, NULL);
  CADE_DEBUG("Device Extension Property Count: %u", prop_count);
  vkEnumerateDeviceExtensionProperties(device, NULL, &prop_count, ext_props);

  for (int i = 0; i < prop_count; i++) {
    if (strcmp(ext_props[i].extensionName, required_extensions[0]) == 0) {
      CADE_INFO("Physical device supported");
      is_supported = TRUE;
      break;
    }
  }

  return is_supported;
}
