#pragma once
#include "SimpleShader.h"

/*
 * This is a perfect example of the limitations of CPU-based rendering.
 * This shader is a bit more complex than the base lit shader, and it does
 * make a difference in rendering times.
 * Obviously, this doesn't matter since this is just a proof of concept project,
 * to study one possible approach to building a rendering pipeline
 */

/**
 * \brief Variation of the lit shader, applies an animated color to the fragments
 * the animation depends on the value of the "time" float uniform
 */
class RainbowShader : public SimpleShader
{
public:
	RainbowShader(glm::mat4 persp, bool lit = true);
	void setUniform1fv(std::string name, float value) override;

protected:
	ofColor getColor(glm::vec3 barycentric, PosVertexData** enclosingVerts) override;

private:
	float time{};
	float freq{};
};
