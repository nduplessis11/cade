#pragma once

#include "defines.h"
#include <vulkan/vulkan_core.h>
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>

typedef struct vulkan_frame_t {
  VkCommandPool command_pool;
  VkCommandBuffer main_command_buffer;
  VkSemaphore swapchain_semaphore;
  VkSemaphore render_semaphore;
  VkFence render_fence;
} vulkan_frame_t;

typedef struct vulkan_swapchain_support_t {
  VkSurfaceCapabilitiesKHR capabilities;
  VkSurfaceFormatKHR formats[32];
  u32 format_count;
  VkPresentModeKHR present_modes[32];
  u32 present_mode_count;
} vulkan_swapchain_support_t;

typedef struct vulkan_context_t {
  VkInstance instance;
  VkSurfaceKHR surface;
  VkPhysicalDevice physical_device;
  VkDevice device;
  u32 queue_family_index;
  VkSwapchainKHR swapchain;
  VkImage swapchain_images[32];
  u32 swapchain_image_count;
  vulkan_swapchain_support_t swapchain_support;
  vulkan_frame_t frames[FRAME_OVERLAP];
  u32 frame_number;
  VkDebugUtilsMessengerEXT debug_messenger;
  xcb_connection_t *connection;
} vulkan_context_t;

