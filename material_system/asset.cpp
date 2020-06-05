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

BB Asset::getBoundingBox(glm::mat4 &model_transform)
{
	glm::vec4 min;
	glm::vec4 max;

	switch(geometry)
	{
		//Fall throughs on Purpose
		case eTriangle:
		case eSquare:
			min = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
			max = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		break;

		//Fall throughs on Purpose
		case eCube:
		case eInvertedCube:
			min = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
			max = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		break;

		default:
		break;
	}


	min = model_transform * min;
	max = model_transform * max;

	//Perspective Division bring back to World Space
	glm::vec3 tr_min = glm::vec3(min.x, min.y, min.z) / min.w;
	glm::vec3 tr_max = glm::vec3(max.x, max.y, max.z) / max.w;

	BB BoundingBox;
	BoundingBox.InitFromMinMax(tr_min, tr_max);

	return BoundingBox;
}
