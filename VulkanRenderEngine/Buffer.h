/*
* TITLE:	Vulkan Render Engine
* BY:		Eric Hollas	
*
* FILE:		Buffer.h
* DETAILS:	This file defines the buffer object to be used in several instances in
*				RenderEngine. It is abstracted as an object in this file.
*/
#pragma once
#include "Exception.h"
#include "Utilities.h"
#include "InitStructs.h"

namespace vkAPI {
	namespace Buffer {
		struct BufferObject {
			VkDevice device;
			VkBuffer buffer;
			VkDeviceMemory memory;
			VkDeviceSize size;
			VkDeviceSize alignment;
			VkBufferUsageFlags usageFlags;
			VkMemoryPropertyFlags memoryPropertyFlags;
			VkDescriptorBufferInfo descriptorInfo;
			void * pMapped;


			/*
			* Constructor
			*/
			BufferObject(VkDevice physclDvc) {
				device = physclDvc;
				buffer = VK_NULL_HANDLE;
				memory = VK_NULL_HANDLE;
				size = 0;
				alignment = 0;
				pMapped = nullptr;
			}
			/*
			* Deconstructor
			*/
			~BufferObject() {
				if (buffer) {
					vkDestroyBuffer(device, buffer, nullptr);
				}
				if (memory) {
					vkFreeMemory(device, memory, nullptr);
				}
			}

			/*
			* Function: bind
			*
			* Paramters: VkDeviceSize offset
			*
			* Return Type: void
			*
			* Description: binds the buffer to its memory
			*
			*/
			void bind(VkDeviceSize offset = 0) throw(Exception) {
				try {
					if (vkBindBufferMemory(device, buffer, memory, offset)) {
						throw Exception("failed to bind buffer to buffer memory", "Buffer.h", "bind");
					}
				}
				catch (Exception &excpt) {
					throw excpt;
				}
			}
			/*
			* Function: copyTo
			*
			* Paramters: void * pData,
			*			 VkDeviceSize devSize
			*
			* Return Type: void
			*
			* Description: coppies the data parameter to the pMapped member variable
			*
			*/
			void copyTo(void * pData, VkDeviceSize devSize) {
				assert(pMapped);
				memcpy(pMapped, pData, devSize);
			}
			/*
			* Function: unmap
			*
			* Paramters: none
			*
			* Return Type: void
			*
			* Description: unmaps the pMapped member variable
			*
			*/
			void unmap() {
				if (pMapped) {
					vkUnmapMemory(device, memory);
					pMapped = nullptr;
				}
			}
			/*
			* Function: setupDescriptorInfo
			*
			* Paramters: VkDeviceSize devSize
			*			 VkDeviceSize offset
			*
			* Return Type: VkDescriptorBufferInfo
			*
			* Description: sets and returns the struct to used for the buffer
			*				in the descriptor
			*
			*/
			VkDescriptorBufferInfo setupDescriptorInfo(VkDeviceSize devSize = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
				descriptorInfo.offset = offset;
				descriptorInfo.buffer = buffer;
				descriptorInfo.range = devSize;
				return descriptorInfo;
			}


			/*
			* Function: flush
			*
			* Paramters: VkDeviceSize devSize
			*			 VkDeviceSize offset
			*
			* Return Type: VkResult
			*
			* Description: flushes the memory in the buffer,
			*				this must be done for the dynamic unifrom buffer's
			*				memory to made visible
			*
			*/
			VkResult flush(VkDeviceSize devSize = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
				VkMappedMemoryRange mappedRange = {};
				mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				mappedRange.memory = memory;
				mappedRange.offset = offset;
				mappedRange.size = devSize;
				return vkFlushMappedMemoryRanges(device, 1, &mappedRange);
			}

			/*
			* Function: map
			*
			* Paramters: VkDeviceSize devSize
			*			 VkDeviceSize offset
			*
			* Return Type: VkResult
			*
			* Description: maps the device memory of the buffer to the device
			*
			*/
			VkResult map(VkDeviceSize devSize = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
				return vkMapMemory(device, memory, offset, devSize, 0, &pMapped);
			}
			/*
			* Function: createBuffer
			*
			* Paramters: VkDeviceSize devSize,
			*			 VkBufferUsageFlags usage,
			*			 VkPhysicalDevice physclDev, VkMemoryPropertyFlags propFlags
			*
			* Return Type: VkResult
			*
			* Description: creates the buffer and its memory within the device
			*
			*/
			VkResult createBuffer(VkDeviceSize devSize, VkBufferUsageFlags usage, VkPhysicalDevice physclDev, VkMemoryPropertyFlags propFlags) throw(Exception) {
				try {
					if (initBuffer(devSize, usage)) {
						throw Exception("vkCreateBuffer function failed", "Buffer.h", "createBuffer");
					}
					if (initBufferMemory(physclDev, propFlags)) {
						throw Exception("failed to allocate buffer memory", "Buffer.h", "createBuffer");
					}
				}
				catch (Exception &excpt) {
					throw excpt;
				}
			}

		private:
			/*
			* Function: initBuffer
			*
			* Paramters: VkDeviceSize devSize,
			*			 VkBufferUsageFlags usage
			*
			* Return Type: VkResult
			*
			* Description: runs the vkCreateBuffer function
			*
			*/
			VkResult initBuffer(VkDeviceSize devSize, VkBufferUsageFlags usage) {
				usageFlags = usage;
				VkBufferCreateInfo info = vkAPI::InitStructs::BufferInfo(devSize, usage, VK_SHARING_MODE_EXCLUSIVE);

				return vkCreateBuffer(device, &info, nullptr, &buffer);
			}
			/*
			* Function: initBufferMemory
			*
			* Paramters: VkPhysicalDevice physclDev,
			*			 VkMemoryPropertyFlags propFlags
			*
			* Return Type: VkResult
			*
			* Description: allocates the buffer memory in the device
			*
			*/
			VkResult initBufferMemory(VkPhysicalDevice physclDev, VkMemoryPropertyFlags propFlags) {
				VkMemoryRequirements memoryReqs;
				vkGetBufferMemoryRequirements(device, buffer, &memoryReqs);

				size = memoryReqs.size;
				alignment = memoryReqs.alignment;
				memoryPropertyFlags = propFlags;

				VkMemoryAllocateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				info.allocationSize = memoryReqs.size;
				info.memoryTypeIndex = Utilities::findMemoryType(physclDev, memoryReqs.memoryTypeBits, propFlags);

				return vkAllocateMemory(device, &info, nullptr, &memory);
			}
		};
	}
}