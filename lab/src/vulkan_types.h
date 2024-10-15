#pragma once

#include <vulkan/vulkan_core.h>
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>

typedef struct {
  VkSurfaceCapabilitiesKHR capabilities;
  VkSurfaceFormatKHR formats[32];
  VkPresentModeKHR present_modes[32];
} vulkan_swapchain_support_t;

typedef struct {
  VkInstance instance;
  VkSurfaceKHR surface;
  VkPhysicalDevice physical_device;
  VkDevice device;
  vulkan_swapchain_support_t swapchain_support;
  VkDebugUtilsMessengerEXT debug_messenger;
  xcb_connection_t *connection;
} vulkan_context_t;

