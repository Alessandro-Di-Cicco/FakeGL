#include "CubeGen.h"

#include <glm/gtx/transform.hpp>

// The coordinate values used for the cube vertices, in object space
constexpr float MIN = -0.5f, MAX= -MIN;

// The vertices of the triangles of a cube mesh
std::vector<glm::vec3> cubeTriangles{
	// Front bottom-left triangle
	{MIN, MIN, MIN},
	{MIN, MAX, MIN},
	{MAX, MIN, MIN},

	// Front top-right triangle
	{MAX, MIN, MIN},
	{MAX, MAX, MIN},
	{MIN, MAX, MIN},

	// Back bottom-left triangle
	{MIN, MIN, MAX},
	{MIN, MAX, MAX},
	{MAX, MIN, MAX},

	// Back top-right triangle
	{MAX, MIN, MAX},
	{MAX, MAX, MAX},
	{MIN, MAX, MAX},

	// Right bottom-left triangle
	{MAX, MIN, MAX},
	{MAX, MAX, MAX},
	{MAX, MIN, MIN},

	// Right, top-right triangle
	{MAX, MAX, MAX},
	{MAX, MAX, MIN},
	{MAX, MIN, MIN},

	// Left bottom-right triangle
	{MIN, MIN, MAX},
	{MIN, MAX, MAX},
	{MIN, MIN, MIN},

	// Left, top-left triangle
	{MIN, MAX, MAX},
	{MIN, MAX, MIN},
	{MIN, MIN, MIN},

	// Top bottom-left triangle
	{MIN, MAX, MAX},
	{MIN, MAX, MIN},
	{MAX, MAX, MAX},

	// Top top-right triangle
	{MAX, MAX, MAX},
	{MAX, MAX, MIN},
	{MIN, MAX, MIN},

	// Bottom bottom-left triangle
	{MIN, MIN, MAX},
	{MIN, MIN, MIN},
	{MAX, MIN, MAX},

	// bottom top-right triangle
	{MAX, MIN, MAX},
	{MAX, MIN, MIN},
	{MIN, MIN, MIN},
};

constexpr float PYR_Y = 0.25;
std::vector<glm::vec3> pyramidTriangles{
	// Bottom base triangles
	{MIN, PYR_Y, MIN},
	{MAX, PYR_Y, MIN},
	{MAX, PYR_Y, MAX},

	{MIN, PYR_Y, MIN},
	{MIN, PYR_Y, MAX},
	{MAX, PYR_Y, MAX},

	// Front facing triangle
	{MIN, PYR_Y, MAX},
	{MAX, PYR_Y, MAX},
	{0, -PYR_Y, 0},

	// Back facing triangle
	{MIN, PYR_Y, MIN},
	{MAX, PYR_Y, MIN},
	{0, -PYR_Y, 0},

	// Left facing triangle
	{MIN, PYR_Y, MIN},
	{MIN, PYR_Y, MAX},
	{0, -PYR_Y, 0},

	// Right facing triangle
	{MAX, PYR_Y, MIN},
	{MAX, PYR_Y, MAX},
	{0, -PYR_Y, 0}
};


const float sinCos45 = glm::cos(glm::radians(45.0f));
std::vector<glm::vec3> pyramidNormals{
	{0, 1, 0},
	{0, 1, 0},
	{0, -sinCos45, sinCos45},
	{0, -sinCos45, -sinCos45},
	{-sinCos45, -sinCos45, 0},
	{sinCos45, -sinCos45, 0}
};

std::vector<VertexData*> genCubeVertexData(ofColor color)
{
	std::vector<glm::vec3> normals{
		{0, 0, -1},
		{0, 0, 1},
		{1, 0, 0},
		{-1, 0, 0},
		{0, 1, 0},
		{0, -1, 0}
	};

	std::vector<VertexData*> data{ };
	for (int i = 0; i < normals.size(); i++)
	{
		for (int j = 0; j < 6; j++) {
			// This allocates on dynamic memory, the mesh destructor takes care of this!
			auto posData = new PosVertexData(normals[i], color, {});
			data.push_back(posData);
		}
	}

	return data;
}

std::vector<VertexData*> genPyramidVertexData(ofColor color)
{
	std::vector<VertexData*> data{};

	for (auto normal : pyramidNormals)
	{
		for (int i = 0; i < 3; i++) {
			auto vertData = new PosVertexData(normal, color, {});
			data.push_back(vertData);
		}
	}

	return data;
}


std::vector<VertexData*> genOctaVertexData(ofColor color)
{
	std::vector<VertexData*> original{ genPyramidVertexData(color) };
	std::vector<VertexData*> data{ };

	data.insert(data.end(), original.begin() + 6, original.end());

	for (int i = 6; i < original.size(); i++)
	{
		const auto vData = original[i];

		glm::vec3 normal = vData->normal;
		normal.y *= -1;
		auto flip = new PosVertexData(normal, color, {});
		data.push_back(flip);
	}

	return data;
}


std::vector<glm::vec3> genOctaVertices()
{
	std::vector<glm::vec3> verts{};

	verts.insert(verts.end(), pyramidTriangles.begin() + 6, pyramidTriangles.end());
	verts.insert(verts.end(), pyramidTriangles.begin() + 6, pyramidTriangles.end());

	for (int i = 0; i < 12; i++)
		verts[i].y -= PYR_Y;

	for (int i = 12; i < verts.size(); i++)
	{
		verts[i].y -= PYR_Y;
		// This "flips" the pyramid around the y axis
		verts[i].y *= -1;
	}

	return verts;
}


Mesh generateCube(glm::vec3 position, glm::vec3 scale, ofColor color)
{
	Mesh m{ cubeTriangles, genCubeVertexData(color)};

	m.setPosition(position);
	m.setScale(scale);

	return m;
}

Mesh generatePyramid(glm::vec3 pos, glm::vec3 scale, ofColor color)
{
	Mesh m{ pyramidTriangles, genPyramidVertexData(color) };
	m.setPosition(pos);
	m.setScale(scale);

	return m;
}

Mesh generateOctahed(glm::vec3 pos, glm::vec3 scale, ofColor color)
{
	Mesh m{ genOctaVertices(), genOctaVertexData(color) };

	m.setPosition(pos);
	m.setScale(scale);

	return m;
}

