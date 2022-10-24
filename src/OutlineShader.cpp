#include "OutlineShader.h"

OutlineShader::OutlineShader(const glm::mat4& persp, bool lit, ofColor outlineColor): SimpleShader(persp, lit),
	outline{outlineColor}
{}

void OutlineShader::setUniform1fv(std::string name, float value)
{
	if (name == "sinTime") {
		sinTime = value;

		const float normalized = (value + 1) / 2;

		const float delta = maxThickness - minThickness;
		usedThickness = minThickness + delta * normalized;
	}
	else if (name == "minThickness")
		minThickness = value;
	else if (name == "maxThickness")
		maxThickness = value;
}

ofColor OutlineShader::getColor(glm::vec3 barycentric, PosVertexData** enclosingVerts)
{
	// return SimpleShader::getColor(barycentric, enclosingVerts);

	glm::vec3 localPos{0};

	for (int i = 0; i < 3; i++)
		localPos += enclosingVerts[i]->localPos * barycentric[i];

	int matchCounter = 0;

	// Has to match two coordinates being above the threshold
	for (int i = 0; i < 3; i++) {
		if (fabs(localPos[i]) > usedThickness)
			if (matchCounter == 0)
				matchCounter += 1;
			else return outline;
	}

	// Rollback to default behaviour if the fragment is not inside the outline
	return SimpleShader::getColor(barycentric, enclosingVerts);
}
