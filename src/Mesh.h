#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "Renderer.h"
#include "VertexData.h"

/**
 * \brief A renderable mesh with configurable position, scale and rotation
 */
class Mesh
{
public:
	/**
	 * \brief Initialize this mesh
	 * \param vertices the vertices composing the mesh, interpreted as triangles
	 * \param vertexData the data bound to the vertices. If this vector has fewer elements than vertices, the data is repeated
	 * by wrapping around the vector for vertices that exceed it's size
	 * the data bound to the three vertices of composing the requested triangle, as a vector of three VertexData objects
	 */
	Mesh(std::vector<glm::vec3> vertices, std::vector<VertexData*>&& vertexData);
	Mesh(const Mesh& other) = default;

	Mesh& operator= (const Mesh& other) = default;

	Mesh(Mesh&& other);
	Mesh& operator= (Mesh&& other);

	/**
	 * \brief Draw the mesh on the screen
	 * \param renderer the used renderer
	 */
	void render(Renderer& renderer);

	void setPosition(glm::vec3 pos);
	void setScale(glm::vec3 scl);
	void setRotation(glm::vec3 rot);

	glm::vec3& getPosition();
	glm::vec3& getScale();
	glm::vec3& getRotation();

	~Mesh();
private:
	// Vertex data of the mesh
	std::vector<glm::vec3> verts;

	// The data bound to the mesh' vertices
	std::vector<VertexData*> vertexData;

	glm::vec3 position{};
	glm::vec3 scale{};
	glm::vec3 rotation{};

	bool matrixDirty;
	glm::mat4 matrix{};

	/**
	 * \brief When matrixDirty is set to true, compute the transform matrix and set matrixDirty to false
	 */
	void updateMatrix();

	/**
	 * \param firstVertIndex the index of the first vertex of the triangle (i.e. index % 3 == 0)
	 * \return the data of the vertices composing the triangle whose first vertex has the given index
	 */
	 std::vector<VertexData*> getTriangleData(int firstVertIndex);
};
