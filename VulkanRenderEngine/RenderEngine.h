#pragma once
#include "stdafx.h"
#include "Exception.h"
#include "GeometryManager.h"
#include "Camera.h"
#include "Buffer.h"

class RenderEngine {
public:
	void initVulkan(GLFWwindow *pW, Geometry::GeometryManager &newManagement) throw(Exception);
	void cleanup();

	void updateUniformBuffer(Camera::UniformBufferObject ubo, std::vector<glm::mat4> uniformMats);
	void drawFrame() throw(Exception);

	void updateGeometryBuffers(Geometry::GeometryManager &newManagement) throw(Exception);


private:
	GLFWwindow * pWindow;
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


	void createInstance() throw(Exception);
	bool checkValidationLayerSupport();
	void pickPhysicalDevice() throw(Exception);
	void createLogicalDevice() throw(Exception);

	void cleanupSwapchain();
	void recreateSwapchain() throw(Exception);
	void createSurface() throw(Exception);
	void createSwapchain() throw(Exception);
	void createFramebuffers() throw(Exception);

	void createImage(VkFormat format,
					 VkImageTiling tiling,
					 VkImageUsageFlags usage,
					 VkMemoryPropertyFlags properties,
					 VkImage& image,
					 VkDeviceMemory& imageMemory) throw(Exception);
	void createImageViews();
	VkImageView createView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) throw(Exception);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) throw(Exception);
	void createDepthResources();
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void createVertexBuffer() throw(Exception);
	void updateVertexBuffer() throw(Exception);
	void createIndexBuffer() throw(Exception);
	void createUniformBuffer() throw(Exception);

	void createDescriptorPool() throw(Exception);
	void createDescriptorSet() throw(Exception);
	void createDescriptorSetLayout() throw(Exception);

	void createRenderPass() throw(Exception);
	void createGraphicsPipeline() throw(Exception);

	void createSemaphores() throw(Exception);

	void createCommandPool() throw(Exception);
	void createCommandBuffers() throw(Exception);
};