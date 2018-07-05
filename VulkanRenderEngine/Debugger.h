/*
* TITLE:	Vulkan Render Engine
* BY:		Eric Hollas	
*	
* FILE:		Debugger.h
* DETAILS:	This file adds static methods to the Utilities namespace. The functions
*				in this file relate to the debug report for the RenderEngine object.
*/
#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Exception.h"

namespace Utilities {
	namespace Debug {
		/*
		* Function: debugCallback
		*
		*
		* Description: this is the callback for the debug report
		*
		*/
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags,
															VkDebugReportObjectTypeEXT objType,
															uint64_t obj,
															size_t location,
															int32_t code,
															const char* layerPrefix,
															const char* msg,
															void* userData) {
			std::cerr << "validation layer: " << msg << std::endl;

			return VK_FALSE;
		}
		/*
		* Function: setupDebugCallback
		*
		*
		* Description: this sets up the callback function for the debug report
		*
		*/
		static void setupDebugCallback(VkInstance inst, VkDebugReportCallbackEXT* pCallback) throw(Exception) {
			try {
				if (!isDebugging) return;

				VkDebugReportCallbackCreateInfoEXT createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
				createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
				createInfo.pfnCallback = debugCallback;

				VkResult result;
				auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(inst, "vkCreateDebugReportCallbackEXT");
				if (func != nullptr) {
					result = func(inst, &createInfo, nullptr, pCallback);
				}
				else {
					result = VK_ERROR_EXTENSION_NOT_PRESENT;
				}

				if (result != VK_SUCCESS) {
					throw Exception("failed to set up debug callback", "Debugger.h", "setupDebugCallback");
				}
			}
			catch (Exception &excpt) {
				throw excpt;
			}
		}
	}
}