#ifndef TARGET_H
#define TARGET_H

#include "../pch.h"
#include "model.h"
#include "cluster.h" //for BB class

#include "../utils/collision.h"
#include "../utils/outline.h"
#include "../scene/scenegraph.h"

class Target
{
public:
	Target(glm::vec3 position, char orientation); //'N' 'S' 'E' 'W' -- For Bounding Box 
	Target(glm::vec3 position, char orientation, glm::vec4 outline_color, float outline_size); //Special outline
	~Target();
	static void LoadModel(char* path);
	static void LoadSmoothModel(char* path);

	nModel* GetNodeModel();

	Model* GetModel();
	Model* GetSmoothModel();
	Outline* GetOutline();

	void Shot();
	bool IsShot();
	float TimeSinceShot();
	bool Erase();

	void ResetDist(float max_dist);
	bool TestDist(float dist);
	void SetHitRay(bool val);
	bool HitRay();


	BB body_low_bb;
	BB body_high_bb;
	BB head_bb;

private: 
	static Model* model;
	static Model* smoothModel;

	Outline* outline = NULL;
	nModel* nodeModel = NULL;
	glm::mat4 transform;

	
	static bool loadedModel; //only load model once 
	static bool loadedSmoothModel; //only load model once 
	static int count;

	float lifeSpan = 0.30f;
	bool shot = false;
	float shotTime = 0.0f;
	float collisionDist = 1000.0f;
	bool hitRay = false;
};


#endif //TARGET_H