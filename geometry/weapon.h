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
	void Update(float delta_time);
	Model* GetModel();
	glm::vec3 GetScaling();
	glm::vec4 GetADSOffset(); //get the correct offset by interpolation from hip to ads //also do cam zoom  
	glm::vec3 GetScale();
	glm::mat4 SetModelTransform(glm::mat4 weapon_transform);
	glm::mat4 GetModelTransform();
	bool IsFullyADS();
	float GetRecoilOffset();
	float GetFireOffset();
	float GetReloadOffset();
	bool NewBullet();
	bool IsFiring();
	bool IsFlashing();
	static bool ads;//weapon is being aimed -- keep static
	static bool fire;//pulled the trigger -- keep static
	static bool reload;
	static bool newBullet; //keep static
	Asset* muzzleFlash = new Asset(eSquare, "muzzleFlash", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f), 32.0f, glm::vec4(0.0f), true, "./textures/muzzleflashtrimmed2.png");

private:
	//Weapon Specific
	Model* model;
	glm::vec3 scaling;
	glm::vec3 hip_offset;
	glm::vec3 ads_offset;
	glm::mat4 model_transform;
	float zoom_min;
	float zoom_max;
	unsigned int clip_size = 30; //MAKE VARY PER WEAPON
	unsigned int bullets = clip_size; //MAKE VARY PER WEAPON
	float ads_time = 0.2f; //MAKE VARY PER WEAPON
	float fire_time = 0.03f; //HALF OF TIME BETWEEN TWO BULLETS //MAKE VARY PER WEAPON 
	float reload_time = 1.0f; //MAKE VARY PER WEAPON

	//Shared for all weapons
	float ads_time_offset = 0.0f;
	float fire_time_offset = 0.0f;
	float reload_time_offset = 0.0f;
	int recoil_sign = 1;
	bool flash = false;
};


#endif //WEAPON_H