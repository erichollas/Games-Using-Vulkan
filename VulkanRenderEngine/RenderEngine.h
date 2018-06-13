/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		RenderEngine.h																*
* DETAILS:	This file defines the render engine object. This uses the Vulkan API to		*
*				create the object that defines how the game will be rendered to the		*
*				display.																*
*			Note: because this file and its corresponding .cpp file are much longer		*
*				than other files, functions listed in this file are in the same order	*
*				in the corresponding .cpp file. Also, private functions are grouped		*
*				and the same order as the class memeber values that they edit.			*
*																						*
*****************************************************************************************/

#pragma once
#include "stdafx.h"
#include "../../VulkanRenderEngine/RenderEngine/GeometryManager.h"
#include "../../VulkanRenderEngine/RenderEngine/Camera.h"
#include "../../VulkanRenderEngine/RenderEngine/Buffer.h"

class RenderEngine {
public:
	void initVulkan(GLFWwindow *pW, Geometry::GeometryManager &newManagement);
	void cleanup();

	void updateUniformBuffer(Camera::UniformBufferObject ubo, std::vector<glm::mat4> uniformMats);
	void drawFrame();

	void updateGeometryBuffers(Geometry::GeometryManager &newManagement);


private:
	GLFWwindow *						 pWindow;
	Geometry::GeometryManager *			 pOffsetManager;

	VkInstance							 instance;
	VkDebugReportCallbackEXT			 callback;
	VkPhysicalDevice					 physicalDevice = VK_NULL_HANDLE;
	VkDevice							 device;
	VkQueue								 graphicsQueue;
	VkQueue								 presentQueue;

	VkSurfaceKHR						 surface;
	VkSwapchainKHR						 swapchain;
	std::vector<VkImage>				 swapchainImages;
	VkFormat							 swapchainImageFormat;
	VkExtent2D							 swapchainExtent;
	std::vector<VkImageView>			 swapchainImageViews;
	std::vector<VkFramebuffer>			 swapchainFramebuffers;

	VkImage								 depthImage;
	VkDeviceMemory						 depthImageMemory;
	VkImageView							 depthImageView;

	Geometry::DynamicUniformBufferObject dynamicUBO;
	size_t								 dynamicAlignment;

	vkAPI::Buffer::BufferObject *		 pVertexBuffer;
	vkAPI::Buffer::BufferObject *		 pIndexBuffer;
	vkAPI::Buffer::BufferObject *		 pViewUniformBuffer;
	vkAPI::Buffer::BufferObject *		 pDynamicUniformBuffer;

	VkDescriptorPool					 descriptorPool;
	VkDescriptorSet						 descriptorSet;
	VkDescriptorSetLayout				 descriptorSetLayout;

	VkRenderPass						 renderPass;
	VkPipelineLayout					 pipelineLayout;
	VkPipeline							 graphicsPipeline;

	VkSemaphore							 imageAvailableSemaphore;
	VkSemaphore							 renderFinishedSemaphore;

	VkCommandPool						 commandPool;
	std::vector<VkCommandBuffer>		 commandBuffers;


	void createInstance();
	bool checkValidationLayerSupport();
	void pickPhysicalDevice();
	void createLogicalDevice();

	void cleanupSwapchain();
	void recreateSwapchain();
	void createSurface();
	void createSwapchain();
	void createFramebuffers();

	void createImage(VkFormat format,
					 VkImageTiling tiling,
					 VkImageUsageFlags usage,
					 VkMemoryPropertyFlags properties,
					 VkImage& image,
					 VkDeviceMemory& imageMemory);
	void createImageViews();
	VkImageView createView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void createDepthResources();
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void createVertexBuffer();
	void updateVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffer();

	void createDescriptorPool();
	void createDescriptorSet();
	void createDescriptorSetLayout();

	void createRenderPass();
	void createGraphicsPipeline();

	void createSemaphores();

	void createCommandPool();
	void createCommandBuffers();
};
