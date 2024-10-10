#include "vulkan_swapchain.h"
#include <stdio.h>
#include <vulkan/vulkan_core.h>

result_t create_swapchain(vulkan_context_t *context) {
  result_t result = {.success = TRUE, .message = NULL};
  fprintf(stdout, "Creating swapchain...\n");

  VkSwapchainKHR swapchain = {0};

  VkSwapchainCreateInfoKHR create_info = {0};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = context->surface;

  return result;
}
