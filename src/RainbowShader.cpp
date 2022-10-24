#include "RainbowShader.h"

#include "ofColor.h"

RainbowShader::RainbowShader(glm::mat4 persp, bool lit)
	: SimpleShader(persp, lit) {}


void RainbowShader::setUniform1fv(std::string name, float value)
{
	SimpleShader::setUniform1fv(name, value);

	if (name == "time")
		time = value;
	else if (name == "frequency")
		freq = value;
}


ofColor RainbowShader::getColor(glm::vec3 barycentric, PosVertexData** enclosingVerts)
{
	glm::vec3 fragPos{};

	for (int i = 0; i < 3; i++)
		fragPos += enclosingVerts[i]->localPos * barycentric[i];

	ofColor col{};
	col.setHsb(remainderf(length(fragPos * freq) + time, 255.0f), 255, 255);

	return col;
}
