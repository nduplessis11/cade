#include "vulkan_init.h"

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

result_t initialize_vulkan(vulkan_context_t *context) {
  VkResult vk_result = {0};
  result_t result = {.success = TRUE, NULL};

  // Query available extensions
  u32 extension_count = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
  VkExtensionProperties *extensions =
      malloc(extension_count * sizeof(VkExtensionProperties));
  vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);

  // Check if the required extensions are available
  const char *required_extensions[] = {VK_KHR_SURFACE_EXTENSION_NAME,
                                       VK_KHR_XCB_SURFACE_EXTENSION_NAME};
  for (int i = 0; i < 2; i++) {
    if (!is_extension_available(required_extensions[i], extension_count,
                                extensions)) {
      fprintf(stderr, "Required extension %s not available\n",
              required_extensions[i]);
      free(extensions);
      xcb_disconnect(context->connection);
      exit(1);
    }
  }
  free(extensions);

  VkApplicationInfo appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                               .pApplicationName = "Vulkan XCB Example",
                               .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                               .pEngineName = "No engine",
                               .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                               .apiVersion = VK_API_VERSION_1_0};

  VkInstanceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo,
      .enabledExtensionCount = 2,
      .ppEnabledExtensionNames = required_extensions};

  vk_result = vkCreateInstance(&createInfo, NULL, &context->instance);
  result = check_vk_result(vk_result);

  return result;
}

void cleanup_vulkan(vulkan_context_t *context) {
  if (context->surface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(context->instance, context->surface, NULL);
  }
  if (context->instance != VK_NULL_HANDLE) {
    vkDestroyInstance(context->instance, NULL);
  }
}
