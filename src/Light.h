#pragma once
#include "Mesh.h"

class Light
{
public:
	Light(Mesh& mesh, float intensity, ofColor col = {});

	void setIntensity(float val);
	Mesh& getMesh();
	float getIntensity();
	ofColor getColor();
	void setColor(ofColor col);

private:
	Mesh& mesh;
	float intensity;
	ofColor color;
};
