#include "vulkan_backend.h"
#include <vulkan/vulkan.h>

#include "core/logging/logger.h"
#include "renderer/vulkan/vulkan_device.h"
#include "renderer/vulkan/vulkan_instance.h"

static VulkanContext g_vkcontext;

static b8 init_vma(void) {
	VmaAllocatorCreateInfo vma_alloc_info = {
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = g_vkcontext.device.physical_dev,
		.device = g_vkcontext.device.logical_dev,
		.instance = g_vkcontext.instance,
		.vulkanApiVersion = VK_API_VERSION_1_4,
	};
	if (vmaCreateAllocator(&vma_alloc_info, &g_vkcontext.vma_allocator) != VK_SUCCESS) {
		return FALSE;
	}
	return TRUE;
}

b8 vulkan_backend_init(Platform* platform) {
	// TODO: custom allocator
	g_vkcontext.allocator = 0;

	if (!create_vulkan_instance(&g_vkcontext)) {
		LOG_ERROR("Failed to create Vulkan instance");
		return FALSE;
	}
	LOG_INFO("Created Vulkan instance");

	if (!platform_create_vk_surface(platform, &g_vkcontext.instance, &g_vkcontext.surface)) {
		LOG_ERROR("Failed to create Vulkan surface");
		return FALSE;
	}
	LOG_INFO("Created Vulkan surface");

	if (!create_logical_dev(&g_vkcontext)) {
		LOG_ERROR("Failed to create logical device");
		return FALSE;
	}
	LOG_INFO("Created Vulkan logical device");

	if (!init_vma()) {
		LOG_ERROR("Failed to create Vulkan Memory Allocator");
		return FALSE;
	}
	LOG_INFO("Crated Vulkan Memory Allocator");

	return TRUE;
}

void vulkan_backend_shutdown(void) {
	vkDeviceWaitIdle(g_vkcontext.device.logical_dev);
	if (g_vkcontext.vma_allocator) {
		vmaDestroyAllocator(g_vkcontext.vma_allocator);
	};

	if (g_vkcontext.surface) {
		vkDestroySurfaceKHR(g_vkcontext.instance, g_vkcontext.surface, g_vkcontext.allocator);
	};

	if (g_vkcontext.device.logical_dev) {
		vkDestroyDevice(g_vkcontext.device.logical_dev, g_vkcontext.allocator);
	}

	if (g_vkcontext.instance) {
		vkDestroyInstance(g_vkcontext.instance, g_vkcontext.allocator);
	}
}

b8 vulkan_backend_begin_frame(RenderPacket* packet) {
	return TRUE;
}
b8 vulkan_backend_end_frame(void) {
	return TRUE;
}
