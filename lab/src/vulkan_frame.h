#pragma once

#include "vulkan_types.h"

result_t frame_init_commands(vulkan_context_t *context);
void frame_cleanup_commands(vulkan_context_t *context);

result_t frame_init_sync_structures(vulkan_context_t *context);
result_t frame_submit_queue(vulkan_context_t *context);
