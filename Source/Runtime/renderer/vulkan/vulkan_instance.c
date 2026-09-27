#include "vulkan_instance.h"
#include "core/platform/platform.h"

b8 create_vulkan_instance(VulkanContext* vkcontext) {
	u32 n_exts;
	const char** exts = platform_get_vk_instance_ext(&n_exts);
	static const char* layers[] = {"VK_LAYER_KHRONOS_validation"};
	vkcontext->n_ext = n_exts;
	vkcontext->n_layers = 1;
	vkcontext->exts = exts;
	vkcontext->layers = layers;

	// Vulkan application info
	VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Triangle",
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.pEngineName = "CustomEngine",
		.engineVersion = VK_MAKE_VERSION(0, 1, 0),
		.apiVersion = VK_API_VERSION_1_4,
	};

	// Vulkan instance info
	VkInstanceCreateInfo inst_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info,
		.enabledLayerCount = vkcontext->n_layers,
		.enabledExtensionCount = vkcontext->n_ext,
		.ppEnabledLayerNames = vkcontext->layers,
		.ppEnabledExtensionNames = vkcontext->exts,
	};

	if (vkCreateInstance(&inst_info, vkcontext->allocator, &vkcontext->instance) != VK_SUCCESS) {
		return FALSE;
	}
	return TRUE;
}
