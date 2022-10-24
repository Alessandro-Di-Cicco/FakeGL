#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "VertexData.h"

/**
 * \brief Generic shader program used by a Renderer object to draw Mesh objects
 */
class ShaderProgram
{
public:
	/**
	 * \brief Runs the vertex shader on the given vertex
	 * \param vertexPos The position of the vertex
	 * \param vertexData the data bound to the vertex
	 * \return The final position of the vector, including the perspective divider w
	 */
	virtual glm::vec4 runVertexShader(glm::vec3 vertexPos, VertexData* vertexData)
	{
		return { vertexPos.x, vertexPos.y, vertexPos.z, 1 };
	}

	/**
	 * \brief runs the fragment shader on a fragment with the given barycentric coords and the surrounding vertices
	 * \param barycentric The barycentric coordinates of the triangle within its triangle
	 * \param enclosingVertices the data of the three vertices containing the fragment
	 * \return the color of the fragment
	 */
	virtual ofColor runFragmentShader(glm::vec3 barycentric, std::vector<VertexData*> enclosingVertices)
	{
		return {255, 255, 255, 255};
	}

	// Variable-setting functions
	virtual void setUniform4fm(std::string name, glm::mat4 mat) {}
	virtual void setUniform3fv(std::string name, glm::vec3 vec) {}
	virtual void setUniform1fv(std::string name, float value) {}
	virtual void setUniformVec1fv(std::string name, std::vector<float>& vec) {}
	virtual void setUniformVec3fv(std::string name, std::vector<glm::vec3>& vec) {}

	virtual ~ShaderProgram() = default;
};
