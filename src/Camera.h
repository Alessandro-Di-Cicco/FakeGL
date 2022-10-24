#pragma once
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

enum class CameraDir;
enum class CameraAxis;

/**
 * \brief Allows to manipulate a virtual camera and easily get it's view matrix
 */
class Camera
{
public:
	Camera(glm::vec3 pos);
	/**
	 * \brief Update the camera's position
	 * \param dir the direction
	 */
	void move(CameraDir dir);
	/**
	 * \brief Rotate the camera around the given axis
	 * \param axis the axis of rotation
	 * \param mouseDelta the rotation delta
	 */
	void rotate(CameraAxis axis, float mouseDelta);

	/**
	 * \return The view matrix of this camera
	 */
	glm::mat4 getMatrix();

private:
	/**
	 * \brief The matrix returned by getMatrix()
	 */
	glm::mat4 cachedMatrix{};

	// Self explainatory
	glm::vec3 position;
	float pitch{0};
	float yaw{};
	float speed{ 4 };
	float rotationSpeed{ 0.3f };

	/**
	 * \brief Set to true by move() and rotate() calls. When calling getMatrix(), if this is true, updateMatrix() is called
	 * before returning cachedMatrix
	 */
	bool dirty;

	// Forward and right direction vectors, used to move and rotate
	glm::vec3 forward;
	glm::vec3 right;

	/**
	 * \brief Re-compute the view matrix and store it in cachedMatrix
	 */
	void updateMatrix();
};

/**
 * \brief Represents a movement direction
 */
enum class CameraDir
{
	Forward,
	Back,
	Up,
	Down,
	Right,
	Left
};

/**
 * \brief Represents a rotation direction around one of the three euclidean axes
 */
enum class CameraAxis
{
	Pitch,
	Yaw
};