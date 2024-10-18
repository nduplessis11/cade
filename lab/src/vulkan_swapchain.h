#pragma once

#include "defines.h"
#include "platform_linux.h"
#include "vulkan_types.h"
#include <vulkan/vulkan_core.h>

result_t create_swapchain(vulkan_context_t *context, linux_context_t *linux_context);

