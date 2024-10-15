/*
* TODO: I don't like the way I handle result and close the program on error 
* */

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "logger.h"
#include "platform_linux.h"
#include "vulkan_device.h"
#include "vulkan_init.h"
#include "vulkan_swapchain.h"

int main() {
  linux_context_t linux_context = {0};
  vulkan_context_t vulkan_context = {VK_NULL_HANDLE, VK_NULL_HANDLE, NULL};
  result_t result = {.success = TRUE, .message = NULL};

  result = initialize_linux_window(&linux_context, 800, 600);
  if (!result.success) {
    CADE_ERROR("Linux error: %s\n", result.message);
    exit(1);
  }

  vulkan_context.connection = linux_context.connection;

  result = initialize_vulkan(&vulkan_context);
  if (!result.success) {
    CADE_ERROR("Vulkan error: %s", result.message);
    cleanup_vulkan(&vulkan_context);
    cleanup_linux(&linux_context);
    exit(1);
  }

  result = create_vulkan_surface(&vulkan_context, linux_context.window);
  if (!result.success) {
    CADE_ERROR("Vulkan error: %s", result.message);
    cleanup_vulkan(&vulkan_context);
    cleanup_linux(&linux_context);
    exit(1);
  }

  CADE_INFO("Vulkan Surface created");
  result = get_vk_physical_device(&vulkan_context);
  if (!result.success) {
    CADE_ERROR("Vulkan error: %s", result.message);
    cleanup_vulkan(&vulkan_context);
    cleanup_linux(&linux_context);
    exit(1);
  }

  result = create_swapchain(&vulkan_context);
  if (!result.success) {
    CADE_ERROR("Vulkan error: %s", result.message);
    cleanup_vulkan(&vulkan_context);
    cleanup_linux(&linux_context);
    exit(1);
  }

  // Game loop
  poll_events(&linux_context);

  // Cleanup
  cleanup_vulkan(&vulkan_context);
  cleanup_linux(&linux_context);
  return 0;
}
