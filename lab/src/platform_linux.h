#pragma once

#include <xcb/xcb.h>

#include "defines.h"
#include "vulkan_utils.h"

typedef struct {
  xcb_connection_t *connection;
  xcb_window_t window;
} linux_context_t;

result_t initialize_linux_window(linux_context_t *context, u32 width, u32 height);

void cleanup_linux(linux_context_t *context);

b8 poll_events(linux_context_t *context, vulkan_context_t *vulkan_context);

result_t create_vulkan_surface(vulkan_context_t *context, xcb_window_t window); 

void get_framebuffer_size(linux_context_t *linux_context, u16 *width, u16 *height);
