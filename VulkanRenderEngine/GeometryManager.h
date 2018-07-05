/*
* TITLE:	VulkanRenderEngine
* BY:		Eric Hollas
*
* FILE:		GeometryManager.h
* DETAILS:	This file defines the Geometry Manager object. It will be used to synch	
*				vertex data and index data in the render engine.
*				Since the Render Engine uses a dynamic uniform buffer and single
*				index and vertex buffers, this manager will be used in the command	
*				buffer, index buffer, and vertex buffer functions in the render
*				engine.	It will also manage loading and deleting the vertex and index
*				data in the CharacterManager object as well.
*/
#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <array>

namespace Geometry {
	enum color {
		white,
		red,
		darkRed,
		brown,
		darkBrown,
		green,
		greenGrass,
		blue,
		navy,
		magenta,
		yellow,
		orange,
		teal,
		black,
		darkGray,
		gray
	};
	inline glm::vec3 getColor(color cl) {
		const std::vector<glm::vec3> color_pallette = {
			{ 1.0f, 1.0f, 1.0f }, //white
			{ 1.0f, 0.0f, 0.0f }, //red
			{ 0.6f, 0.0f, 0.0f }, //darkRed
			{ 0.5f, 0.3f, 0.0f }, //brown
			{ 0.3f, 0.2f, 0.0f }, //darkBrown
			{ 0.0f, 1.0f, 0.0f }, //green
			{ 0.0f, 0.4f, 0.1f }, //greenGrass
			{ 0.0f, 0.0f, 1.0f }, //blue
			{ 0.0f, 0.0f, 0.4f }, //navy
			{ 1.0f, 0.0f, 1.0f }, //magenta
			{ 1.0f, 1.0f, 0.0f }, //yellow
			{ 0.8f, 0.4f, 0.0f }, //orange
			{ 0.0f, 1.0f, 1.0f }, //teal
			{ 0.0f, 0.0f, 0.0f }, //black
			{ 0.2f, 0.2f, 0.2f }, //darkGray
			{ 0.6f, 0.6f, 0.6f }  //gray
		};
		return color_pallette[cl];
	}
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;

		/*
		* Function: getBindingDescription
		*
		* Paramters: none
		*
		* Return Type: VkVertexInputBindingDescription
		*
		* Description: returns the struct for the vertex bindings to be used
		*				for the vertex buffer in the descriptor layout
		*
		*/
		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		/*
		* Function: getAttributeDescriptions
		*
		* Paramters: none
		*
		* Return Type: std::array<VkVertexInputAttributeDescription, 2>
		*
		* Description: returns the binding descriptions of the vertex
		*				buffer for the descriptor layout
		*
		*/
		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			return attributeDescriptions;
		}
	};

	struct DynamicUniformBufferObject {
		glm::mat4 * pModel;
	};


	class GeometryManager {
	public:
		GeometryManager() {
		}
		~GeometryManager() {
			geometryInfo.~vector();
		}

		/*
		* Function: addObject
		*
		* Paramters: std::vector<Geometry::Vertex> vertices
		*			 std::vector<uint32_t> indices
		*
		* Return Type: void
		*
		* Description: takes the parameters to calc and add the offsets to the class member variables
		*				it also accumulates the vertices and indices into class memeber variables
		*
		*/
		void addObject(std::vector<Geometry::Vertex> vertices, std::vector<uint32_t> indices) {
			offset temp;

			if (geometryInfo.empty()) {
				temp.offset_index = 0;
				temp.offset_vertex = 0;
				temp.indexBuffer = indices;
				temp.vertexBuffer = vertices;
			}
			else {
				temp.offset_index = geometryInfo.back().offset_index + geometryInfo.back().indexBuffer.size();
				temp.offset_vertex = geometryInfo.back().offset_vertex + geometryInfo.back().vertexBuffer.size();
				temp.indexBuffer = indices;
				temp.vertexBuffer = vertices;
			}

			geometryInfo.push_back(temp);
		}
		/*
		* Function: deleteLast
		*
		* Paramters: none
		*
		* Return Type: void
		*
		* Description: deletes all data associated with the last character from every class memeber variable
		*
		*/
		void deleteLast() {
			geometryInfo.pop_back();
		}
		/*
		* Function: updateObject
		*
		* Paramters: uint32_t object,
		*			 std::vector<Vertex> vertices,
		*			 std::vector<uint32_t> indices
		*
		* Return Type: void
		*
		* Description: takes the object variable and updates that objects geometric data to be the other
		*				parameters that were passed. Then updates the offset data for the objects that
		*				come after that object in the vector of objects. Order must be maintained to match
		*				the order of the uniform matrices, which are not tracked here.
		*
		*/
		void updateObject(uint32_t object, std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
			if (object < geometryInfo.size()) {
				geometryInfo[object].indexBuffer.clear();
				geometryInfo[object].vertexBuffer.clear();
				geometryInfo[object].indexBuffer = indices;
				geometryInfo[object].vertexBuffer = vertices;

				while (++object < geometryInfo.size()) {
					geometryInfo[object].offset_index = geometryInfo[object - 1].indexBuffer.size() + geometryInfo[object - 1].offset_index;
					geometryInfo[object].offset_vertex = geometryInfo[object - 1].vertexBuffer.size() + geometryInfo[object - 1].offset_vertex;
				}
			}
		}

		/*
		* The following 8 functions are generic accessor methods, but they do compile their necessary
		*		before returning the data if need be.
		*
		*/
		uint32_t getNumOfObjects() const {
			return geometryInfo.size();
		}
		uint32_t getVertexOffset(int object) const {
			if (object > geometryInfo.size()) {
				return 0;
			}
			return geometryInfo[object].offset_vertex;
		}
		uint32_t getIndexOffset(int object) const {
			if (object > geometryInfo.size()) {
				return 0;
			}
			return geometryInfo[object].offset_index;
		}
		uint32_t getIndiciesInObject(int object) const {
			if (object > geometryInfo.size()) {
				return 0;
			}
			return geometryInfo[object].indexBuffer.size();
		}

		uint32_t getTotalVertices() const {
			return (geometryInfo.back().offset_vertex + geometryInfo.back().vertexBuffer.size());
		}
		uint32_t getTotalIndices() const {
			return (geometryInfo.back().offset_index + geometryInfo.back().indexBuffer.size());
		}

		std::vector<uint32_t> getIndicies() const {
			std::vector<uint32_t> indices;
			std::vector<offset>::const_iterator it = geometryInfo.begin();

			while (it != geometryInfo.end()) {
				offset temp = *it;
				indices.insert(indices.end(), temp.indexBuffer.begin(), temp.indexBuffer.end());
				it++;
			}

			return indices;
		}
		std::vector<Geometry::Vertex> getVertices() const {
			std::vector<Vertex> vertices;
			std::vector<offset>::const_iterator it = geometryInfo.begin();

			while (it != geometryInfo.end()) {
				offset temp = *it;
				vertices.insert(vertices.end(), temp.vertexBuffer.begin(), temp.vertexBuffer.end());
				it++;
			}

			return vertices;
		}

	private:
		struct offset {
			std::vector<Vertex> vertexBuffer;
			std::vector<uint32_t> indexBuffer;

			uint32_t offset_index;
			uint32_t offset_vertex;
		};

		std::vector<offset> geometryInfo;

	};
}