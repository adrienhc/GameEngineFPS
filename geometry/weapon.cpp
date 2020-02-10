#include "weapon.h"

bool Weapon::ads = false;
bool Weapon::fire = false;
bool Weapon::reload = false;
bool Weapon::newBullet = false;

Weapon::Weapon(char* path, glm::vec3 hip_ofst, glm::vec3 ads_ofst, float scale_factor, float zoom_minimum, float zoom_maximum)
{
	model = new Model(path);
	hip_offset = hip_ofst;
	ads_offset = ads_ofst;
	scaling = glm::vec3(scale_factor);
	zoom_min = zoom_minimum;
	zoom_max = zoom_maximum;
	recoil_sign = 1.0f;
}

Weapon::~Weapon()
{
	delete model;
}

void Weapon::Update(float delta_time)
{

	//RELOAD STATE
	if(reload)
	{
		if(bullets == clip_size)
		{
			reload = false;
			reload_time_offset = 0.0f;
		}
		else
		{
			fire = false;
			ads = false;
			fire_time_offset = 0.0f;
			
			if(ads_time_offset == 0.0f) //Only Reload once not aiming
				reload_time_offset += delta_time;
			

			if(reload_time_offset > reload_time)
			{
				reload_time_offset = 0.0f;
				bullets = clip_size;
				reload = false;
			}
		}
	}

	//ADS STATE
	if(ads)
	{
		ads_time_offset += delta_time;
		if ( ads_time_offset > ads_time )
			ads_time_offset = ads_time;
	}
	else
	{
		ads_time_offset -= delta_time;
		if ( ads_time_offset < 0.0f )
			ads_time_offset = 0.0f;
	}	

	if (newBullet && bullets > 0)
	{ 
		newBullet = false;
		bullets--;
	}

	fire = fire && (bullets > 0);

	//FIRING STATE
	if(fire) 
	{
		if ( fire_time_offset > fire_time )
		{
			recoil_sign = -1;
			fire_time_offset = fire_time;
		}
		else if ( fire_time_offset <= 0.0f )
		{
			recoil_sign = 1;
			newBullet = true; //New Bullet only when firing and gun at steady pos  
			fire_time_offset = 0.0f;
		}

		fire_time_offset += delta_time * recoil_sign;
		flash = (recoil_sign == 1) && (bullets > 0); // && (fire_time_offset < 0.3f * fire_time);
	}
	else
	{
		newBullet = false;
		flash = false;
		recoil_sign = -1;
		fire_time_offset += delta_time * recoil_sign;
		if ( fire_time_offset < 0.0f )
			fire_time_offset = 0.0f;
	}

}

Model* Weapon::GetModel()
{
	return model;
}

glm::vec3 Weapon::GetScaling()
{
	return scaling;
}

glm::vec4 Weapon::GetADSOffset()
{
	float front = (1.0f/ads_time) * ( ( ads_time - ads_time_offset ) * hip_offset.x + ads_time_offset * ads_offset.x);
	float right = (1.0f/ads_time) * ( ( ads_time - ads_time_offset ) * hip_offset.y + ads_time_offset * ads_offset.y);
	float down =  (1.0f/ads_time)  * ( ( ads_time - ads_time_offset ) * hip_offset.z + ads_time_offset * ads_offset.z);
	float zoom =  (1.0f/ads_time)  * ( ( ads_time - ads_time_offset ) * zoom_min + ads_time_offset * zoom_max);

	return glm::vec4(front, right, down, zoom);
}

bool Weapon::IsFullyADS()
{
	return ads_time_offset >= ads_time;
}

bool Weapon::NewBullet()
{
	return newBullet;
}

bool Weapon::IsFiring()
{
	return fire;
}

bool Weapon::IsFlashing()
{
	return flash;
}

float Weapon::GetFireOffset()
{
	return fire_time_offset;
}

float Weapon::GetRecoilOffset()
{
	return 20.0f * fire_time_offset;
}

float Weapon::GetReloadOffset()
{
	return  reload_time_offset/reload_time;
}