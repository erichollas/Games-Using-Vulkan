#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>


namespace Utilities {
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};


	/*
	* Struct: QueueFamilyIndices
	*
	*
	* Description: Will used to set the correct queues for the physical device
	*
	*/
	struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete() {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	/*
	* Struct: SwapChainSupportDetails
	*
	*
	* Description: used to search for the correct device to support the swapchain we want
	*
	*/
	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	/*
	* Function: querySwapchainSupport
	*
	* Paramters: VkPhysicalDevice dvc
	*			 VkSurfaceKHR srfc
	*
	* Return Type: SwapchainSupportDetails
	*
	* Description: queries the swapchain details from the physical device
	*				parameter and returns the swapchain details struct object
	*
	*/
	inline SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice dvc, VkSurfaceKHR srfc) {
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dvc, srfc, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(dvc, srfc, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(dvc, srfc, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(dvc, srfc, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(dvc, srfc, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	/*
	* Function: findQueueFamilies
	*
	* Paramters: VkPhysicalDevice dvc
	*			 VkSurfaceKHR srfc
	*
	* Return Type: QueueFamilyIndices
	*
	* Description: queries the indices of the type of queue family
	*				found that the physical device parameter supports
	*
	*/
	inline QueueFamilyIndices findQueueFamilies(VkPhysicalDevice dvc, VkSurfaceKHR srfc) {
		QueueFamilyIndices indices;

		uint32_t familyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(dvc, &familyCount, nullptr);

		std::vector<VkQueueFamilyProperties> families(familyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(dvc, &familyCount, families.data());

		int i = 0;
		for (const auto &queueFamily : families) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(dvc, i, srfc, &presentSupport);

			if (queueFamily.queueCount > 0 && presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	/*
	* Function: findMemoryType
	*
	* Paramters: VkPhysicalDevice physDevice
	*			 uint32_t typeFilter
	*			 VkMemoryPropertyFlags properties
	*
	* Return Type: uint32_t
	*
	* Description: Searches the hardware to find a graphics card that supports the
	*				memory we want for the buffers (index and vertex buffers)
	*
	*/
	inline uint32_t findMemoryType(VkPhysicalDevice physDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) throw(Exception) {
		try {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			throw Exception("failed to find suitable memory type", "Utilities.h", "findMemoryType");
		}
		catch (Exception &excpt) {
			throw excpt;
		}
	}

	/*
	* Function: checkDeviceExtensionSupport
	*
	* Paramters: VkPhysicalDevice device
	*
	* Return Type: bool
	*
	* Description: to be called from isDeviceSuitable
	*				enumerates the extensions supported by the device parameter
	*				to search for the required extensions, returns true if
	*				the required extensions are supported, otherwise returns false
	*
	*/
	inline bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	/*
	* Function: isDeviceSuitable
	*
	* Paramters: VkPhysicalDevice dvc
	*			 VkSurfaceKHR srfc
	*
	* Return Type: bool
	*
	* Description: Determines if the physical device parameter supports the
	*				required extensions to be able to present rendered images
	*				directly to the surface through the use of a swapchain
	*				returns true if the device is suitable, flase otherwise
	*				calls checkDeviceExtensionSupport to check the device
	*
	*/
	inline bool isDeviceSuitable(VkPhysicalDevice dvc, VkSurfaceKHR srfc) {
		Utilities::QueueFamilyIndices inds = Utilities::findQueueFamilies(dvc, srfc);

		bool extensionsSupported = checkDeviceExtensionSupport(dvc);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			Utilities::SwapchainSupportDetails swapChainSupport = Utilities::querySwapchainSupport(dvc, srfc);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return inds.isComplete() && extensionsSupported && swapChainAdequate;
	}

	/*
	* Function: chooseSwapchainSurfaceFormat
	*
	* Paramters: const std::vector<VkSurfaceFormatKHR>& availableFormats
	*
	* Return Type: VkSurfaceFormatKHR
	*
	* Description: checks if the swapchain surface supports the format for the color
	*				space we want most, otherwise returns the format that fits the
	*				best case we define in the function
	*
	*/
	inline VkSurfaceFormatKHR chooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	/*
	* Function: chooseSwapchainPresentMode
	*
	* Paramters: const std::vector<VkPresentModeKHR> availablePresentModes
	*
	* Return Type: VkPresentModeKHR
	*
	* Description: picks one of the four presentation modes for the swapchain
	*				VK_PRESENT_MODE_MAILBOX_KHR is our first choice
	*				VK_PRESENT_MODE_IMMEDIATE_KHR is our second
	*				and VK_PRESENT_MODE_FIFO_KHR is guaranteed to be available
	*				and our third choice
	*
	*/
	inline VkPresentModeKHR chooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}

	/*
	* Function: chooseSwapchainExtent
	*
	* Paramters: const VkSurfaceCapabilitiesKHR& capabilities
	*				   GLFWwindow *pWind
	*
	* Return Type: VkExtent2D
	*
	* Description: Selects the best available resolution to be rendered to the surface
	*
	*/
	inline VkExtent2D chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow *pWind) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetWindowSize(pWind, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	/*
	* Function: findSupportedFormat
	*
	* Paramters:		VkPhysicalDevice dvc,
	*			 const	std::vector<VkFormat> &candidates,
	*					VkImageTiling tiling,
	*					VkFormatFeatureFlags features
	*
	* Return Type: VkFormat
	*
	* Description: searches for the best possible format we will use for the depth buffer
	*
	*/
	inline VkFormat findSupportedFormat(VkPhysicalDevice dvc,
										const std::vector<VkFormat> &candidates,
										VkImageTiling tiling,
										VkFormatFeatureFlags features) throw(Exception) {
		try {
			for (VkFormat format : candidates) {
				VkFormatProperties props;
				vkGetPhysicalDeviceFormatProperties(dvc, format, &props);
				if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
					return format;
				}
				else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
					return format;
				}
			}
			throw Exception("failed to find supported format", "Utilities.h", "findSupportedFormat");
		}
		catch (Exception &excpt) {
			throw excpt;
		}
	}

	/*
	* Function: ReadFile
	*
	* Paramters: const std::string& filename
	*
	* Return Type: std::vector<char>
	*
	* Description: reads in from a file and returns the file contents as a std::vector<char>
	*				will be used to interpret the shaders and load them into shader modules
	*
	*/
	inline std::vector<char> ReadFile(const std::string& filename) throw(Exception) {
		try {
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				throw Exception("failed to open file", "Utilities.h", "ReadFile");
			}

			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();

			return buffer;
		}
		catch (Exception &excpt) {
			throw excpt;
		}
	}
	/*
	* Function: createShaderModule
	*
	* Paramters: const std::vector<char> &code,
	*				   VkDevice &device
	*
	* Return Type: VkShaderModule
	*
	* Description: creates a shader module from the parameters and
	*				returns the module to be used as a local variable
	*				in the graphics PSO
	*
	*/
	inline VkShaderModule createShaderModule(const std::vector<char> &code, VkDevice &device) throw(Exception) {
		try {
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

			VkShaderModule shaderModule;
			if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
				throw Exception("failed to create shader module", "Utilities.h", "createShaderModule");
			}

			return shaderModule;
		}
		catch (Exception &excpt) {
			throw excpt;
		}
	}

	/*
	* Function: hasStencilComponent
	*
	* Paramters: VkFormat format
	*
	* Return Type: bool
	*
	* Description: checks if the format parameter has a either
	*				of the two formats for the stencil buffer
	*
	*/
	inline bool hasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	/*
	* Function: alignedAlloc
	*
	* Paramters: size_t size,
	*			 size_t alignment
	*
	* Return Type: void*
	*
	* Description: aligns memory and returns said aligned memory
	*				based upon the parameters
	*
	*/
	inline void* alignedAlloc(size_t size, size_t alignment) {
		void *data = nullptr;
		data = _aligned_malloc(size, alignment);
		return data;
	}

	/*
	* Function: alignedFree
	*
	* Paramters: void* data
	*
	* Return Type: void
	*
	* Description: frees the aligned memory it takes in as a parameter
	*
	*/
	inline void alignedFree(void* data) {
		_aligned_free(data);
	}
}