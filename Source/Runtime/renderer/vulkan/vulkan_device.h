#pragma once
#include "defines.h"
#include "vulkan_types.h"

b8 create_logical_dev(VulkanContext* vkcontext);
static b8 find_phys_dev(VulkanContext* vkcontext);
static b8 find_graphics_queue(VulkanContext* vkcontext);
