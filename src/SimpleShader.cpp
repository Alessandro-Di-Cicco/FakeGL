#include "SimpleShader.h"

#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "ofColor.h"
#include "ofMath.h"
#include "ofUtils.h"

SimpleShader::SimpleShader(glm::mat4 persp, bool lit)
	: lit{ lit }, perspective{ persp }, lights{} {}

void SimpleShader::setPersp(glm::mat4 persp) { perspective = persp; }


void SimpleShader::addLight(Light& light)
{
	lights.push_back(light);
}


void SimpleShader::removeLight(Light& light)
{
	for (int i = 0; i < lights.size(); i++)
	{
		if (&light == &(lights[i].get()))
		{
			lights.erase(lights.begin() + i);
			return;
		}
	}
}

/*
 * Simply applies world-space transform + perspective and stores the obtained position in the
 * globalPos variable of the vertex data
 */
glm::vec4 SimpleShader::runVertexShader(glm::vec3 vertPos, VertexData* data)
{
	const glm::vec4 vert{ vertPos.x, vertPos.y, vertPos.z, 1 };

	// Uses a custom subtype of VertexData that allows to store local and global vertex position
	PosVertexData* posData;
	if ((posData = dynamic_cast<PosVertexData*>(data)) == nullptr)
	{
		std::cerr << "Error, simple shader received object that is not PosVertexData";
		throw std::bad_function_call();
	}

	posData->localPos = vertPos;
	posData->globalPos = transform * vert;
	return perspective * view * posData->globalPos;
}

ofColor SimpleShader::runFragmentShader(glm::vec3 barycentric, std::vector<VertexData*> enclosingVerts)
{
	PosVertexData* posData[3] {};

	// Make sure the vertex data is of the appropriate subtype, fail if it isn't
	for (int i = 0; i < 3; i++)
	{
		if ((posData[i] = dynamic_cast<PosVertexData*>(enclosingVerts[i])) == nullptr)
		{
			std::cerr << "Error, simple shader received object that is not PosVertexData";
			throw std::bad_function_call();
		}
	}

	const ofColor baseColor{getColor(barycentric, posData)};
	ofColor finalColor{ baseColor };

	if (lit) {
		// Base light pass
		constexpr float ambient = 0.01f;
		finalColor *= ambient;

		// Compute and add together every diffuse color from every registered light
		for (int i = 0; i < lights.size(); i++) {
			const ofColor diffuse = getDiffuse(barycentric, posData, baseColor, lights[i].get().getMesh().getPosition(),
			                                   lights[i].get().getIntensity(), lights[i].get().getColor());
			// Note: ofColor operator+() doesn't add up alpha values, it keeps the alpha of the first operand and clamps the values to prevent overflow
			// This also means that, with this formula, lights are additive and not multiplicative. It should be relatively easy to change it
			finalColor += diffuse * 0.5f;
		}
	}

	finalColor.a = 255;

	return finalColor;
}

void SimpleShader::setUniform4fm(std::string name, glm::mat4 matrix)
{
	if (name == "transform") {
		transform = matrix;
		normalTransform = transpose(inverse(transform));
	}

	else if (name == "view")
		view = matrix;
}

/**
 * \brief Computes the color of a fragment by averaging the color of the three containing vertices
 * \param barycentric the barycentric coordinates of the fragment
 * \param enclosingVerts the data of the vertices of the enclosing triangle
 * \return the material color of the fragment
 */
ofColor SimpleShader::getColor(glm::vec3 barycentric, PosVertexData** enclosingVerts)
{
	ofColor color{0, 0, 0, 0};

	for (int i = 0; i < 3; i++)
		color += enclosingVerts[i]->color * barycentric[i];

	return color;
}

/**
 * \brief Compute the diffuse color of the fragment relatively to the given light
 * \param barycentric barycentric coordinates of the fragment within the containing triangle
 * \param enclosingVerts the vertex data of the triangle that contains the fragment
 * \param fragColor the base color of the fragment
 * \param lightPos the position of the light in world space
 * \param lightStrength the strength of the light
 * \return the processed color
 */
ofColor SimpleShader::getDiffuse(const glm::vec3& barycentric, PosVertexData** enclosingVerts, ofColor fragColor,
                                 const glm::vec3& lightPos, const float& lightStrength, const ofColor& lightColor)
{
	static ofColor zero{0, 0, 0, 0};

	// Compute fragment world space pos and normal from the enclosing vertices
	glm::vec4 fragPos{};
	glm::vec3 normal{};
	for (int i = 0; i < 3; i++) {
		fragPos += enclosingVerts[i]->globalPos * barycentric[i];
		normal += enclosingVerts[i]->normal * barycentric[i];
	}

	normal = normalize(normal);
	glm::vec4 temp = { normal.x, normal.y, normal.z, 0 };
	// Apply the rotation to the normal for proper lighting when the mesh is rotated
	// The normal transform doesn't encode translations/scales, keeps it nice and working!
	normal = normalTransform * temp;

	const glm::vec3 spacePos{ fragPos };
	const glm::vec3 lightOffset{ lightPos - spacePos };
	const glm::vec3 lightDir{ normalize(lightOffset) };

	// Instead of clamping the dot in the range [0, 1], return when <= 0 to
	// save some precious CPU time
	const float dotP = dot(lightDir, normal);
	if (dotP <= 0) return zero;

	// Apply the inverse square law to account for the distance
	const float dst = pow(length(lightOffset), 0.5f);

	// Put it all together
	return fragColor * lightColor * dotP * (1 / (dst + 0.0001f)) * lightStrength;
}