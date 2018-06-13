/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		GeometryManager.h															*
* DETAILS:	This file defines the Geometry Manager object. It will be used to synch		*
*				vertex data and index data in the render engine.						*
*				Since the Render Engine uses a dynamic uniform buffer and single		*
*				index and vertex buffers, this manager will be used in the command		*
*				buffer, index buffer, and vertex buffer functions in the render			*
*				engine.	It will also manage loading and deleting the vertex and index	*	
*				data in the CharacterManager object as well.							*
*																						*
*****************************************************************************************/

#pragma once
#include "stdafx.h"

namespace Geometry {
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
			vertexOffsets.~vector();
			indexOffsets.~vector();
			indicesPerObject.~vector();
			indexBuffer.~vector();
			vertexBuffer.~vector();
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
			indicesPerObject.push_back((indices.size()));
			indexOffsets.push_back((indexBuffer.size()));

			vertexOffsets.push_back(vertexBuffer.size());

			vertexBuffer.insert(vertexBuffer.end(), vertices.begin(), vertices.end());
			indexBuffer.insert(indexBuffer.end(), indices.begin(), indices.end());

			numOfObjects++;
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
			//local variables are used to run the for loops the correct number of times
			uint32_t numVertices = vertexOffsets.back();
			uint32_t numIndices = indicesPerObject.back();

			//deletes the offsets
			vertexOffsets.pop_back();
			indexOffsets.pop_back();
			indicesPerObject.pop_back();

			//deletes the mesh data
			for (uint32_t i = vertexBuffer.size(); i > numVertices; i--) {
				vertexBuffer.pop_back();
			}
			for (uint32_t i = 0; i < numIndices; i++) {
				indexBuffer.pop_back();
			}

			numOfObjects--;
		}

		/*
		* The rest of the functions are generic accessor methods
		*
		*/
		uint32_t getNumOfObjects() const {
			return numOfObjects;
		}
		uint32_t getVertexOffset(int object) const {
			return vertexOffsets[object];
		}
		uint32_t getIndexOffset(int object) const {
			return indexOffsets[object];
		}
		uint32_t getIndiciesInObject(int object) const {
			return indicesPerObject[object];
		}

		uint32_t getTotalVertices() const {
			return vertexBuffer.size();
		}
		uint32_t getTotalIndices() const {
			return indexBuffer.size();
		}

		std::vector<uint32_t> getIndicies() const {
			return indexBuffer;
		}
		std::vector<Geometry::Vertex> getVertices() const {
			return vertexBuffer;
		}

	private:
		uint32_t numOfObjects = 0;

		std::vector<uint32_t> vertexOffsets;
		std::vector<uint32_t> indexOffsets;
		std::vector<uint32_t> indicesPerObject;

		std::vector<uint32_t> indexBuffer;
		std::vector<Geometry::Vertex> vertexBuffer;

	};
}
