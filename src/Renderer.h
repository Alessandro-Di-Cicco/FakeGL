#pragma once
#include <glm/vec3.hpp>

#include "DepthBuffer.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ShaderProgram.h"

/**
 * \brief Does all of the heavy lifting, draws funny shapes inside a window!
 */
class Renderer {
public:
	Renderer(int width = 160, int height = 200);

	/**
	 * \brief Renders a triangle on screen
	 * \param tri an array of three vertex positions
	 */
	void renderTriangle(const glm::vec3* tri, std::vector<VertexData*> data);
	/**
	 * \brief Clears the screen buffer and the depth buffer
	 */
	void clearBuffers();
	/**
	 * \param shader The shader to use when rendering triangles
	 */
	void setShader(ShaderProgram* shader);
	ShaderProgram* getShader();

	/**
	 * \brief Sets the color used by calls of clearBuffers() to clear the render texture
	 */
	void setClearColor(ofColor col);

	/**
	 * \return the render texture 
	 */
	ofImage	getTexture() const;

private:
	int TexWidth;
	int TexHeight;
	// The structure used internally to draw. It's the internal "framebuffer"
	ofPixels_<unsigned char> pix;
	DepthBuffer depthBuffer;
	ShaderProgram * shader;
	ofColor clearColor;

	// The most important function of the whole project, performs all of the computations required to draw on screen
	void processTriangle(glm::vec3* triangle, int minX, int minY, int maxX, int maxY, float texWidth, float texHeight, std::vector<VertexData*> data);
};
