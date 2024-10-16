#pragma once

#include "vulkan_types.h"

result_t frame_init_commands(vulkan_context_t *context);
void frame_cleanup_commands(vulkan_context_t *context);
