#include "VkUtilDevice.h"
#include "vk_functions.h"
#define VK_NO_PROTOTYPES
#include <vulkan.h>
#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace VkUtil
{
	VkUtilDevice::VkUtilDevice(VkPhysicalDevice device) : m_device(device)
	{
		loadInfo();
	}

	void VkUtilDevice::loadInfo()
	{
		// Get device extensions
		uint32_t count = 0;
		if (VK_SUCCESS != vkEnumerateDeviceExtensionProperties(m_device, nullptr, &count, nullptr))
		{
			std::cout << "Could not load the extensions properties info." << std::endl;
		}
		m_extensions.resize(count);
		if (count == 0 || VK_SUCCESS != vkEnumerateDeviceExtensionProperties(m_device, nullptr, &count, &m_extensions[0]))
		{
			std::cout << "Could not load the extensions properties info." << std::endl;
		}

		// Get device features
		vkGetPhysicalDeviceFeatures(m_device, &m_features);

		// Get	device properties
		vkGetPhysicalDeviceProperties(m_device, &m_properties);

		// Get queque infos
		count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_device, &count, nullptr);
		if (count == 0)
		{
			throw std::runtime_error("Could not load the queue family properties");
		}
		m_queueFamilyProperties.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(m_device, &count, &m_queueFamilyProperties[0]);
	}

	VkPhysicalDeviceProperties VkUtilDevice::getProperties()
	{
		return m_properties;
	}
	
	VkPhysicalDeviceFeatures VkUtilDevice::getFeatures()
	{ 
		return m_features; 
	}
	
	bool VkUtilDevice::setRequiredFeatures(VkPhysicalDeviceFeatures requiredFeatures)
	{
		// TODO: check that required features are supported	
		m_requiredFeatures = requiredFeatures;
		return true;
	}

	std::vector<VkExtensionProperties> VkUtilDevice::getExtensions()
	{
		return m_extensions;
	}

	bool VkUtilDevice::isExtensionSupported(std::string extension)
	{
		for (VkExtensionProperties e : m_extensions)
		{
			if (extension == std::string(e.extensionName)) return true;
		}
		return false;
	}

	bool VkUtilDevice::setRequiredExtensions(std::vector<const char*> requiredExtensions)
	{
		// Check if extensions are supported
		for (auto e : requiredExtensions)
		{
			if (!isExtensionSupported(e)) return false;
		}
		m_requiredExtensions = requiredExtensions;
		return true;
	}

	std::vector<VkQueueFamilyProperties> VkUtilDevice::getQueueFamily()
	{ 
		return m_queueFamilyProperties; 
	}

	std::vector<uint32_t> VkUtilDevice::getQueueByCapability(VkQueueFlags requiredFlags)
	{
		std::vector<uint32_t> idxs;
		for (auto q : m_queueFamilyProperties)
		{
			if ((q.queueFlags & requiredFlags) == requiredFlags)
			{
				idxs.push_back(idxs.size());
			}
		}

		return idxs;
	}

	bool VkUtilDevice::setRequiredQueues(std::vector<VkUtilCreateQueueInfo> requiredQueues)
	{
		for (auto& q : requiredQueues)
		{
			if(q.index > m_queueFamilyProperties.size() - 1) return false;
			if(q.priorities.size() > m_queueFamilyProperties[q.index].queueCount) return false;
		}
		m_requiredQueues = requiredQueues;
		return true;
	}

	void VkUtilDevice::createLogical()
	{
		std::vector<VkDeviceQueueCreateInfo> queuesInfo;
		for (auto& q : m_requiredQueues)
		{
			queuesInfo.push_back(
				{
					VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					nullptr,
					0,
					q.index,
					static_cast<uint32_t>(q.priorities.size()),
					q.priorities.size() > 0 ? &q.priorities[0] : nullptr
				});
		}

		// Create the device
		VkDeviceCreateInfo device_create_info = 
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			nullptr,
			0,
			static_cast<uint32_t>(queuesInfo.size()),
			queuesInfo.size() > 0 ? &queuesInfo[0] : nullptr,
			0,
			nullptr,
			static_cast<uint32_t>(m_requiredExtensions.size()),
			m_requiredExtensions.size() > 0 ? &m_requiredExtensions[0] : nullptr,
			&m_requiredFeatures
		};

		VkResult result = vkCreateDevice(m_device, &device_create_info, nullptr, &m_logical_device);
		if ((result != VK_SUCCESS) || (m_logical_device == VK_NULL_HANDLE)) 
		{
			throw std::runtime_error("Could not create logical device, error " + result);
		}
	}
}