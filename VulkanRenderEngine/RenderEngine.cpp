/*
* TITLE:	Vulkan Render Engine
* BY:		Eric Hollas	
*		
* FILE:		RenderEngine.cpp
*	
* Correponds to RenderEngine.h (see for overall description)
*/
#include "stdafx.h"
#include "RenderEngine.h"
#include "Utilities.h"
#include "InitStructs.h"
#include "Debugger.h"



/*
* Function: initVulkan
*
* Paramters: GLFWwindow *pW
*
* Return Type: void
*
* Description: calls the various create functions to initialize the vulkan api
*				keep in mind, order is important for certain function calls if
*				it needs to be edited
*
*/
void RenderEngine::initVulkan(GLFWwindow *pW, Geometry::GeometryManager &management) throw(Exception) {
	try {
		pWindow = pW;
		pOffsetManager;
		pOffsetManager = &management;

		createInstance();
		Utilities::Debug::setupDebugCallback(instance, &callback);
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapchain();
		createImageViews();
		createRenderPass();
		createDescriptorSetLayout();
		createGraphicsPipeline();
		createCommandPool();
		createDepthResources();
		createFramebuffers();
		createVertexBuffer();
		createIndexBuffer();
		createUniformBuffer();
		createDescriptorPool();
		createDescriptorSet();
		createCommandBuffers();
		createSemaphores();
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: cleanup
*
* Paramters: none
*
* Return Type: void
*
* Description: calls the various delete/destroy functions to delete the vulkan api
*				keep in mind, order is important for certain function calls if
*				it needs to be edited
*
*/
void RenderEngine::cleanup() {
	vkDeviceWaitIdle(device);

	cleanupSwapchain();

	vkDestroyDescriptorPool(device, descriptorPool, nullptr);

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

	pViewUniformBuffer->~BufferObject();
	pDynamicUniformBuffer->~BufferObject();
	pIndexBuffer->~BufferObject();
	pVertexBuffer->~BufferObject();
	pOffsetManager->~GeometryManager();

	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);

	vkDestroyCommandPool(device, commandPool, nullptr);

	vkDestroyDevice(device, nullptr);

	if (isDebugging) {
		auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
		if (func != nullptr) {
			func(instance, callback, nullptr);
		}
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
}

/*
* Function: updateUniformBuffer
*
* Paramters: Camera::UniformBufferObject ubo,
*			 std::vector<glm::mat4> uniformMats
*
* Return Type: void
*
* Description: updates the dynamic uniform buffer data and the uniform buffer data, the function
*				will take in data from the event manager in main loop found in the window object
*
*/
void RenderEngine::updateUniformBuffer(Camera::UniformBufferObject ubo, std::vector<glm::mat4> uniformMatrices) {
	for (uint32_t i = 0; i < pOffsetManager->getNumOfObjects(); i++) {
		glm::mat4* modelMat = (glm::mat4*)(((uint64_t)dynamicUBO.pModel + (i * dynamicAlignment)));
		*modelMat = uniformMatrices[i];
	}

	pDynamicUniformBuffer->map();
	pDynamicUniformBuffer->copyTo(dynamicUBO.pModel, pDynamicUniformBuffer->size);
	pDynamicUniformBuffer->flush();
	pDynamicUniformBuffer->unmap();


	size_t sz = sizeof(ubo);
	pViewUniformBuffer->map(sz, 0);
	pViewUniformBuffer->copyTo(&ubo, sz);
	pViewUniformBuffer->unmap();
}
/*
* Function: drawFrame
*
* Paramters: none
*
* Return Type: void
*
* Description: ensures the swapchain does not need to be recreated and submits the queue to the
*				command buffer for drawing a frame and then submits the rendered frame to the
*				swap chain
*
*/
void RenderEngine::drawFrame() throw(Exception) {
	try {
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(device, swapchain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		//ensure swapchian does not need to be updated on size
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw Exception("failed to acquire swap chain image", "RenderEngine.cpp", "drawFrame");
		}

		//get semaphore info
		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };

		//submits the queue to the command buffer
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw Exception("failed to submit draw command buffer", "RenderEngine.cpp", "drawFrame");
		}

		//submits the present queue to the swapchain
		VkSwapchainKHR swapChains[] = { swapchain };
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			recreateSwapchain();
		}
		else if (result != VK_SUCCESS) {
			throw Exception("failed to present swap chain image", "RenderEngine.cpp", "drawFrame");
		}

		if (isDebugging) {
			vkQueueWaitIdle(presentQueue);
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}

/*
* Function: updateGeometryBuffers
*
* Paramters: GeometryManager &newManagement
*
* Return Type: void
*
* Description: Updates the member variable that stores the
*				vertices, indices, and offset info, then
*				updates the vertex, index, and command buffers
*				with the offset manager
*
*/
void RenderEngine::updateGeometryBuffers(Geometry::GeometryManager &newManagement) throw(Exception) {
	try {
		pOffsetManager = &newManagement;
		updateVertexBuffer();
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}





//***********************************END OF PUBLIC METHODS***********************************
//*******************************************************************************************



//***************************************************************************************************************************
//**********************Following functions pretain to instance, devices, debug report, and queues***************************

/*
* Function: createInstance
*
* Paramters: none
*
* Return Type: void
*
* Description: gets the extensions for the glfw and vulkan API's to
*				initialize the debug report and then creates the instane
*				of the vulkan api
*
*/
void RenderEngine::createInstance() throw(Exception) {
	try {
		if (isDebugging && !checkValidationLayerSupport()) {
			throw Exception("validation layers requested, but not available", "RenderEngine.cpp", "createInstance");
		}

		VkApplicationInfo appInfo = vkAPI::InitStructs::AppInfo();

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (isDebugging) {
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (isDebugging) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(Utilities::validationLayers.size());
			createInfo.ppEnabledLayerNames = Utilities::validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw Exception("failed to create instance", "RenderEngine.cpp", "createInstance");
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: checkValidationLayerSupport
*
* Paramters: none
*
* Return Type: bool
*
* Description: checks the layers queried in the createInstance function to ensure that
*				the layers are available
*
*/
bool RenderEngine::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : Utilities::validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}
/*
* Function: pickPhysicalDevice
*
* Paramters: none
*
* Return Type: void
*
* Description: searches for the logical device used for the renderring computations
*				and then selects the appropriate device based on the isDeviceSuitable
*				function from the utilities namespace
*
*/
void RenderEngine::pickPhysicalDevice() throw(Exception) {
	try {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw Exception("failed to find GPUs with Vulkan support", "RenderEngine.cpp", "pickPhysicalDevice");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (Utilities::isDeviceSuitable(device, surface)) {
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE) {
			throw Exception("failed to find a suitable GPU", "RenderEngine.cpp", "pickPhysicalDevice");
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createLogicalDevice
*
* Paramters: none
*
* Return Type: void
*
* Description: Initializes the logical device and initializes the graphics and
*				presentation queues to the queried queues from the logical device
*
*/
void RenderEngine::createLogicalDevice() throw(Exception) {
	try {
		Utilities::QueueFamilyIndices indices = Utilities::findQueueFamilies(physicalDevice, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

		float queuePriority = 1.0f;
		for (int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = vkAPI::InitStructs::QueueInfo(queueFamily, &queuePriority);
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(Utilities::deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = Utilities::deviceExtensions.data();
		if (isDebugging) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(Utilities::validationLayers.size());
			createInfo.ppEnabledLayerNames = Utilities::validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw Exception("failed to create logical device", "RenderEngine.cpp", "createLogicalDevice");
		}

		vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}


//**********************End of functions pretain to instance, devices, debug report, and queues******************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//**********************Following functions pretain to various aspects of the swap chain*************************************


/*
* Function: cleanupSwapchain
*
* Paramters: none
*
* Return Type: void
*
* Description: destroys the various aspects of the render engine associated
*				with the swapchain to re-initlize them with the new window
*				size.
*				This will be called in the general cleanup function as well
*				when the window has been resized.
*
*/
void RenderEngine::cleanupSwapchain() {
	vkDestroyImageView(device, depthImageView, nullptr);
	vkDestroyImage(device, depthImage, nullptr);
	vkFreeMemory(device, depthImageMemory, nullptr);

	for (auto framebuffer : swapchainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);

	for (auto imageView : swapchainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device, swapchain, nullptr);
}
/*
* Function: recreateSwapchain
*
* Paramters: none
*
* Return Type: void
*
* Description: re-initializes the member class variables associated with
*				the swapchain.
*				Note that the cleanup swapchian must be called before this function
*
*/
void RenderEngine::recreateSwapchain() throw(Exception) {
	try {
		int width, height;
		glfwGetWindowSize(pWindow, &width, &height);
		if (width == 0 || height == 0)
			return;

		vkDeviceWaitIdle(device);

		cleanupSwapchain();

		createSwapchain();
		createImageViews();


		createRenderPass();
		createGraphicsPipeline();
		createDepthResources();
		createFramebuffers();
		createCommandBuffers();
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}

/*
* Function: createSurface
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the surface associated with the swapchain
*
*/
void RenderEngine::createSurface() throw(Exception) {
	try {
		if (glfwCreateWindowSurface(instance, pWindow, nullptr, &surface) != VK_SUCCESS) {
			throw Exception("failed to create window surface", "RenderEngine.cpp", "createSurface");
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createSwapchain
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the swapchain and associates the swapchain with the
*				surface, graphics and presentation queues, and the logical device
*				Note that the objects to be asociated with the swap chain must
*				be created before the swapchain
*
*/
void RenderEngine::createSwapchain() throw(Exception) {
	try {
		Utilities::SwapchainSupportDetails swapchainSupport = Utilities::querySwapchainSupport(physicalDevice, surface);

		VkSurfaceFormatKHR surfaceFormat = Utilities::chooseSwapchainSurfaceFormat(swapchainSupport.formats);
		VkPresentModeKHR presentMode = Utilities::chooseSwapchainPresentMode(swapchainSupport.presentModes);
		VkExtent2D extent = Utilities::chooseSwapchainExtent(swapchainSupport.capabilities, pWindow);

		uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
		if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
			imageCount = swapchainSupport.capabilities.maxImageCount;
		}

		Utilities::QueueFamilyIndices indices = Utilities::findQueueFamilies(physicalDevice, surface);
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
			throw Exception("failed to create swap chain", "RenderEngine.cpp", "createSwapChain");
		}

		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
		swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

		swapchainImageFormat = surfaceFormat.format;
		swapchainExtent = extent;
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createFramebuffers
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the frambuffers for the swapchain based on the swapchain image views
*
*/
void RenderEngine::createFramebuffers() throw(Exception) {
	try {
		swapchainFramebuffers.resize(swapchainImageViews.size());

		for (size_t i = 0; i < swapchainImageViews.size(); i++) {
			std::array<VkImageView, 2> attachments = { swapchainImageViews[i], depthImageView };

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapchainExtent.width;
			framebufferInfo.height = swapchainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS) {
				throw Exception("failed to create framebuffer", "RenerEngine.cpp", "createFrameBuffers");
			}
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}


//**********************End of functions pretain to various aspects of the swap chain****************************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//**********************Following functions pretain to various aspects of the image views************************************


/*
* Function: createImage
*
* Paramters: VkFormat format,
*			 VkImageTiling tiling,
*			 VkImageUsageFlags usage,
*			 VkMemoryPropertyFlags properties,
*			 VkImage& image,
*			 VkDeviceMemory& imageMemory
*
* Return Type: void
*
* Description: Creates the image used for the depth buffer
*				and will be used for textures if textures are desired
*
*/
void RenderEngine::createImage(VkFormat format,
							   VkImageTiling tiling,
							   VkImageUsageFlags usage,
							   VkMemoryPropertyFlags properties,
							   VkImage& image,
							   VkDeviceMemory& imageMemory) throw(Exception) {
	try {
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = swapchainExtent.width;
		imageInfo.extent.height = swapchainExtent.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw Exception("failed to create image", "RenderEngine.cpp", "createImage");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Utilities::findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw Exception("failed to allocate image memory", "RenderEngine.cpp", "createImage");
		}

		vkBindImageMemory(device, image, imageMemory, 0);
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createImageViews
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the image views based on the swapchain image views
*
*/
void RenderEngine::createImageViews() {
	swapchainImageViews.resize(swapchainImages.size());

	for (size_t i = 0; i < swapchainImages.size(); i++) {
		swapchainImageViews[i] = createView(swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}
/*
* Function: createView
*
* Paramters: VkImage image,
*			 VkFormat format,
*			 VkImageAspectFlags aspectFlags
*
* Return Type: VkImageView
*
* Description: creates the individual image view to be called in
*				depthResources() and createImageViews()
*
*/
VkImageView RenderEngine::createView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) throw(Exception) {
	try {
		VkImageViewCreateInfo viewInfo = vkAPI::InitStructs::ImageViewInfo(image, format, aspectFlags);
		VkImageView iv;
		if (vkCreateImageView(device, &viewInfo, nullptr, &iv) != VK_SUCCESS) {
			throw Exception("failed to create image views", "RenderEngine.cpp", "createView");
		}
		return iv;
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: transitionImageLayout
*
* Paramters: VkImage image,
*			 VkFormat format,
*			 VkImageLayout oldLayout,
*			 VkImageLayout newLayout
*
* Return Type: void
*
* Description: Defines the ;ayout of the depth buffer's image
*				uses a pipeline barrier because the transition
*				only needs to happen once in the command buffer
*
*/
void RenderEngine::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) throw(Exception) {
	try {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (Utilities::hasStencilComponent(format)) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else {
			throw Exception("unsupported layout transition", "RenderEngine.cpp", "transitionImageLayout");
		}

		vkCmdPipelineBarrier(commandBuffer,
							 sourceStage,
							 destinationStage,
							 0,
							 0,
							 nullptr,
							 0,
							 nullptr,
							 1,
							 &barrier);

		endSingleTimeCommands(commandBuffer);
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createDepthResources
*
* Paramters: none
*
* Return Type: void
*
* Description: sets up the resources to create the depth buffer, which are
*				the image, the view, and the transition layout
*
*/
void RenderEngine::createDepthResources() throw(Exception) {
	try {
		VkFormat depthFormat = Utilities::findSupportedFormat(physicalDevice,
															  { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
															  VK_IMAGE_TILING_OPTIMAL,
															  VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
		createImage(depthFormat,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					depthImage,
					depthImageMemory);
		depthImageView = createView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
		transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: beginSingleTimeCommands
*
* Paramters: none
*
* Return Type: VkCommandBuffer
*
* Description: the function that starts the transition layout in the
*				command buffer, thus this returns the command buffer
*				to transition the image through the depth buffer
*
*/
VkCommandBuffer RenderEngine::beginSingleTimeCommands() {
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}
/*
* Function: endSingleTimeCommands
*
* Paramters: VkCommandBuffer commandBuffer
*
* Return Type: void
*
* Description: this deletes the portion of the command buffer
*				that processes the depth image transition
*
*/
void RenderEngine::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}


//**********************End of functions pretain to various aspects of the image views***************************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//**********************Following functions pretain to the various buffers***************************************************


/*
* Function: copyBuffer
*
* Paramters: VkBuffer srcBuffer,
*			 VkBuffer dstBuffer,
*			 VkDeviceSize size
*
* Return Type: void
*
* Description: copies a buffers data from the srcBuffer parameter
*				to the dstBuffer parameter. This will be called
*				to move data from the staging buffer to the intended buffer
*
*/
void RenderEngine::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}
/*
* Function: createVertexBuffer
*
* Paramters: none
*
* Return Type: void
*
* Description: uses a staging buffer to create the vertex buffer that
*				stores the vertices to be rendered
*
*/
void RenderEngine::createVertexBuffer() throw(Exception) {
	try {
		VkDeviceSize bufferSize = sizeof(Geometry::Vertex) * pOffsetManager->getTotalVertices();

		//creates the staging buffer
		vkAPI::Buffer::BufferObject * pStagingBuffer;
		pStagingBuffer = new vkAPI::Buffer::BufferObject(device);
		if (pStagingBuffer->createBuffer(bufferSize,
										 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
										 physicalDevice,
										 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
			throw Exception("failed to create buffer", "RenderEngine.cpp", "createVertexBuffer");
		}
		pStagingBuffer->bind();

		//stores the vertex data into the staging buffer
		pStagingBuffer->map(bufferSize);
		pStagingBuffer->copyTo((void*)pOffsetManager->getVertices().data(), bufferSize);
		pStagingBuffer->unmap();

		//creates the vertex buffer
		pVertexBuffer = new vkAPI::Buffer::BufferObject(device);
		if (pVertexBuffer->createBuffer(bufferSize,
										VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
										physicalDevice,
										VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
			throw Exception("failed to create buffer", "RenderEngine.cpp", "createVertexBuffer");
		}
		pVertexBuffer->bind();

		//copies the data from the staging buffer to the vertex buffer
		copyBuffer(pStagingBuffer->buffer, pVertexBuffer->buffer, bufferSize);

		pStagingBuffer->~BufferObject();
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: updateVertexBuffer
*
* Paramters: none
*
* Return Type: void
*
* Description: deletes the command buffer and the index and vertex buffers,
*				then recreates the command buffer to account for the updated
*				offsets in the vertex and index buffers, the creation function
*				of the command buffer does not need to change
*
*/
void RenderEngine::updateVertexBuffer() throw(Exception) {
	try {
		vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

		pVertexBuffer->~BufferObject();
		pIndexBuffer->~BufferObject();

		createVertexBuffer();
		createIndexBuffer();

		createCommandBuffers();
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createIndexBuffer
*
* Paramters: none
*
* Return Type: void
*
* Description: uses a staging buffer to create the index buffer that
*				stores the vertices to be rendered
*
*/
void RenderEngine::createIndexBuffer() throw(Exception) {
	try {
		VkDeviceSize bufferSize = sizeof(uint32_t) * pOffsetManager->getTotalIndices();

		//creates the staging buffer
		vkAPI::Buffer::BufferObject * pStagingBuffer;
		pStagingBuffer = new vkAPI::Buffer::BufferObject(device);
		if (pStagingBuffer->createBuffer(bufferSize,
										 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
										 physicalDevice,
										 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
			throw Exception("failed to create buffer", "RenderEngine.cpp", "createIndexBuffer");
		}
		pStagingBuffer->bind();

		//stores the index data into the staging buffer
		pStagingBuffer->map(bufferSize);
		pStagingBuffer->copyTo((void*)pOffsetManager->getIndicies().data(), bufferSize);
		pStagingBuffer->unmap();

		//creates the index buffer
		pIndexBuffer = new vkAPI::Buffer::BufferObject(device);
		if (pIndexBuffer->createBuffer(bufferSize,
										VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
										physicalDevice,
										VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
			throw Exception("failed to create buffer", "RenderEngine.cpp", "createIndexBuffer");
		}
		pIndexBuffer->bind();

		//copies the data from the staging buffer to the index buffer
		copyBuffer(pStagingBuffer->buffer, pIndexBuffer->buffer, bufferSize);

		pStagingBuffer->~BufferObject();
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createUniformBuffer
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the uniform buffer for the camera matrices and
*				then creates the dynamic uniform buffers. Calls utility
*				functions to set the memory alignment to set the matrices
*				to eventually match the offsets in the vertex and index buffers
*
*/
void RenderEngine::createUniformBuffer() throw(Exception) {
	try {
		VkDeviceSize bufferSize = sizeof(Camera::UniformBufferObject);
		pViewUniformBuffer = new vkAPI::Buffer::BufferObject(device);
		if (pViewUniformBuffer->createBuffer(bufferSize,
											 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
											 physicalDevice,
											 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
			throw Exception("failed to create uniform buffer", "RenderEngine.cpp", "createUniformBuffer");
		}
		pViewUniformBuffer->bind();

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(physicalDevice, &props);
		size_t minAlignment = props.limits.minUniformBufferOffsetAlignment;
		dynamicAlignment = sizeof(glm::mat4);
		if (minAlignment > 0) {
			dynamicAlignment = (dynamicAlignment + minAlignment - 1) & ~(minAlignment - 1);
		}
		size_t dynamicBufferSize = pOffsetManager->getNumOfObjects() * dynamicAlignment;

		dynamicUBO.pModel = (glm::mat4*)Utilities::alignedAlloc(dynamicBufferSize, dynamicAlignment);
		assert(dynamicUBO.pModel);

		pDynamicUniformBuffer = new vkAPI::Buffer::BufferObject(device);
		if (pDynamicUniformBuffer->createBuffer(dynamicBufferSize,
												VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
												physicalDevice,
												VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
			throw Exception("failed to create dynamic uniform buffer", "RenderEngine.cpp", "createUniformBuffer");
		}
		pDynamicUniformBuffer->bind();
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}


//**********************End of functions pretain to the various buffers******************************************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//**********************Following functions pretain to the various aspects of the descriptors********************************


/*
* Function: createDescriptorPool
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the descriptor pool for the uniform and dynamic uniform buffers
*
*/
void RenderEngine::createDescriptorPool() throw(Exception) {
	try {
		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = 1;
		VkDescriptorPoolSize poolSizeDynm = {};
		poolSizeDynm.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		poolSizeDynm.descriptorCount = 1;
		std::vector<VkDescriptorPoolSize> sizes = { poolSize, poolSizeDynm };

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
		poolInfo.pPoolSizes = sizes.data();
		poolInfo.maxSets = 1;

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			throw Exception("failed to create descriptor pool", "RenderEngine.cpp", "createDescriptorPool");
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createDescriptorSet
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the descriptor sets for the uniform and dynamic uniform buffers
*				to synch the buffers to the shaders
*
*/
void RenderEngine::createDescriptorSet() throw(Exception) {
	try {
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorSetLayout;

		if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw Exception("failed to allocate descriptor set", "RenderEngine.cpp", "createDescriptorSet");
		}

		VkDescriptorBufferInfo bufferViewInfo = pViewUniformBuffer->setupDescriptorInfo(sizeof(Camera::UniformBufferObject), 0);
		VkDescriptorBufferInfo bufferDynamicInfo = pDynamicUniformBuffer->setupDescriptorInfo(sizeof(Geometry::DynamicUniformBufferObject), 0);

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferViewInfo;
		VkWriteDescriptorSet descriptorWriteDynm = {};
		descriptorWriteDynm.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteDynm.dstSet = descriptorSet;
		descriptorWriteDynm.dstBinding = 1;
		descriptorWriteDynm.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWriteDynm.descriptorCount = 1;
		descriptorWriteDynm.pBufferInfo = &bufferDynamicInfo;

		std::vector<VkWriteDescriptorSet> writes = { descriptorWrite, descriptorWriteDynm };

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createDescriptorSetLayout
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the descriptor set layout for the uniform and dynamic uniform buffers
*				to synch the buffers to the shaders
*/
void RenderEngine::createDescriptorSetLayout() throw(Exception) {
	try {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		VkDescriptorSetLayoutBinding uboLayoutBindingDynm = {};
		uboLayoutBindingDynm.binding = 1;
		uboLayoutBindingDynm.descriptorCount = 1;
		uboLayoutBindingDynm.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		uboLayoutBindingDynm.pImmutableSamplers = nullptr;
		uboLayoutBindingDynm.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		std::vector<VkDescriptorSetLayoutBinding> layouts = { uboLayoutBinding, uboLayoutBindingDynm };

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(layouts.size());
		layoutInfo.pBindings = layouts.data();

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			throw Exception("failed to create descriptor set layout", "RenderEngine.cpp", "createDescriptorSetLayout");
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}


//**********************End of functions pretain to the various aspects of the descriptors***********************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//**********************Following functions pretain to the render pass and graphics PSO**************************************


/*
* Function: createRenderPass
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the render pass to add the color attachment for color
*				blending and the depth attacment for the depth buffer
*
*/
void RenderEngine::createRenderPass() throw(Exception) {
	try {
		VkFormat depthFormat = Utilities::findSupportedFormat(physicalDevice,
															  { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
															  VK_IMAGE_TILING_OPTIMAL,
															  VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
		VkAttachmentDescription colorAttachment = vkAPI::InitStructs::AttachmentInfo(swapchainImageFormat, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		VkAttachmentReference   colorAttachmentRef = vkAPI::InitStructs::AttachmentRef(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		VkAttachmentDescription depthAttachment = vkAPI::InitStructs::AttachmentInfo(depthFormat, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		VkAttachmentReference   depthAttachmentRef = vkAPI::InitStructs::AttachmentRef(1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		VkSubpassDependency     dependency = vkAPI::InitStructs::SubpassDepend();

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw Exception("failed to create render pass", "RenderEngine.cpp", "createRenderPass");
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}

/*
* Function: createGraphicsPipeline
*
* Paramters: none
*
* Return Type: void
*
* Description: Creates the graphics PSO,
*				starts by creating the shader modules and then arranges
*				the various other aspects of the graphics PSO, just make
*				sure the shaders are destroyed at the end of the function
*
*/
void RenderEngine::createGraphicsPipeline() throw(Exception) {
	try {
		//setting up structs and data for the shader stage and vertex input state for pipeline object
		auto vertShaderCode = Utilities::ReadFile(VERTEX_SHADER_PATH);
		auto fragShaderCode = Utilities::ReadFile(FRAGMENT_SHADER_PATH);

		auto bindingDescription = Geometry::Vertex::getBindingDescription();
		auto attributeDescriptions = Geometry::Vertex::getAttributeDescriptions();

		VkShaderModule vertShaderModule = Utilities::createShaderModule(vertShaderCode, device);
		VkShaderModule fragShaderModule = Utilities::createShaderModule(fragShaderCode, device);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo = vkAPI::InitStructs::ShaderInfo(true, vertShaderModule);
		VkPipelineShaderStageCreateInfo fragShaderStageInfo = vkAPI::InitStructs::ShaderInfo(false, fragShaderModule);
		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		//setting up structs for viewport state for pipeline object
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = vkAPI::InitStructs::InputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		VkViewport viewport = vkAPI::InitStructs::ViewportInfo((float)swapchainExtent.width, (float)swapchainExtent.height);
		VkRect2D scissor = vkAPI::InitStructs::ScissorInfo(swapchainExtent);
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		//setting up rasterizer state struct for pipeline object
		VkPipelineRasterizationStateCreateInfo rasterizer = vkAPI::InitStructs::RasterizerInfo(VK_POLYGON_MODE_FILL, 1.0f, VK_FRONT_FACE_COUNTER_CLOCKWISE);
		//setting up multisample state struct for pipeline object
		VkPipelineMultisampleStateCreateInfo multisampling = vkAPI::InitStructs::MultiSampleInfo();

		//setting up color blend state struct for pipeline object
		VkPipelineColorBlendAttachmentState colorBlendAttachment = vkAPI::InitStructs::ColorBlendAttachmentInfo();
		VkPipelineColorBlendStateCreateInfo colorBlending = vkAPI::InitStructs::ColorBlendInfo();
		colorBlending.pAttachments = &colorBlendAttachment;

		//setting up depth stencil state struct for pipeline object
		VkPipelineDepthStencilStateCreateInfo depthStencil = {};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f;
		depthStencil.maxDepthBounds = 1.0f;
		depthStencil.stencilTestEnable = VK_FALSE;

		//create pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw Exception("failed to create pipeline layout", "RenderEngine.cpp", "createGraphicsPipeLine");
		}

		//create the pipeline object
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
			throw Exception("failed to create graphics pipeline", "RenderEngine.cpp", "createGraphicsPipeLine");
		}

		//destroy the shaders
		vkDestroyShaderModule(device, fragShaderModule, nullptr);
		vkDestroyShaderModule(device, vertShaderModule, nullptr);
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}


//**********************End of functions pretain to the render pass and graphics PSO*****************************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//**********************Following functions pretain to the semaphores********************************************************


/*
* Function: createSemaphores
*
* Paramters: none
*
* Return Type: void
*
* Description: Creates the semaphores to be ensure that something wont be deleted
*				while it is being used to render something new. The semaphores
*				are also used to control the double bufferring in the swapchain
*				and the command buffer.
*
*/
void RenderEngine::createSemaphores() throw(Exception) {
	try {
		VkSemaphoreCreateInfo semaphoreInfo = vkAPI::InitStructs::SemaphoreInfo();

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {

			throw Exception("failed to create semaphores", "RenderEngine.cpp", "createSemaphores");
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}


//**********************End of functions pretain to the semaphores***********************************************************
//***************************************************************************************************************************
//***************************************************************************************************************************
//**********************Following functions pretain to the command buffer****************************************************


/*
* Function: createCommandPool
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the command pool for the queues in the command buffer
*
*/
void RenderEngine::createCommandPool() throw(Exception) {
	try {
		VkCommandPoolCreateInfo poolInfo = vkAPI::InitStructs::CmdPoolInfo(Utilities::findQueueFamilies(physicalDevice, surface).graphicsFamily);
		if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw Exception("failed to create graphics command pool", "RenderEngine.cpp", "createCommandPool");
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}
/*
* Function: createCommandBuffers
*
* Paramters: none
*
* Return Type: void
*
* Description: creates the command buffers that run the render pass,
*				then creates the rendered image from the index and
*				vertex buffer using the OffsetManager to draw the
*				correct indices and vertices fromt each correct offset
*
*/
void RenderEngine::createCommandBuffers() throw(Exception) {
	try {
		commandBuffers.resize(swapchainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw Exception("failed to allocate command buffers", "RenderEngine.cpp", "createCommandBuffer");
		}

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		std::array<VkClearValue, 2> clearValues = {};
		glm::vec3 clearCol = Geometry::getColor(Geometry::black);
		clearValues[0].color = { clearCol.x, clearCol.y, clearCol.z, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapchainExtent;
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		//draws to the frames
		for (size_t i = 0; i < commandBuffers.size(); i++) {
			vkBeginCommandBuffer(commandBuffers[i], &beginInfo);
			renderPassInfo.framebuffer = swapchainFramebuffers[i];
			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			//offsets the device memory to be drawn
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &(pVertexBuffer->buffer), offsets);
			vkCmdBindIndexBuffer(commandBuffers[i], pIndexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);

			//draws each object to pair with the correct unifrom matrix with the dynamic uniform buffer
			//   each object is managed by the offset manager
			for (uint32_t objectIndex = 0; objectIndex < pOffsetManager->getNumOfObjects(); objectIndex++) {
				uint32_t dynamicOffset = objectIndex * static_cast<uint32_t>(dynamicAlignment);
				vkCmdBindDescriptorSets(commandBuffers[i],
										VK_PIPELINE_BIND_POINT_GRAPHICS,
										pipelineLayout,
										0,
										1,
										&descriptorSet,
										1,
										&dynamicOffset);
				vkCmdDrawIndexed(commandBuffers[i],
								 pOffsetManager->getIndiciesInObject(objectIndex),
								 1,
								 pOffsetManager->getIndexOffset(objectIndex),
								 pOffsetManager->getVertexOffset(objectIndex),
								 0);
			}

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw Exception("failed to record command buffer", "RenderEngine.cpp", "createCommandBuffer");
			}
		}
	}
	catch (Exception &excpt) {
		throw excpt;
	}
}


//**********************End of functions pretain to the command buffer*******************************************************
//***************************************************************************************************************************