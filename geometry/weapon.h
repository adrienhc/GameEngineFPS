#ifndef WEAPON_H
#define WEAPON_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include "model.h"


class Weapon 
{
public:
	Weapon(char* path, glm::vec3 hip_ofst, glm::vec3 ads_ofst, float scale_factor, float zoom_minimum, float zoom_maximum);
	~Weapon();
	Model* GetModel();
	glm::vec4 GetOffset(); //get the correct offset by interpolation from hip to ads //also do cam zoom  
	glm::vec3 scaling;
	static bool ads;//weapon is being aimed
	static float ads_time; //time to aim in seconds
	static float time_offset;
	static void InterpolateOffset(float delta_time);

private:
	Model* model;
	glm::vec3 hip_offset;
	glm::vec3 ads_offset;
	float zoom_min;
	float zoom_max;
	//Later add bullet count, ammunition ...
};


#endif //WEAPON_H