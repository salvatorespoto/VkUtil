#pragma once

#include <windows.h>
#define VK_NO_PROTOTYPES
#include <vulkan.h>
#include <string>

namespace VkUtil
{
	// The type of a vulkan function is the function name prepended with PFN_
	#define PNF_(name) PFN_##name

	// Declare a vulkan function type
	#define VK_DECLARE_FUNCTION(name) extern PNF_(name) name;

	#define VK_IMPORT_FUNCTION(lib, name)\
		name = (PNF_(name)) GetProcAddress(lib, #name); \
		if (name == nullptr) throw std::runtime_error( std::string("Cannot import ") + #name); \
		else std::cout << "Function" << #name << " imported" << std::endl;

	#define VK_IMPORT_GLOBAL_FUNCTION(name) \
		name = (PNF_(name)) vkGetInstanceProcAddr(nullptr, #name); \
		if (name == nullptr) throw std::runtime_error(std::string("Cannot import ") + #name); \
		else std::cout << "Function " << #name << "imported" << std::endl;

	#define VK_IMPORT_INSTANCE_FUNCTION(instance, name)\
		name = (PNF_(name)) vkGetInstanceProcAddr(instance, #name); \
		if (name == nullptr) throw std::runtime_error(std::string("Cannot import ") + #name); \
		else std::cout << "Function " << #name << "imported" << std::endl;

	#define VK_IMPORT_INSTANCE_EXTENSION_FUNCTION(instance, name) \
		if(!isExtensionSupported(std::string(#name))) throw std::runtime_error(std::string("Cannot import ") + #name); \
		name = (PNF_(name)) vkGetInstanceProcAddr(instance, #name); \
		if (name == nullptr) throw std::runtime_error(std::string("Cannot import ") + #name); \
		else std::cout << "Function " << #name << "imported" << std::endl;

	#define VK_IMPORT_DEVICE_FUNCTION(device, name) \
		name = (PNF_(name)) vkGetInstanceProcAddr(device, #name); \
		if (name == nullptr) throw std::runtime_error(std::string("Cannot import ") + #name); \
		else std::cout << "Function " << #name << "imported" << std::endl;

	#define VK_IMPORT_DEVICE_EXTENSION_FUNCTION(device, name) \
		if(!isExtensionSupported(std::string(#name))) throw std::runtime_error(std::string("Cannot import ") + #name); \
		name = (PNF_(name)) vkGetInstanceProcAddr(instance, #name); \
		if (name == nullptr) throw std::runtime_error(std::string("Cannot import ") + #name); \
		else std::cout << "Function " << #name << "imported" << std::endl;


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