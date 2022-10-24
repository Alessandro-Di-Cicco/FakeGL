#include "Mesh.h"

#include <glm/ext/matrix_transform.hpp>

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<VertexData*>&& vertData) :
	position{}, scale{ 1 }, rotation{}, verts{ vertices }, vertexData{ vertData }, matrixDirty{ true } {}

Mesh::Mesh(Mesh&& other)
{
	verts = other.verts;
	vertexData = other.vertexData;
	position = other.position;
	scale = other.scale;
	rotation = other.rotation;
	matrixDirty = other.matrixDirty;
	matrix = other.matrix;

	// Remove the pointers from the original so that the vertex data is not deallocated other is destroyed
	other.vertexData.clear();
}


Mesh& Mesh::operator=(Mesh&& other)
{
	verts = other.verts;
	vertexData = other.vertexData;
	position = other.position;
	scale = other.scale;
	rotation = other.rotation;
	matrixDirty = other.matrixDirty;
	matrix = other.matrix;

	// Remove the pointers from the original so that the vertex data is not deallocated other is destroyed
	other.vertexData.clear();

	return *this;
}


void Mesh::render(Renderer& renderer)
{
	updateMatrix();

	// Iterate over every triangle, notice the += 3 increment
	for (int i = 0; i < verts.size(); i += 3)
	{
		// Set the global transform used by the shader
		renderer.getShader()->setUniform4fm("transform", matrix);

		// Pass the vertex positions and the triangle data to the renderer
		const std::vector<VertexData*> vData{ getTriangleData(i) };
		renderer.renderTriangle(verts.data() + i, vData);
	}
}

void Mesh::updateMatrix()
{
	if (!matrixDirty) return;

	matrix = glm::mat4(1);
	matrix = translate(matrix, position);

	matrix = rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	matrix = rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	matrix = rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));

	matrix = glm::scale(matrix, scale);

	matrixDirty = false;
}

std::vector<VertexData*> Mesh::getTriangleData(int index)
{
	std::vector<VertexData*> output{};
	output.reserve(3);

	for (int i = 0; i < 3; i++)
		/* The reason modulo is used is because it makes it possible to give fewer VertexData objects than there are vertices
		 * In that case, it wraps around for vertex indices that are >= vertexData.size()
		 * It's mostly for experimentation, as this would NEVER happen in a real use case, but it doesn't cost anything to
		 * leave it here for future tests!
		 */
		output.push_back(vertexData[(index + i) % vertexData.size()]);

	return output;
}

glm::vec3& Mesh::getPosition()
{
	return position;
}

glm::vec3& Mesh::getScale()
{
	return scale;
}

glm::vec3& Mesh::getRotation()
{
	return rotation;
}

void Mesh::setPosition(glm::vec3 pos)
{
	position = pos;
	matrixDirty = true;
}

void Mesh::setScale(glm::vec3 scl)
{
	scale = scl;
	matrixDirty = true;
}

void Mesh::setRotation(glm::vec3 rot)
{
	rotation = rot;
	matrixDirty = true;
}

/**
 * \brief Release all of the vertex data memory
 */
Mesh::~Mesh()
{
	for (int i = 0; i < vertexData.size(); i++)
		delete vertexData[i];
}

