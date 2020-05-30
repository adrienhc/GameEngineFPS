#include "material.h"

Material::Material(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, 
	               float shine, glm::vec4 col, bool tex, std::string Path)
{
	ambient = amb;
	diffuse = diff;
	specular = spec;
	shininess = shine;
	color = col;
	has_texture = tex;

	if(has_texture)
	{
		texture = new Texture(Path);
	}

	//between [0,1] so can use full 8 bit range [0, 255] for min loss of precision
	int r = col.x * 255.0f;
	int g = col.y * 255.0f;
	int b = col.z * 255.0f;
	int a = col.w * 255.0f;
	colorPacked = ( a << 24 | b << 16 | g << 8 | r );

	int am = ambient.x * 255.0f; 
	int di = diffuse.x * 255.0f;
	int sp = specular.x * 255.0f;
	//cannot do the same trick, shininess value are already between [0, 255]
	int sh = shininess; 
	lightingPacked = ( sh << 24 | sp << 16 | di << 8 | am );
}

Material::~Material()
{
	if(has_texture && texture != NULL)
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

glm::vec4 Material::getColor()
{
	return color;
}

unsigned int Material::getTextureID()
{
	if(has_texture && texture != NULL) //if texture is NULL
		return texture->GetID();
	else
		return 0;
}

unsigned int Material::getColorPacked()
{
	return colorPacked;
}

unsigned int Material::getLightingPacked()
{
	return lightingPacked;
}