#pragma once

#define VK_NO_PROTOTYPES // Don't use the vulkan.h functions definition, we are going to load them ourself
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan.h>
#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace VkUtil
{
	class VkUtilDevice;

	/**
	 * A wrapper for Vulkan
	 */
	class VKUtil
	{
	public:

		/**
		 * Constructor
		 */
		VKUtil();

		/**
		 * Set the Vulkan loader library name
		 */
		void setLoaderLibrary(LPCWSTR vkDllName);

		/**
		 * Initialization: connect to the Vulkan loader library, import global functions
		 * and get the list of supported instance extensions 
		 */
		void init();
		
		/**
		 * Create a the Vulkan instance
		 */
		void createInstance(VkApplicationInfo appInfo);

		/**
		 * Check if an instance extensions is supported
		 */
		bool isInstanceExtensionSupported(std::string extension);

		/**
		 * Get the instance extensions list
		 */
		std::vector<VkExtensionProperties> getInstanceExtensions() const;

		/**
		 * Set the required instance extensions
		 */
		void setRequiredInstanceExtensions(std::vector<const char*> requiredExtensions);

		/**
		 * Get the physical device list
		 */
		std::vector<VkUtilDevice> getDevices() const;


	private:

		/**
		 * Load the Vulkan loader library
		 */
		void loadLoader();

		/**
		 * Import vulkan global functions 
		 */
		void importGlobalFunctions();

		/**
		 * Import vulkan instance functions
		 */
		void importInstanceFunctions();

		/**
		 * Get the list of all supported instance extension
		 */
		void enumerateInstanceExtensions();

		/**
		 * Get the list of the phyisical devices available
		 */
		void enumeratePhysicalDevices();

		
		LPCWSTR m_vkDllName;		/*< The name of the vulkan loader library */
		HMODULE m_vkLibrary;		/*< Handler to the vulkan loader library */
		VkInstance m_vkInstance;	/*< The instance of vulkan */
		std::vector<VkExtensionProperties> m_instanceExtensions;	/*< The list of supported instance extensions */
		std::vector<VkUtilDevice> m_physicalDevices;				/*< The list of physical devices */
		std::vector<const char*> m_requestedInstanceExtensions;		/*< Requested instance extension from the client app */
	};
}