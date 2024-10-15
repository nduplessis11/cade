#pragma once

#include "defines.h"
#include <vulkan/vulkan_core.h>
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>

typedef struct {
  VkSurfaceCapabilitiesKHR capabilities;
  VkSurfaceFormatKHR formats[32];
  u32 format_count;
  VkPresentModeKHR present_modes[32];
  u32 present_mode_count;
} vulkan_swapchain_support_t;

typedef struct {
  VkInstance instance;
  VkSurfaceKHR surface;
  VkPhysicalDevice physical_device;
  VkDevice device;
  VkSwapchainKHR swapchain;
  vulkan_swapchain_support_t swapchain_support;
  VkDebugUtilsMessengerEXT debug_messenger;
  xcb_connection_t *connection;
} vulkan_context_t;

