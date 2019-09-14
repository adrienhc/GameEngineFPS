/*
#ifndef SCENE
#define SCENE

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "scenegraph.h"
#include "geometry.h"
#include "camera.h"
#include "asset.h"



class Scene
{
public:
	Scene();
	nNode* Root;
private:
	glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 LightPos = glm::vec3(2.0f, 2.0f, 0.0f);
	Asset crate = Asset(eCube, "crate", glm::vec3(0.4f), glm::vec3(0.8f), glm::vec3(1.0f), 32.0f, glm::vec3(0.0f), true, "./textures/cratebw.jpg");
	Light light = Light(LightPos, glm::vec3(0.3f),  LightColor, glm::vec3(1.0f));
};

/*
nTransform* Origin = new nTransform(glm::vec3( 0.0f,  0.0f,  0.0f));
nAsset* Crate = new nAsset(&crate, eObject); 
nLight* Light = new nLight(&light);
Root->AddChildren(Origin);
Origin->AddChildren(Light);
Origin->AddChildren(Crate);

#endif
*/