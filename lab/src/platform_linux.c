#include "platform_linux.h"

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <xcb/xcb.h>

result_t initialize_linux_window(linux_context_t *context, u32 width,
                                 u32 height) {
  result_t result = {.success = TRUE, .message = NULL};

  // Open the connection to the X server
  xcb_connection_t *connection = xcb_connect(NULL, NULL);
  if (xcb_connection_has_error(connection)) {
    result.success = FALSE;
    result.message = "Unable to open X display";
    return result;
  }
  context->connection = connection;

  // Get the first screen
  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t *screen = iter.data;

  // Create a window
  xcb_window_t window = xcb_generate_id(connection);
  u32 mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  u32 values[2] = {screen->black_pixel,
                   XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 10,
                    10, 800, 600, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, mask, values);

  context->window = window;

  // Display window to the screen
  xcb_map_window(connection, window);

  // Flush the requests to the X server
  xcb_flush(connection);

  return result;
}

void cleanup_linux(linux_context_t *context) {
  if (context->connection) {
    xcb_disconnect(context->connection);
  }
}

b8 poll_events(linux_context_t *context) {
  b8 is_running = TRUE;
  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(context->connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_EXPOSE:
      printf("Window exposed\n");
      break;
    case XCB_KEY_PRESS:
      is_running = FALSE;
      break;
    }
    free(event);
    if (is_running == FALSE) {
      return FALSE;
    }
  }

  return FALSE;
}

result_t create_vulkan_surface(vulkan_context_t *context, xcb_window_t window) {
  result_t result = {.success = TRUE, .message = NULL};
  VkResult vk_result = {0};

  VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
      .connection = context->connection,
      .window = window};

  vk_result = vkCreateXcbSurfaceKHR(context->instance, &surfaceCreateInfo, NULL,
                                    &context->surface);
  result = check_vk_result(vk_result);
  return result;
}
