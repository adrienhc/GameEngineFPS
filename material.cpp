#include "material.h"

Material::Material(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, 
	               float shine, glm::vec3 col, bool tex, std::string Path)
{
	ambient = amb;
	diffuse = diff;
	specular = spec;
	shininess = shine;
	
	has_texture = tex;

	if(has_texture)
	{
		texture = new Texture(Path);
	}
	else
	{
		color = col;
	}
}

Material::~Material()
{
	if(has_texture)
		delete texture;
}

Texture* Material::getTexture()
{
	if(has_texture && texture != NULL)
	{
		return texture;
	}
	else
	{
		std::cout << "Material has no Texture" << std::endl;
		return NULL;
	}
}