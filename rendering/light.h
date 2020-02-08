#ifndef LIGHT
#define LIGHT

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

#include "shader.h"
#include <glm/gtx/string_cast.hpp>

class Shader; //since shader also includes PointLight

class PointLight
{
public:
	PointLight(int num_lights, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	void setTransform(glm::mat4 transform);

	static int MAX_LIGHTS; 
	
	//Light Properties
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;	
	glm::vec3 position;
	
	//Attenuation
	float constant;
	float linear;
	float quadratic;

	void shadowPassSetup(Shader depthShader, int index);
	void bindShadowMap(Shader shader, int index);

private: 
	int NUM_LIGHTS;
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;
	std::vector<unsigned int> shadowFBO; //[NUM_LIGHTS];
	std::vector<unsigned int> shadowCubemap; //[NUM_LIGHTS];

	float aspect;
	float near;
	float far;
	glm::mat4 shadowProj;
	std::vector<glm::mat4> shadowTransform;

};

#endif //LIGHT