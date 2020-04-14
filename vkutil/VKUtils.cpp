#include "VKUtils.h"
#include "VkUtilDevice.h"
#include "vk_functions.h"
#define VK_NO_PROTOTYPES // Don't use the vulkan.h functions definition, we are going to load them ourself
#include <vulkan.h>
#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cstring>

namespace VkUtil
{
	VKUtil::VKUtil() : m_vkDllName(L"vulkan-1.dll"), m_vkLibrary(0), m_vkInstance(NULL)
	{}

	void VKUtil::setLoaderLibrary(LPCWSTR vkDllName)
	{
		m_vkDllName = vkDllName;
	}

	void VKUtil::init()
	{
		loadLoader();
		importGlobalFunctions();
		enumerateInstanceExtensions();
	}

	void VKUtil::createInstance(VkApplicationInfo application_info) 
	{
		VkInstanceCreateInfo instance_create_info = {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			  nullptr,
			  0,
			  &application_info,
			  0,
			  nullptr,
			  static_cast<uint32_t>(m_requestedInstanceExtensions.size()),
			  &m_requestedInstanceExtensions[0]
		};
 
		if (VK_SUCCESS != vkCreateInstance(&instance_create_info, nullptr, &m_vkInstance) || (m_vkInstance == VK_NULL_HANDLE)) 
		{
			throw std::runtime_error("Cannot create Vulkan Instance");
		}
		std::cout << "Created vulkan instance" << std::endl;

		importInstanceFunctions();
		enumeratePhysicalDevices();
	}

	void VKUtil::loadLoader()
	{
		m_vkLibrary = LoadLibrary(m_vkDllName);
		std::cout << "Vulkan loader library " << m_vkDllName << " loaded" << std::endl;
		if (m_vkLibrary == nullptr) throw std::runtime_error("Cannot load vulkan-1.ddl");
	}

	void VKUtil::importGlobalFunctions()
	{
		VK_IMPORT_FUNCTION(m_vkLibrary, vkGetInstanceProcAddr)

		VK_IMPORT_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties)
		VK_IMPORT_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties)
		VK_IMPORT_GLOBAL_FUNCTION(vkCreateInstance)
	};

	void VKUtil::importInstanceFunctions()
	{
		VK_IMPORT_INSTANCE_FUNCTION(m_vkInstance, vkEnumeratePhysicalDevices)
		VK_IMPORT_INSTANCE_FUNCTION(m_vkInstance, vkGetPhysicalDeviceProperties)
		VK_IMPORT_INSTANCE_FUNCTION(m_vkInstance, vkEnumerateDeviceExtensionProperties)
		VK_IMPORT_INSTANCE_FUNCTION(m_vkInstance, vkGetPhysicalDeviceQueueFamilyProperties)
		VK_IMPORT_INSTANCE_FUNCTION(m_vkInstance, vkGetPhysicalDeviceFeatures)
		VK_IMPORT_INSTANCE_FUNCTION(m_vkInstance, vkCreateDevice)
		VK_IMPORT_INSTANCE_FUNCTION(m_vkInstance, vkGetDeviceProcAddr)
	}

	void VKUtil::enumerateInstanceExtensions()
	{
		uint32_t count = 0;
		if (VK_SUCCESS != vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr))
		{
			std::cout << "Could not load the extensions properties info." << std::endl;
		}
		m_instanceExtensions.resize(count);
		if (count == 0 || VK_SUCCESS != vkEnumerateInstanceExtensionProperties(nullptr, &count, &m_instanceExtensions[0]))
		{
			std::cout << "Could not load the extensions properties info." << std::endl;
		}
	}

	bool VKUtil::isInstanceExtensionSupported(std::string extension)
	{
		for (VkExtensionProperties e : m_instanceExtensions)
		{
			if (extension == std::string(e.extensionName)) return true;
		}
		return false;
	}

	std::vector<VkExtensionProperties> VKUtil::getInstanceExtensions() const
	{
		return m_instanceExtensions;
	}

	void VKUtil::setRequiredInstanceExtensions(std::vector<const char *> requiredExtensions)
	{
		m_requestedInstanceExtensions = requiredExtensions;
	}

	void VKUtil::enumeratePhysicalDevices()
	{
		uint32_t count = 0;
		if (VK_SUCCESS != vkEnumeratePhysicalDevices(m_vkInstance, &count, nullptr))
		{
			std::cout << "Could not enumerate physical devices" << std::endl;
		}
		
		std::vector<VkPhysicalDevice> physicalDevices(count);
		if (count == 0 || VK_SUCCESS != vkEnumeratePhysicalDevices(m_vkInstance, &count, &physicalDevices[0]))
		{
			std::cout << "Could not enumerate physical devices" << std::endl;
		}

		for (VkPhysicalDevice d : physicalDevices) 
		{
			VkUtilDevice uDevice(d);
			m_physicalDevices.push_back(std::move(d));
		}
	};

	std::vector<VkUtilDevice> VKUtil::getDevices() const
	{
		return m_physicalDevices;
	}
}