#include "vulkan_device.h"
#include "cade_assert.h"
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
  CADE_INFO("Device Name: %s", physical_device_props.deviceName);
  CADE_INFO("Vulkan API Version: %u.%u.%u",
            VK_VERSION_MAJOR(physical_device_props.apiVersion),
            VK_VERSION_MINOR(physical_device_props.apiVersion),
            VK_VERSION_PATCH(physical_device_props.apiVersion));

  // Device surface swapchain support
  vk_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      context->physical_device, context->surface,
      &context->swapchain_support.capabilities);
  result = check_vk_result(vk_result);
  if (!result.success) {
    CADE_WARN("Error retrieving swapchain surface support info for physical "
              "device: %s",
              result.message);
  }
  vk_result = vkGetPhysicalDeviceSurfaceFormatsKHR(
      context->physical_device, context->surface,
      &context->swapchain_support.format_count, NULL);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Physical Device Surface Format count: %u",
             context->swapchain_support.format_count);
  vk_result = vkGetPhysicalDeviceSurfaceFormatsKHR(
      context->physical_device, context->surface,
      &context->swapchain_support.format_count,
      context->swapchain_support.formats);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Physical Device Surface Formats retrieved.");
  vk_result = vkGetPhysicalDeviceSurfacePresentModesKHR(
      context->physical_device, context->surface,
      &context->swapchain_support.present_mode_count, NULL);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Physical Device Present Mode count: %u",
             context->swapchain_support.present_mode_count);
  vk_result = vkGetPhysicalDeviceSurfacePresentModesKHR(
      context->physical_device, context->surface,
      &context->swapchain_support.present_mode_count,
      context->swapchain_support.present_modes);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Physical Device Present Modes retrieved.");

  vkGetPhysicalDeviceQueueFamilyProperties(context->physical_device,
                                           &queue_family_prop_count, NULL);
  vkGetPhysicalDeviceQueueFamilyProperties(
      physical_device[0], &queue_family_prop_count, queue_family_props);
  CADE_DEBUG("Queue Family Property Count: %u", queue_family_prop_count);

  for (int i = 0; i < queue_family_prop_count; i++) {
    if (queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      CADE_DEBUG("Found graphics queue family at index: %u", i);
      context->queue_family_index = i;

      CADE_DEBUG("Queue Family Index %u Properties: %u", i,
                 queue_family_props[i].queueCount);
      break;
    }
  }

  float queue_priority = 1.0f;
  VkDeviceQueueCreateInfo queue_create_info = {};
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.queueFamilyIndex = context->queue_family_index;
  queue_create_info.queueCount = 1;
  queue_create_info.pQueuePriorities = &queue_priority;

  VkPhysicalDeviceFeatures features = {};
  VkPhysicalDeviceVulkan13Features features13 = {};
  features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
  // set the desired features here:
  features13.dynamicRendering = VK_TRUE;
  features13.synchronization2 = VK_TRUE;

  VkPhysicalDeviceFeatures2 features2 = {};
  features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
  features2.pNext = &features13;
  features2.features = features;

  VkDeviceCreateInfo device_create_info = {};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.pNext = &features2;
  device_create_info.queueCreateInfoCount = 1;
  device_create_info.pQueueCreateInfos = &queue_create_info;
  device_create_info.enabledExtensionCount = 1;
  device_create_info.ppEnabledExtensionNames = required_extensions;

  vk_result =
      vkCreateDevice(physical_device[0], &device_create_info, NULL, &device);
  result = check_vk_result(vk_result);

  CADE_INFO("Device Created");
  context->device = device;

  vkGetDeviceQueue(context->device, context->queue_family_index, 0, &context->queue);

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
