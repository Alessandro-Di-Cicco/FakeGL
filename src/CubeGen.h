#pragma once
#include "Mesh.h"

/**
 * \brief Generate a cube mesh with the given settings
 * \param position the position of the cube, in world space
 * \param color the cube's color
 * \return the requested cube mesh
 */
Mesh generateCube(glm::vec3 position, glm::vec3 scale, ofColor color);

/**
 * \brief Generate a pyramid mesh with the given 
 */
Mesh generatePyramid(glm::vec3 pos, glm::vec3 scale, ofColor color);

Mesh generateOctahed(glm::vec3 pos, glm::vec3 scale, ofColor color);