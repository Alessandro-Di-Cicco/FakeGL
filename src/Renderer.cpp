#include "Renderer.h"

#include "ofImage.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

float triangleArea(glm::vec3* tri);
glm::vec3 getBarycentric(glm::vec3* tri, glm::vec3 pt);
glm::vec4 getTriangleBounds(glm::vec2* triangle, int texWidth, int texHeight);

Renderer::Renderer(const int width, const int height) : TexWidth { width }, TexHeight{ height }, depthBuffer{ width, height }, shader{ nullptr }, clearColor{ 255, 255 }
{
	pix.allocate(width,  height, 4);
}

void Renderer::clearBuffers()
{
	depthBuffer.clear(1000);
	pix.setColor(clearColor);
}

void Renderer::setShader(ShaderProgram* s)
{
	shader = s;
}

ShaderProgram* Renderer::getShader()
{
	return shader;
}

void Renderer::setClearColor(ofColor col)
{
	clearColor = col;
}

ofImage Renderer::getTexture() const
{
	ofImage img{ pix };
	// This sets the upscaling filter to linear, to avoid blurring when the window's resolution is greater
	// than the "renderbuffer" resolution
	img.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);
	return img;
}


void Renderer::renderTriangle(const glm::vec3* tri, std::vector<VertexData*> data)
{
	if (shader == nullptr) return;

	// Get the screen coordinates of the triangle
	glm::vec4 processedVerts[] = {
		shader->runVertexShader(tri[0], data[0]),
		shader->runVertexShader(tri[1], data[1]),
		shader->runVertexShader(tri[2], data[2])
	};

	// Don't process triangles with at least one vertex whose z coordinate is outside of [0, 100]
	if (std::any_of(processedVerts, processedVerts + 3, [](glm::vec3 vert) {return vert.z <= 0 || vert.z >= 100; }))
		return;

	// Perspective division
	for (auto& vert : processedVerts)
	{
		if (vert.w != 0)
		{
			vert.x /= vert.w;
			vert.y /= vert.w;
			vert.z /= vert.w;
		}
	}

	// Remove the z and w coordinates
	glm::vec2 screenCoords[] = {
		processedVerts[0],
		processedVerts[1],
		processedVerts[2]
	};

	// Get the smallest rectangle containing the entirety of the triangle (clamped in the range [0, width] for the x values and [0, height] for the y values)
	glm::vec4 bounds = getTriangleBounds(screenCoords, TexWidth, TexHeight);

	// Get rid of the w component, not needed anymore
	glm::vec3 fragmentTriangle[] = {
		processedVerts[0],
		processedVerts[1],
		processedVerts[2]
	};

	// Draw it!
	processTriangle(fragmentTriangle, bounds[0], bounds[1], bounds[2], bounds[3], TexWidth, TexHeight, data);
}

/**
 * \brief Computes the bounds of a given triangle
 * \param triangle the triangle, expects three glm::vec2
 * \return a glm::vec4 with the structure {minX, minY, maxX, maxY}
 */
glm::vec4 getTriangleBounds(glm::vec2* triangle, int texWidth, int texHeight)
{
	glm::vec4 out{};
	for (int i = 0; i < 3; i++)
	{
		out[0] = std::min(triangle[i].x, out[0]);
		out[1] = std::min(triangle[i].y, out[1]);

		out[2] = std::max(triangle[i].x, out[2]);
		out[3] = std::max(triangle[i].y, out[3]);
	}

	out[0] += 1;
	out[0] *= texWidth / 2.0f;

	out[1] += 1;
	out[1] *= texHeight / 2.0f;

	out[2] += 1;
	out[2] *= texWidth / 2.0f;

	out[3] += 1;
	out[3] *= texHeight / 2.0f;

	// Cut away anything that goes outside of the screen
	out[0] = ofClamp(out[0], 0, texWidth - 1);
	out[2] = ofClamp(out[2], 0, texWidth - 1);
	out[1] = ofClamp(out[1], 0, texHeight - 1);
	out[3] = ofClamp(out[3], 0, texHeight - 1);

	return out;
}

void Renderer::processTriangle(glm::vec3* triangle, int minX, int minY, int maxX, int maxY, float texWidth, float texHeight, std::vector<VertexData*> data)
{
	// Sets z to 0
	glm::vec3 screenTri[] = {
		{triangle[0].x, triangle[0].y, 0},
		{triangle[1].x, triangle[1].y, 0},
		{triangle[2].x, triangle[2].y, 0}
	};

	// Iterate over all the pixel coordinates of the bounds
	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			// Map the x and y in the range [-1, 1]
			const float ndcX = x / texWidth * 2 - 1;
			const float ndcY = y / texHeight * 2 - 1;

			// Get the barycentric coordinates of the pixel ndc inside of the triangle
			glm::vec3 barycentric = getBarycentric(screenTri, glm::vec3(ndcX, ndcY, 0));

			// When one of the barycentric is < 0, it means that the given point is out of the triangle, skip!
			if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0) {
				// Obtain the z-value of the fragment by interpolating the z of the vertices
				float zVal = 0;
				for (int i = 0; i < 3; i++)
					zVal += triangle[i].z * barycentric[i];

				// depth-testing, draw only if the current z is greater than the written one
				if (depthBuffer.get(x, y) <= zVal) continue;

				// Get the fragment's color
				ofColor col = shader->runFragmentShader(barycentric, data);

				// Draw!
				pix.setColor(x, y, col);

				// Update depth buffer
				depthBuffer.set(x, y, zVal);
			}
		}
	}
}

/**
 * \brief A utility function to calculate area of a triangle formed by (x1, y1), (x2, y2) and (x3, y3)
 */
float triangleArea(glm::vec3* tri)
{
	const glm::vec3 ab = tri[1] - tri[0];
	const glm::vec3 ac = tri[2] - tri[0];

	return dot(ab, ac) / 2;
}

/**
 * \brief Compute the barycentric coordinates of a point relatively to a given triangle
 * \param t the triangle
 * \param p the point
 */
glm::vec3 getBarycentric(glm::vec3* t, glm::vec3 p)
{
	const float denominator = (t[1].y - t[2].y) * (t[0].x - t[2].x) + (t[2].x - t[1].x) * (t[0].y - t[2].y);

	float wa = ((t[1].y - t[2].y) * (p.x - t[2].x) + (t[2].x - t[1].x) * (p.y - t[2].y)) / denominator;

	float wb = ((t[2].y - t[0].y) * (p.x - t[2].x) + (t[0].x - t[2].x) * (p.y - t[2].y)) / denominator;

	return { wa, wb, 1 - wa - wb };
}