/*
* TITLE:	Vulkan Render Engine
* BY:		Eric Hollas	
*		
* FILE:		InitStructs.h	
* DETAILS:	This file adds to the vkAPI namespace basic functions to initialize the 
*				many generic struct objects used throughout the Vulkan API in the
*				Render Engine.	
*/
#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vkAPI {
	namespace InitStructs {
		inline VkApplicationInfo AppInfo() {
			VkApplicationInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			info.pApplicationName = APP_NAME;
			info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			info.pEngineName = ENGINE_NAME;
			info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			info.apiVersion = VK_API_VERSION_1_0;
			return info;
		}
		inline VkDeviceQueueCreateInfo QueueInfo(int family, float *priority) {
			VkDeviceQueueCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			info.queueFamilyIndex = family;
			info.queueCount = 1;
			info.pQueuePriorities = priority;
			return info;
		}
		inline VkImageViewCreateInfo ImageViewInfo(VkImage img, VkFormat frmt, VkImageAspectFlags mask) {
			VkImageViewCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.image = img;
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = frmt;
			info.subresourceRange.aspectMask = mask;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;
			return info;
		}
		inline VkAttachmentDescription AttachmentInfo(VkFormat frmt, VkImageLayout finalLayout) {
			VkAttachmentDescription info = {};
			info.format = frmt;
			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			info.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			info.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			info.finalLayout = finalLayout;
			return info;
		}
		inline VkAttachmentReference AttachmentRef(uint32_t count, VkImageLayout layout) {
			VkAttachmentReference info = {};
			info.attachment = count;
			info.layout = layout;
			return info;
		}
		inline VkSubpassDependency SubpassDepend() {
			VkSubpassDependency info = {};
			info.srcSubpass = VK_SUBPASS_EXTERNAL;
			info.dstSubpass = 0;
			info.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			info.srcAccessMask = 0;
			info.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			info.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			return info;
		}
		inline VkPipelineShaderStageCreateInfo ShaderInfo(bool isVertexShader, VkShaderModule mod) {
			VkPipelineShaderStageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info.module = mod;
			info.pName = "main";
			if (isVertexShader) {
				info.stage = VK_SHADER_STAGE_VERTEX_BIT;
			}
			else {
				info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			}
			return info;
		}
		inline VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo(VkPrimitiveTopology topo) {
			VkPipelineInputAssemblyStateCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			info.topology = topo;
			info.primitiveRestartEnable = VK_FALSE;
			return info;
		}
		inline VkViewport ViewportInfo(float wide, float tall) {
			VkViewport info = {};
			info.x = 0.0f;
			info.y = 0.0f;
			info.width = (float)wide;
			info.height = (float)tall;
			info.minDepth = 0.0f;
			info.maxDepth = 1.0f;
			return info;
		}
		inline VkRect2D ScissorInfo(VkExtent2D extent) {
			VkRect2D info = {};
			info.offset = { 0, 0 };
			info.extent = extent;
			return info;
		}
		inline VkPipelineRasterizationStateCreateInfo RasterizerInfo(VkPolygonMode polyMode,
			float lineSize,
			VkFrontFace cullOrientation) {
			VkPipelineRasterizationStateCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			info.depthClampEnable = VK_FALSE;
			info.rasterizerDiscardEnable = VK_FALSE;
			info.polygonMode = polyMode;
			info.lineWidth = lineSize;
			info.cullMode = VK_CULL_MODE_BACK_BIT;
			info.frontFace = cullOrientation;
			info.depthBiasEnable = VK_FALSE;
			return info;
		}
		inline VkPipelineMultisampleStateCreateInfo MultiSampleInfo() {
			VkPipelineMultisampleStateCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			info.sampleShadingEnable = VK_FALSE;
			info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			return info;
		}
		inline VkPipelineColorBlendAttachmentState ColorBlendAttachmentInfo() {
			VkPipelineColorBlendAttachmentState info = {};
			info.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			info.blendEnable = VK_FALSE;
			return info;
		}
		inline VkPipelineColorBlendStateCreateInfo ColorBlendInfo() {
			VkPipelineColorBlendStateCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			info.logicOpEnable = VK_FALSE;
			info.logicOp = VK_LOGIC_OP_COPY;
			info.attachmentCount = 1;
			info.blendConstants[0] = 0.0f;
			info.blendConstants[1] = 0.0f;
			info.blendConstants[2] = 0.0f;
			info.blendConstants[3] = 0.0f;
			return info;
		}
		inline VkCommandPoolCreateInfo CmdPoolInfo(uint32_t familyIndex) {
			VkCommandPoolCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			info.queueFamilyIndex = familyIndex;
			return info;
		}
		inline VkSemaphoreCreateInfo SemaphoreInfo() {
			VkSemaphoreCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			return info;
		}
		inline VkBufferCreateInfo BufferInfo(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode mode) {
			VkBufferCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.size = size;
			info.usage = usage;
			info.sharingMode = mode;
			return info;
		}
	}
}