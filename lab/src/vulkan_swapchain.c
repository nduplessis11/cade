#include "vulkan_swapchain.h"
#include "logger.h"
#include "cade_assert.h"
#include <stdio.h>
#include <vulkan/vulkan_core.h>

VkSurfaceFormatKHR choose_format(VkSurfaceFormatKHR *formats, u32 format_count);

result_t create_swapchain(vulkan_context_t *context) {
  result_t result = {.success = TRUE, .message = NULL};

  CADE_INFO("Creating swapchain...");

  VkSurfaceFormatKHR format;
  b8 found_format = FALSE;
  for (int i = 0; i < context->swapchain_support.format_count; i++) {
    if (context->swapchain_support.formats[i].format ==
            VK_FORMAT_B8G8R8A8_SRGB &&
        context->swapchain_support.formats[i].colorSpace ==
            VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      format = context->swapchain_support.formats[i];
      found_format = TRUE; 
      break;
    }
  }
  CADE_ASSERT_DEBUG(found_format);
  CADE_DEBUG("Format selected.");

  VkPresentModeKHR present_mode;
  b8 found_present_mode = FALSE;
  for (int i = 0; i < context->swapchain_support.present_mode_count; i++) {
    if (context->swapchain_support.present_modes[i] == VK_PRESENT_MODE_FIFO_KHR) {
      present_mode = context->swapchain_support.present_modes[i];
      found_present_mode = TRUE;
      break;
    }
  }
  CADE_ASSERT_DEBUG(found_present_mode);
  CADE_DEBUG("Present mode selected.");

  VkSwapchainKHR swapchain = {0};

  VkSwapchainCreateInfoKHR create_info = {0};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = context->surface;

  return result;
}
