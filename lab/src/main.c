/*
 * TODO: I don't like the way I handle result and close the program on error
 * */

#include <stdio.h>
#include <stdlib.h>

#include "cade_assert.h"
#include "cade_application.h"
#include "defines.h"
#include "logger.h"
#include "platform_linux.h"
#include "vulkan_frame.h"
#include "vulkan_device.h"
#include "vulkan_init.h"
#include "vulkan_swapchain.h"

int main() {
  cade_context_t cade_context = {0};
  linux_context_t linux_context = {0};
  vulkan_context_t vulkan_context = {0};
  result_t result = {.success = TRUE, .message = NULL};

  cade_context.window.width = 800;
  cade_context.window.height = 600;
  result = initialize_linux_window(&linux_context, cade_context.window.width,
                                   cade_context.window.height);
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

  result = create_swapchain(&vulkan_context, &linux_context);
  if (!result.success) {
    CADE_ERROR("Vulkan error: %s", result.message);
    cleanup_vulkan(&vulkan_context);
    cleanup_linux(&linux_context);
    exit(1);
  }

  result = frame_init_commands(&vulkan_context);
  CADE_ASSERT_DEBUG(result.success);

  result = frame_init_sync_structures(&vulkan_context);
  CADE_ASSERT_DEBUG(result.success);

  // renderer_draw(&vulkan_context);

  // Game loop
  poll_events(&linux_context, &vulkan_context);

  // Cleanup
  cleanup_vulkan(&vulkan_context);
  cleanup_linux(&linux_context);
  return 0;
}
