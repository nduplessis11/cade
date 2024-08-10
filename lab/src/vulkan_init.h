#pragma once

#include "vulkan_utils.h"

result_t initialize_vulkan(vulkan_context_t *context);

void cleanup_vulkan(vulkan_context_t *context);
