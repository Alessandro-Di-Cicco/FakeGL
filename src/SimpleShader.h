#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>

#include "Light.h"
#include "ofColor.h"
#include "ShaderProgram.h"

/**
 * \brief Represents a lit shader (i.e. a shader that supports the computation of diffuse lighting)
 */
class SimpleShader: public ShaderProgram
{
public:
	SimpleShader(glm::mat4 persp, bool lit = true);

	glm::vec4 runVertexShader(glm::vec3 vertexPos, VertexData* vertexData) override;
	ofColor runFragmentShader(glm::vec3 barycentric, std::vector<VertexData*> containingVertices) override;
	void setUniform4fm(std::string name, glm::mat4 matrix) override;

	/**
	 * \brief Track a new light. Uses references to allow updating the position of a light without having to remove it and re-insert it
	 */
	void addLight(Light& light);

	/**
	 * \brief Remove a light from the rendering calculations
	 * not going to be used anymore, otherwise nothing happens
	 */
	void removeLight(Light& light);

	/**
	 * \brief Set the perspective matrix used in rendering
	 * \param persp the matrix
	 */
	void setPersp(glm::mat4 persp);

	/**
	 * \brief Set to true to perform lighting calculations, false turns this into an unlit shader
	 */
	bool lit;

protected:
	/**
	 * \brief Get the color of a fragment given the data of its enclosing vertices and its barycentric coordinates
	 * within said triangle
	 * \param barycentric the barycentric coordinates
	 * \param enclosingVerts the enclosing vertices' data
	 * \return the color of the fragment
	 */
	virtual ofColor getColor(glm::vec3 barycentric, PosVertexData** enclosingVerts);

private:
	/**
	 * \brief get the diffuse color of the given fragment
	 * \param barycentric the barycentric coordinates of the fragment within it's 3 enclosing vertices
	 * \param enclosingVerts the vertices of the triangle containing the fragment
	 * \param color the base color of the fragment
	 * \param lightPos the position of the light to use
	 * \param lightStrength the strength of the light to use
	 * \return the diffuse color of the fragment
	 */
	ofColor getDiffuse(const glm::vec3& barycentric, PosVertexData** enclosingVerts, ofColor color, const glm::vec3& lightPos, const float& lightStrength, const ofColor& lightColor);

	glm::mat4 perspective;
	glm::mat4 view;

	// Multiple lights are supported. The fragment shader stage just iterates over all of them and computes the diffuse for each one
	std::vector<std::reference_wrapper<Light>> lights;

	// Uniforms
	glm::mat4 transform;
	glm::mat4 normalTransform;
};
