#ifndef WEAPON_H
#define WEAPON_H

#include "../pch.h"

#include "model.h"
#include "../material_system/asset.h"
#include "../particle_system/particle.h"


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
	bool IsReloading();
	float GetRecoilOffset();
	float GetFireOffset();
	float GetReloadOffset();
	float GetMagazineCapacity();
	bool NewBullet();
	bool IsFiring();
	bool IsFlashing();
	static bool ads;//weapon is being aimed -- keep static
	static bool fire;//pulled the trigger -- keep static
	static bool reload;
	static bool newBullet; //keep static
	Asset* muzzleFlash;
	Particle* baseBullet;
	Particle* baseFlash;
	Particle* baseSmokeFire;
	Particle* baseSmokeStill;

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
	float ads_time = 0.1f; //MAKE VARY PER WEAPON
	float fire_time = 0.08f; //HALF OF TIME BETWEEN TWO BULLETS //MAKE VARY PER WEAPON 
	float reload_time = 0.8f; //MAKE VARY PER WEAPON
	float recoil_strength;
	
	//Shared for all weapons
	float ads_time_offset = 0.0f;
	float fire_time_offset = 0.0f;
	float reload_time_offset = 0.0f;
	int recoil_sign = 1;
	bool flash = false;
};


#endif //WEAPON_H