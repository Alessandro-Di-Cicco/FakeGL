#include "Light.h"

Light::Light(Mesh& mesh, float intensity, ofColor col) : mesh{ mesh }, intensity{ intensity }, color{ col }{}

void Light::setIntensity(float val)
{
	intensity = val;
}

float Light::getIntensity()
{
	return intensity;
}

Mesh& Light::getMesh()
{
	return mesh;
}

ofColor Light::getColor()
{
	return color;
}

void Light::setColor(ofColor col)
{
	color = col;
}

