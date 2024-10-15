#pragma once

#include "defines.h"
#include "vulkan_utils.h"
// #include <vulkan/vulkan_core.h>
//
// typedef struct vulkan_swapchain_support {
//   VkSurfaceCapabilitiesKHR capabilities;
//   VkSurfaceFormatKHR *formats;
//   VkPresentModeKHR *present_modes;
// } vulkan_swapchain_support;

result_t create_swapchain(vulkan_context_t *context);

