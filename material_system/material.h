#ifndef MATERIAL
#define MATERIAL

#include<iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "texture.h"

class Material
{
public:

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
			  float shininess, glm::vec3 color, bool texture, std::string texPath);
	~Material();
	Texture* getTexture();
	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;	
	glm::vec3 color;
	bool has_texture;
	Texture* texture;
};

#endif //MATERIAL