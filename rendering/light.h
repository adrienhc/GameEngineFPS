#ifndef LIGHT
#define LIGHT

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class PointLight
{
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	void setTransform(glm::mat4 transform);

	//Light Properties
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;	
	glm::vec3 position;
	
	//Attenuation
	float constant;
	float linear;
	float quadratic;
};

#endif //LIGHT