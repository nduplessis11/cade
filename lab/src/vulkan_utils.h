#pragma once

#include <stdio.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

#include "defines.h"
#include "logger.h"
#include "vulkan_types.h"

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
    CADE_ERROR("VULKAN ERROR: %s", vk_message);
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

static inline const char *
to_string_message_severity(VkDebugUtilsMessageSeverityFlagBitsEXT s) {
  switch (s) {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    return "VERBOSE";
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    return "ERROR";
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    return "WARNING";
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    return "INFO";
  default:
    return "UNKNOWN";
  }
}

// TODO: Sync with CADE log levels
static inline const char *
to_string_message_type(VkDebugUtilsMessageTypeFlagsEXT s) {
  if (s == 7)
    return "General | Validation | Performance";
  if (s == 6)
    return "Validation | Performance";
  if (s == 5)
    return "General | Performance";
  if (s == 4 /*VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT*/)
    return "Performance";
  if (s == 3)
    return "General | Validation";
  if (s == 2 /*VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT*/)
    return "Validation";
  if (s == 1 /*VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT*/)
    return "General";
  return "Unknown";
}

// Default debug messenger
static inline VKAPI_ATTR VkBool32 VKAPI_CALL default_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData) {
  const char *ms = to_string_message_severity(messageSeverity);
  const char *mt = to_string_message_type(messageType);
  CADE_WARN("[%s: %s]\n%s", ms, mt, pCallbackData->pMessage);

  return VK_FALSE; // Applications must return false here
}

// Function to load vkCreateDebugUtilsMessengerEXT
static PFN_vkCreateDebugUtilsMessengerEXT
load_vkCreateDebugUtilsMessengerEXT(VkInstance instance) {
  PFN_vkCreateDebugUtilsMessengerEXT func =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkCreateDebugUtilsMessengerEXT");
  if (func == NULL) {
    CADE_ERROR("Function not found: 'vkCreateDebugUtilsMessengerEXT'");
  }
  return func;
}

static inline result_t
create_debug_messenger(vulkan_context_t *context,
                       VkDebugUtilsMessengerCreateInfoEXT *debug_create_info) {
  result_t result = {.success = TRUE, .message = NULL};

  VkDebugUtilsMessengerEXT debug_messenger;
  // Load the function pointer
  PFN_vkCreateDebugUtilsMessengerEXT func =
      load_vkCreateDebugUtilsMessengerEXT(context->instance);
  if (func != NULL) {
    VkResult vk_result =
        func(context->instance, debug_create_info, NULL, &debug_messenger);
    result = check_vk_result(vk_result);
    if (result.success == TRUE) {
      CADE_INFO("Debug Messenger Created");
      context->debug_messenger = debug_messenger;
    } else {
      CADE_ERROR("Failed to create debug messenger: %s", result.message);
    }
  }

  return result;
}

static PFN_vkDestroyDebugUtilsMessengerEXT
load_vkDestroyDebugUtilsMessengerEXT(VkInstance instance) {
  PFN_vkDestroyDebugUtilsMessengerEXT func =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func == NULL) {
    CADE_ERROR("Function not found: 'vkDestroyDebugUtilsMessengerEXT'");
  }
  return func;
}

static inline void destroy_debug_messenger(vulkan_context_t *context) {
  PFN_vkDestroyDebugUtilsMessengerEXT func =
      load_vkDestroyDebugUtilsMessengerEXT(context->instance);
  if (func != NULL) {
    func(context->instance, context->debug_messenger, NULL);
  }
}
