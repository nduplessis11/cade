#include "vulkan_init.h"
#include "logger.h"
#include "vulkan_frame.h"
#include "vulkan_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

result_t initialize_vulkan(vulkan_context_t *context) {
  VkResult vk_result = {0};
  result_t result = {.success = TRUE, NULL};

  // Query available extensions
  u32 extension_count = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
  VkExtensionProperties extensions[32];
  vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);

  // Check if the required extensions are available
  const char *required_extensions[] = {VK_KHR_SURFACE_EXTENSION_NAME,
                                       VK_KHR_XCB_SURFACE_EXTENSION_NAME,
                                       VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
  for (int i = 0; i < 3; i++) {
    if (!is_extension_available(required_extensions[i], extension_count,
                                extensions)) {
      CADE_ERROR("Required extension %s not available", required_extensions[i]);
      xcb_disconnect(context->connection);
      exit(1);
    }
  }

  // TODO: Extract this out to vulkan_util
  // Enable Validation Layers: "VK_LAYER_KHRONOS_validation" is built-in default
  u32 layer_count;
  VkLayerProperties available_layers[32];
  const char *required_layer = "VK_LAYER_KHRONOS_validation";
  vkEnumerateInstanceLayerProperties(&layer_count, NULL);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers);
  b8 layer_found = FALSE;
  for (int i = 0; i < layer_count; i++) {
    if (strcmp(available_layers[i].layerName, required_layer) == 0) {
      layer_found = TRUE;
      CADE_INFO("Layer found: %s", required_layer);
      break;
    }
  }
  if (!layer_found) {
    CADE_WARN("Validation layer not found: %s", required_layer);
  }

  VkApplicationInfo appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                               .pApplicationName = "Game Application",
                               .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                               .pEngineName = "No engine",
                               .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                               .apiVersion = VK_API_VERSION_1_3};

  // Setup vulkan debugging
  VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
  debug_create_info.sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debug_create_info.pfnUserCallback = default_debug_callback;
  debug_create_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debug_create_info.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debug_create_info.pUserData = NULL; // Optional

  VkInstanceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo,
      .enabledExtensionCount = 3,
      .ppEnabledExtensionNames = required_extensions,
      .ppEnabledLayerNames = &required_layer,
      .enabledLayerCount = 1,
      .pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debug_create_info};

  vk_result = vkCreateInstance(&createInfo, NULL, &context->instance);
  result = check_vk_result(vk_result);
  if (result.success) {
    CADE_INFO("Vulkan initialized");
  } else {
    CADE_ERROR("Failed initializing Vulkan: %s", result.message);
  }

  result = create_debug_messenger(context, &debug_create_info);
  if (!result.success) {
    CADE_ERROR("Failed to created debug messenger: %s", result.message);
  }

  return result;
}

void cleanup_vulkan(vulkan_context_t *context) {
  vkDeviceWaitIdle(context->device);

  frame_cleanup_commands(context);
  if (context->swapchain != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(context->device, context->swapchain, NULL);
  }
  if (context->surface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(context->instance, context->surface, NULL);
  }
  if (context->device != VK_NULL_HANDLE) {
    vkDestroyDevice(context->device, NULL);
  }
  if (context->debug_messenger != VK_NULL_HANDLE) {
    destroy_debug_messenger(context);
  }
  if (context->instance != VK_NULL_HANDLE) {
    vkDestroyInstance(context->instance, NULL);
  }
}
