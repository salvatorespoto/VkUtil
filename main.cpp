// main.cpp

#include "vkutil/VKUtils.h"
#include "vkutil/VkUtilDevice.h"
#include <stdexcept>
#include <memory>
#include <iostream>

void printInstanceInfo(const VkUtil::VKUtil& vkUtil)
{
	std::cout << "Vulkan instance supported extensions:" << std::endl;
	for (auto e : vkUtil.getInstanceExtensions())
	{
		std::cout << " " << e.extensionName << std::endl;
	}
};

void printDevicesInfo(const VkUtil::VKUtil& vkUtil)
{
	for(auto d : vkUtil.getDevices()) 
	{
		std::cout << "--- Physical device id: " << d.getProperties().deviceID << " ---" << "\n\n";
		std::cout << " name:" << d.getProperties().deviceName << std::endl;
		std::cout << " type:" << d.getProperties().deviceType << std::endl;
		std::cout << " vendor id:" << d.getProperties().vendorID << std::endl;
		std::cout << " driver version:" << d.getProperties().driverVersion << "\n\n";
		
		std::cout << " Features: " << d.getProperties().deviceID << std::endl;
		std::cout << "  geometry shader:" << d.getFeatures().tessellationShader << std::endl;
		std::cout << "  tessellation shader:" << d.getFeatures().geometryShader << "\n\n";

		std::cout << " Extensions: " << std::endl;
		std::vector<VkExtensionProperties> extensions = d.getExtensions();
		for (auto e : extensions)
		{
			std::cout << "  " << e.extensionName << std::endl;
		}

		int iq = 0;
		for (auto q : d.getQueueFamily())
		{
			std::cout << " Queque " << iq << std::endl;
			std::cout << "  count:" << q.queueCount << std::endl;
			std::cout << "  capabilities:"
				<< (((q.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) ? "VK_QUEUE_GRAPHICS_BIT, " : "")
				<< (((q.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT) ? "VK_QUEUE_COMPUTE_BIT, " : "")
				<< (((q.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT) ? "VK_QUEUE_TRANSFER_BIT, " : "")
				<< (((q.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) == VK_QUEUE_SPARSE_BINDING_BIT) ? "VK_QUEUE_SPARSE_BINDING_BIT " : "a") << std::endl;
			iq++;
		};
	}
}

int main()
{
	VkUtil::VKUtil vkUtil; // Wrapper for Vulkan 

	try
	{
		// Initialization: connect to vulkan loader library, import global functions
		// and get the list of supported instance extensions 
		vkUtil.init();	
		
		// Set up and create the Vulkan instance
		// Add an extension to create a presentation surface that permits to draw on an application window.
		// Drawing on an application window is an extension due it's os dependant, indeed on linux
		// and windows there are two differen extensions. 
		vkUtil.setRequiredInstanceExtensions({ VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME });
		VkApplicationInfo applicationInfo = {
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,
			"VKUtil demo",
			VK_MAKE_VERSION(1, 0, 0),
			"VKUtil demo",
			VK_MAKE_VERSION(1, 0, 0),
			VK_MAKE_VERSION(1, 0, 0)
		};
		vkUtil.createInstance(applicationInfo);
		printInstanceInfo(vkUtil);

		// Get the physical devices info
		std::vector<VkUtil::VkUtilDevice> devices = vkUtil.getDevices();
		printDevicesInfo(vkUtil);

		// Create the logical device with specific capabilities (extensions, features, queues)
		for (auto device : devices)
		{
			if(!device.setRequiredExtensions({ VK_KHR_SWAPCHAIN_EXTENSION_NAME })) continue;
			
			VkPhysicalDeviceFeatures features = {};
			features.geometryShader = VK_TRUE;
			if(!device.setRequiredFeatures(features)) continue;
			
			std::vector<uint32_t> validQueues = device.getQueueByCapability(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
			if (!device.setRequiredQueues({ { validQueues[0], { 1, 1, 1} } })) continue;

			device.createLogical();
			std::cout << "Device created" << std::endl;
			return EXIT_SUCCESS;
		}

		std::cout << "Cannot create a device with the requested capabilities" << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception & e)
	{
		std::cout << "Exception: [ " << e.what() << " ]" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}