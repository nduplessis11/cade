#pragma once

#include <xcb/xcb.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_xcb.h>

#include <string.h>

#include "defines.h"

typedef struct {
  VkInstance instance;
  VkSurfaceKHR surface;
  VkDevice device;
  xcb_connection_t *connection;
} vulkan_context_t;

static inline const char *get_vk_result_message(VkResult vk_result) {
  switch (vk_result) {
  case VK_SUCCESS:
    return "Command successfully completed.";
  case VK_NOT_READY:
    return "A fence or query has not yet completed.";
  case VK_TIMEOUT:
    return "A wait operation has not completed in the specified time.";
  case VK_EVENT_SET:
    return "An event is signaled.";
  case VK_EVENT_RESET:
    return "An event is unsignaled.";
  case VK_INCOMPLETE:
    return "A return array was too small for the result.";
  case VK_ERROR_OUT_OF_HOST_MEMORY:
    return "A host memory allocation has failed.";
  case VK_ERROR_OUT_OF_DEVICE_MEMORY:
    return "A device memory allocation has failed.";
  case VK_ERROR_INITIALIZATION_FAILED:
    return "Initialization of an object could not be completed for "
           "implementation-specific reasons.";
  case VK_ERROR_DEVICE_LOST:
    return "The logical or physical device has been lost.";
  case VK_ERROR_MEMORY_MAP_FAILED:
    return "Mapping of a memory object has failed.";
  case VK_ERROR_LAYER_NOT_PRESENT:
    return "A requested layer is not present or could not be loaded.";
  case VK_ERROR_EXTENSION_NOT_PRESENT:
    return "A requested extension is not supported.";
  case VK_ERROR_FEATURE_NOT_PRESENT:
    return "A requested feature is not supported.";
  case VK_ERROR_INCOMPATIBLE_DRIVER:
    return "The requested version of Vulkan is not supported by the driver or "
           "is otherwise incompatible.";
  case VK_ERROR_TOO_MANY_OBJECTS:
    return "Too many objects of the type have already been created.";
  case VK_ERROR_FORMAT_NOT_SUPPORTED:
    return "A requested format is not supported on this device.";
  case VK_ERROR_FRAGMENTED_POOL:
    return "A pool allocation has failed due to fragmentation of the pool's "
           "memory.";
  default:
    return "Unknown Vulkan error.";
  }
}

static inline result_t check_vk_result(VkResult vk_result) {
  if (vk_result != VK_SUCCESS) {
    const char *vk_message = get_vk_result_message(vk_result);
    result_t result = {.success = FALSE, .message = vk_message};
    return result;
  }
  result_t result = {.success = TRUE, .message = NULL};
  return result;
}

static inline b8 is_extension_available(const char *extension,
                                        u8 extension_count,
                                        VkExtensionProperties *extensions) {
  for (u8 i = 0; i < extension_count; i++) {
    if (strcmp(extension, extensions[i].extensionName) == 0) {
      return TRUE;
    }
  }
  return FALSE;
}

static inline b8 is_layer_available(const char *layer, u8 layer_count,
                                    VkLayerProperties *layers) {
  for (u8 i = 0; i < layer_count; i++) {
    if (strcmp(layer, layers[i].layerName) == 0) {
      return TRUE;
    }
  }
  return FALSE;
}
