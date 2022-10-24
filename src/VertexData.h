#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

/**
 * \brief Stores basic data of a vertex
 */
class VertexData
{
public:
	glm::vec3 normal;
	ofColor color;

	VertexData(glm::vec3 norm, ofColor col) : normal{ norm }, color{ col } {}

	virtual ~VertexData() = default;
};

/**
 * \brief Stores basic + positional data of a vertex
 */
class PosVertexData : public VertexData
{
public:
	glm::vec3 localPos;
	glm::vec4 globalPos;

	PosVertexData(glm::vec3 norm, ofColor col, glm::vec3 locPos) : VertexData(norm, col), localPos{ locPos }, globalPos{} {}
};