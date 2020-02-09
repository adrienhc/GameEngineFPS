#ifndef WEAPON_H
#define WEAPON_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "model.h"
#include "../material_system/asset.h"

//could be an FSM, using design pattern!
class Weapon 
{
public:
	Weapon(char* path, glm::vec3 hip_ofst, glm::vec3 ads_ofst, float scale_factor, float zoom_minimum, float zoom_maximum);
	~Weapon();
	Model* GetModel();
	glm::vec4 GetADSOffset(); //get the correct offset by interpolation from hip to ads //also do cam zoom  
	bool IsFullyADS();
	float GetRecoilOffset();
	glm::vec3 scaling;
	static bool ads;//weapon is being aimed
	static float ads_time; //time to aim in seconds
	static float ads_time_offset;
	static bool fire;//pulled the trigger
	static float fire_time; //time to aim in seconds
	static float fire_time_offset;
	static int recoil_sign;
	static bool flash;
	static void InterpolateOffsets(float delta_time);
	Asset* muzzleFlash = new Asset(eSquare, "muzzleFlash", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f), 32.0f, glm::vec3(0.0f), true, "./textures/muzzleflashtrimmed2.png");

private:
	Model* model;
	glm::vec3 hip_offset;
	glm::vec3 ads_offset;
	float zoom_min;
	float zoom_max;
	//Later add bullet count, ammunition ...
};


#endif //WEAPON_H