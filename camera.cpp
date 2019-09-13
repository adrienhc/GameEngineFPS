#include "camera.h"

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up); 
}

void Camera::ProcessKeyboard(CameraMovements direction, float deltaTime)
{
	//float velocity = MovementSpeed * deltaTime;
	/*if(direction == FORWARD)
		Position += Front * velocity;
	if(direction == BACKWARDS)
		Position -= Front * velocity;
	if(direction == LEFT)
		Position -= Right * velocity;
	if(direction == RIGHT)
		Position += Right * velocity;*/

	glm::vec3 velocity = glm::vec3(MovementSpeed.x * deltaTime, MovementSpeed.y * deltaTime, MovementSpeed.z * deltaTime);
	
	if(direction == FORWARD)
		Position += glm::vec3(Front.x * velocity.x, 0.0f, Front.z * velocity.z); //PROBLEM, DEPENDS ON CAMERA, DECOMPOSE VEC 
	if(direction == BACKWARDS)
		Position -= glm::vec3(Front.x * velocity.x, 0.0f, Front.z * velocity.z);
	if(direction == LEFT)
		Position -= glm::vec3(Right.x * velocity.x, 0.0f, Right.z * velocity.z);
	if(direction == RIGHT)
		Position += glm::vec3(Right.x * velocity.x, 0.0f, Right.z * velocity.z);
	if(direction == UP)
		Position += WorldUp * velocity.y;
}

void Camera::Gravity(float deltaTime)
{
	//GRAVITY
	glm::vec3 velocity = glm::vec3(MovementSpeed.x * deltaTime, MovementSpeed.y * deltaTime, MovementSpeed.z * deltaTime);
	Position -= WorldUp * 0.5f * velocity.y; //ALWAYS
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if(Zoom >= 10.0f && Zoom <= 75.0f)
		Zoom -= yoffset;
	if(Zoom < 10.0f)
		Zoom = 10.0f;
	if(Zoom > 75.0f)
		Zoom = 75.0f;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}



glm::vec3 Camera::GetMinBB()
{
	float x = Position.x - SIDE/2.0f;
	float y = Position.y - EYE_LEVEL * HEIGHT;
	float z = Position.z - SIDE/2.0f;

	return glm::vec3(x, y, z);
}

glm::vec3 Camera::GetMaxBB()
{
	float x = Position.x + SIDE/2.0f;
	float y = Position.y + (1.0f - EYE_LEVEL) * HEIGHT;
	float z = Position.z + SIDE/2.0f;

	return glm::vec3(x, y, z);
}

void Camera::PosFromBB(glm::vec3 min_bb, glm::vec3 max_bb)
{
	float x = min_bb.x + SIDE/2.0f;
	float y = min_bb.y + EYE_LEVEL * HEIGHT;
	float z = min_bb.z + SIDE/2.0f;

	Position = glm::vec3(x, y, z);
	UpdateCameraVectors();
}