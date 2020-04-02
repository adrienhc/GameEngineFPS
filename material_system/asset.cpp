#include "asset.h"

Asset::Asset(eShape s, std::string nme, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
			 float shininess, glm::vec4 color, bool texture, std::string texPath)
{
	geometry = s;
	name = nme; 
	material = new Material(ambient, diffuse, specular, shininess, color, texture, texPath);
}

Asset::~Asset()
{
	delete material; 
}

eShape Asset::getGeometry()
{
	return geometry;
}

std::string Asset::getName()
{
	return name;
}

Material* Asset::getMaterial()
{
	if(material != NULL)
		return material;
	else
	{
		std::cout << "Asset " << name << " has no Material.";
		return NULL;
	}
}


