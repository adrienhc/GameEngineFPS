#ifndef TARGET_H
#define TARGET_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp> //rotate vector to place targets correctly

#include <iostream>

#include "model.h"
#include "cluster.h" //for BB class

#include "../utils/collision.h"
#include "../utils/outline.h"

#include "../scene/scenegraph.h"

class Target
{
public:
	Target(glm::vec3 position, char orientation); //'N' 'S' 'E' 'W' -- For Bounding Box 
	Target(glm::vec3 position, char orientation, glm::vec3 outline_color, float outline_size); //Special outline
	~Target();
	static void LoadModel(char* path);
	static void LoadSmoothModel(char* path);

	nModel* GetNodeModel();

	Model* GetModel();
	Model* GetSmoothModel();
	Outline* GetOutline();
	
	bool Collide();
	
	static Model* model;
	static Model* smoothModel;
	Outline* outline = NULL;
	nModel* nodeModel = NULL;
	glm::mat4 transform;

private: 
	static bool loadedModel; //only load model once 
	static bool loadedSmoothModel; //only load model once 
	static int count;
	bool shot;
	BB boundingBox;
};


#endif //TARGET_H