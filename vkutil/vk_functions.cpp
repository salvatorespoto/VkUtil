#include "vk_functions.h"

namespace VkUtil
{
	#define VK_DECLARE_FUNCTION(name) PNF_(name) name;

	// List of functions to be imported
	VK_DECLARE_FUNCTION(vkGetInstanceProcAddr)

	VK_DECLARE_FUNCTION(vkEnumerateInstanceExtensionProperties)
	VK_DECLARE_FUNCTION(vkEnumerateInstanceLayerProperties)
	VK_DECLARE_FUNCTION(vkCreateInstance)

	VK_DECLARE_FUNCTION(vkEnumeratePhysicalDevices)
	VK_DECLARE_FUNCTION(vkGetPhysicalDeviceProperties)
	VK_DECLARE_FUNCTION(vkEnumerateDeviceExtensionProperties)
	VK_DECLARE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)
	VK_DECLARE_FUNCTION(vkGetPhysicalDeviceFeatures)
	VK_DECLARE_FUNCTION(vkCreateDevice)
	VK_DECLARE_FUNCTION(vkGetDeviceProcAddr)
}