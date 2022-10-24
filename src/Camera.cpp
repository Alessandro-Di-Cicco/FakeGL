#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>

#include "ofAppRunner.h"

Camera::Camera(glm::vec3 pos): position{pos}, forward { 0, 0, 1 }, right{}
{
	updateMatrix();
}

void Camera::move(CameraDir dir)
{
	const float delta = ofGetLastFrameTime();

	switch (dir)
	{
	case CameraDir::Forward:
		position += forward * speed * delta;
		break;

	case CameraDir::Back:
		position -= forward * speed * delta;
		break;

	case CameraDir::Up:
		position += glm::vec3(0, 1, 0) * speed * delta;
		break;

	case CameraDir::Down:
		position += glm::vec3(0, -1, 0) * speed * delta;
		break;

	case CameraDir::Right:
		position += right * speed * delta;
		break;

	case CameraDir::Left:
		position -= right * speed * delta;
		break;
	}

	dirty = true;
}

void Camera::rotate(CameraAxis axis, float mouseDelta)
{
	// Pitch has to be constrained in a range smaller than [-90, 90] otherwise weird camera behaviour occurs
	// + it makes the controls really unintuitive
	static float limit = glm::radians(89.0f);

	switch (axis)
	{
	case CameraAxis::Pitch:
		pitch += glm::radians(mouseDelta) * rotationSpeed;
		pitch = glm::clamp(pitch, -limit, limit);
		break;

	case CameraAxis::Yaw:
		yaw += glm::radians(mouseDelta) * rotationSpeed;
		break;
	}

	dirty = true;
}

glm::mat4 Camera::getMatrix()
{
	if (dirty)
	{
		updateMatrix();
		dirty = false;
	}

	return cachedMatrix;
}


void Camera::updateMatrix()
{
	// Yaw and pitch are used for this single purpose: obtaining the forward vector
	forward = normalize(glm::vec3{
		sin(yaw) * cos(pitch),
		-sin(pitch),
		cos(yaw) * cos(pitch)
	});

	// The up vector is always {0, 1, 0}, as it's what the camera rotates around when altering the yaw
	right = normalize(cross(forward, glm::vec3(0, 1, 0)));

	// Compute the camera matrix from the position and orientation
	cachedMatrix = glm::mat4(1);
	cachedMatrix = glm::rotate(cachedMatrix, -pitch, glm::vec3(1, 0, 0));
	cachedMatrix = glm::rotate(cachedMatrix, -yaw, glm::vec3(0, 1, 0));
	cachedMatrix = translate(cachedMatrix, position);
}
