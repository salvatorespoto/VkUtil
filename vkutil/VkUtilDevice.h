#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan.h>
#include <vector>
#include <string>

namespace VkUtil
{
	typedef struct 
	{
		uint32_t index;
		std::vector<float> priorities;
	} VkUtilCreateQueueInfo;


	/** 
	 * This class represent a device. 
	 * Could be used to retrieve the physical device capabilities and to instantiate a 
	 * logical device with required capabilities.
	 */
	class VkUtilDevice
	{
	public:

		/**
		 * Create a new device VkUtilDevice, load all the info about the properties,
		 * features, extensions and queues for the physical device.
		 */
		VkUtilDevice(VkPhysicalDevice m_device);

		/**
		 * Get the device properties
		 */
		VkPhysicalDeviceProperties getProperties();

		/**
		 * Get the device properties
		 */
		VkPhysicalDeviceFeatures getFeatures();

		/**
		 * Set the required feature for the logical device
		 */
		bool setRequiredFeatures(VkPhysicalDeviceFeatures requiredFeatures);

		/**
		 * Get the physical supported device extensions
		 */
		std::vector<VkExtensionProperties> getExtensions();

		/**
		 * Check if an extension is supported
		 */
		bool isExtensionSupported(std::string extension);

		/**
		 * Set the requested extension for the logical device
		 */
		bool setRequiredExtensions(std::vector<const char*> requiredExtensions);

		/**
		 * Get all the queue families on the physical device
		 */
		std::vector<VkQueueFamilyProperties> getQueueFamily();

		/**
		 * Look for a device with specific capabilities
		 */
		std::vector<uint32_t> getQueueByCapability(VkQueueFlags requestedFlags);

		/**
		 * Set the required queques
		 */
		bool setRequiredQueues(std::vector<VkUtilCreateQueueInfo> requiredQueues);
		
		/**
		 * Create a logical device with the required capabilities specified in
		 * with setRequiredFeatures(), setRequiredExtensions() and setRequiredQueues()
		 */
		void createLogical();

	private:

		void loadInfo();

		VkPhysicalDevice m_device;											/*< The physical device */
		VkDevice m_logical_device;											/*< The logical device that can be created from this physical device*/
		VkPhysicalDeviceProperties m_properties;							/*< The properties of the physical device */
		VkPhysicalDeviceFeatures m_features;								/*< The features of the physical device */
		std::vector<VkExtensionProperties> m_extensions;					/*< The extensions supported from the physical device */
		std::vector <VkQueueFamilyProperties> m_queueFamilyProperties;		/*< The list of queue families on the physical device */
		VkPhysicalDeviceFeatures m_requiredFeatures;						/*< The required features for the logical device */
		std::vector<const char*> m_requiredExtensions;						/*< The required extensions for the logical device */
		std::vector<VkUtilCreateQueueInfo> m_requiredQueues;				/*< The required queues for the logicla device */
	};
}