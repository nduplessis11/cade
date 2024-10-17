#pragma once

#include <vulkan/vulkan_core.h>

void image_transition(VkCommandBuffer cmd, VkImage image,
                      VkImageLayout current_layout, VkImageLayout new_layout);
