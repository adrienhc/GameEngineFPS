#ifndef MATERIAL
#define MATERIAL

#include "../pch.h"
#include "texture.h"

class Material
{
public:

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
			  float shininess, glm::vec4 color, bool texture, std::string texPath);
	~Material();
	Texture* getTexture();
	unsigned int getTextureID();
	glm::vec4 getColor();
	unsigned int getColorPacked();
	unsigned int getLightingPacked();
	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;	
	unsigned int lightingPacked;

	glm::vec4 color;
	unsigned int colorPacked;
	
	bool has_texture;
	Texture* texture;
};

#endif //MATERIAL