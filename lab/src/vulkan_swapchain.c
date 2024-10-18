#include "vulkan_swapchain.h"
#include "logger.h"
#include "cade_assert.h"
#include "vulkan_utils.h"
#include <stdio.h>
#include <vulkan/vulkan_core.h>

result_t create_swapchain(vulkan_context_t *context) {
  result_t result = {.success = TRUE, .message = NULL};

  CADE_INFO("Creating swapchain...");

  VkSurfaceFormatKHR format;
  b8 found_format = FALSE;
  for (int i = 0; i < context->swapchain_support.format_count; i++) {
    if (context->swapchain_support.formats[i].format ==
            VK_FORMAT_B8G8R8A8_UNORM &&
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

  // Vulkan Exent Selection
  VkExtent2D extent;
  extent.width = 800;
  extent.height = 600;

  VkSwapchainKHR swapchain = {0};
  VkSwapchainCreateInfoKHR create_info = {0};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = context->surface;
  create_info.minImageCount = context->swapchain_support.capabilities.minImageCount;
  create_info.imageFormat = format.format;
  create_info.imageColorSpace = format.colorSpace;
  create_info.imageExtent = extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = VK_NULL_HANDLE;
  create_info.preTransform = context->swapchain_support.capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  VkResult vk_result = vkCreateSwapchainKHR(context->device, &create_info, NULL, &swapchain);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_INFO("Swapchain created.");
  context->swapchain = swapchain;

  // Retrieve swapchain images
  vk_result = vkGetSwapchainImagesKHR(context->device, swapchain, &context->swapchain_image_count, NULL);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Swapchain image count: %u", context->swapchain_image_count);

  vk_result = vkGetSwapchainImagesKHR(context->device, swapchain, &context->swapchain_image_count, context->swapchain_images);
  result = check_vk_result(vk_result);
  CADE_ASSERT_DEBUG(result.success);
  CADE_DEBUG("Swapchain images retrieved successfully");

  return result;
}
