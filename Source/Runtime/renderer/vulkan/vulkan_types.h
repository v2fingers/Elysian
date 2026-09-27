#pragma once
#include "core/logging/logger.h"
#include "defines.h"
#include "vulkan_utils.h"
#include <vulkan/vulkan.h>
#include "VMA/vk_mem_alloc.h"

#define MAX_FRAMES_IN_FLIGHT 2

typedef struct vulkan_device {
  VkDevice logical_dev;
  VkPhysicalDevice physical_dev;

  VkPhysicalDeviceMemoryProperties memory;
  VkPhysicalDeviceProperties properties;
  
  VkQueue gfx_queue;
  u32 gfx_queue_fam_idx;

} VulkanDevice;

typedef struct vulkan_context {
  const char** layers;
  const char** exts;
  u32 n_layers;
  u32 n_ext;

  VkAllocationCallbacks* allocator;
  VkInstance instance;
  VkSurfaceKHR surface;
  VmaAllocator vma_allocator;

  VulkanDevice device;
} VulkanContext;
