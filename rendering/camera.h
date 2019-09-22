#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraMovements {FORWARD, BACKWARDS, LEFT, RIGHT, UP};

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 75;


//BOUNDING BOX 
const float SIDE = 0.4f;
const float HEIGHT = 1.5f;
const float EYE_LEVEL = 0.9f; //as fraction of height from bottom to top

class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera Options
	//float MovementSpeed;
	glm::vec3 MovementSpeed = glm::vec3(SPEED, 4.0f * SPEED, SPEED);
	float MouseSensitivity;
	float Zoom;
	//
	float NearPlane;
	float FarPlane;
	float ViewRatio;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch =PITCH)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)),/* MovementSpeed(SPEED),*/ MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	void SetView(float near, float far, float ratio);
	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(CameraMovements direction, float deltaTime);
	void ProcessMouseMovements(float xoffset, float yoffset, GLboolean constraiPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		float Pitch_Limit = 80.0f; // [0,89]
		if (constraiPitch)
		{
			if(Pitch > Pitch_Limit)
				Pitch = Pitch_Limit;
			if(Pitch < -Pitch_Limit)
				Pitch = -Pitch_Limit;
		}

		UpdateCameraVectors();
	}
	void ProcessMouseScroll(float yoffset);

	void Gravity(float deltaTime);
	glm::vec3 GetMinBB();
	glm::vec3 GetMaxBB();
	void PosFromBB(glm::vec3 min_bb, glm::vec3 max_bb);


private:
	void UpdateCameraVectors();
};

#endif