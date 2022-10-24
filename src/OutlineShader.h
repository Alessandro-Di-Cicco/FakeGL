#pragma once
#include "SimpleShader.h"

class OutlineShader: public SimpleShader
{
public:
	OutlineShader(const glm::mat4& persp, bool lit, ofColor outlineColor);

	void setUniform1fv(std::string name, float value) override;
	ofColor getColor(glm::vec3 barycentric, PosVertexData** enclosingVerts) override;

private:
	ofColor outline;
	float sinTime;
	float minThickness;
	float maxThickness;

	float usedThickness;
};
