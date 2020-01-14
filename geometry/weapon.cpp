#include "weapon.h"

bool Weapon::ads = false;
float Weapon::ads_time = 0.2f;
float Weapon::ads_time_offset = 0.0f;

bool Weapon::fire = false;
float Weapon::fire_time = 0.05f;
float Weapon::fire_time_offset = 0.0f;

Weapon::Weapon(char* path, glm::vec3 hip_ofst, glm::vec3 ads_ofst, float scale_factor, float zoom_minimum, float zoom_maximum)
{
	model = new Model(path);
	hip_offset = hip_ofst;
	ads_offset = ads_ofst;
	scaling = glm::vec3(scale_factor);
	zoom_min = zoom_minimum;
	zoom_max = zoom_maximum;
}

Weapon::~Weapon()
{
	delete model;
}

Model* Weapon::GetModel()
{
	return model;
}

void Weapon::InterpolateOffsets(float delta_time)
{
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

	if(fire)
	{
		fire_time_offset += delta_time;
		if ( fire_time_offset > fire_time )
			fire_time_offset = fire_time;
	}
	else
	{
		fire_time_offset -= delta_time;
		if ( fire_time_offset < 0.0f )
			fire_time_offset = 0.0f;
	}
}

glm::vec4 Weapon::GetADSOffset()
{
	float front = (1.0f/ads_time) * ( ( ads_time - ads_time_offset ) * hip_offset.x + ads_time_offset * ads_offset.x);
	float right = (1.0f/ads_time) * ( ( ads_time - ads_time_offset ) * hip_offset.y + ads_time_offset * ads_offset.y);
	float down =  (1.0f/ads_time)  * ( ( ads_time - ads_time_offset ) * hip_offset.z + ads_time_offset * ads_offset.z);
	float zoom =  (1.0f/ads_time)  * ( ( ads_time - ads_time_offset ) * zoom_min + ads_time_offset * zoom_max);

	return glm::vec4(front, right, down, zoom);
}

float Weapon::GetRecoilOffset()
{
	return 20.0f * fire_time_offset;
}