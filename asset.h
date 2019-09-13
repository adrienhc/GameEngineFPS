#ifndef ASSET
#define ASSET

#include <iostream>
#include <string>
#include "material.h"
#include "geometry.h"

class Asset //Geometry type, Material, Color/Texture
{
	public:
		Asset(eShape s, std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				  float shininess, glm::vec3 color, bool texture, std::string texPath);
		~Asset();

		eShape getGeometry();
		std::string getName();
		Material* getMaterial();
		//Geometry* getGeometry();

	private:
		eShape geometry;
		std::string name;
		Material *material;
};

#endif //ASSET