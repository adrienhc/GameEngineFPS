#include "weapon.h"

bool Weapon::ads = false;
float Weapon::ads_time = 0.2f;
float Weapon::time_offset = 0.0f;

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

void Weapon::InterpolateOffset(float delta_time)
{
	if(ads)
	{
		time_offset += delta_time;
		if ( time_offset > ads_time )
			time_offset = ads_time;
		return;
	}
	else
	{
		time_offset -= delta_time;
		if ( time_offset < 0.0f )
			time_offset = 0.0f;
		return;
	}	
}

glm::vec4 Weapon::GetOffset()
{
	float front = (1.0f/ads_time) * ( ( ads_time - time_offset ) * hip_offset.x + time_offset * ads_offset.x);
	float right = (1.0f/ads_time) * ( ( ads_time - time_offset ) * hip_offset.y + time_offset * ads_offset.y);
	float down =  (1.0f/ads_time)  * ( ( ads_time - time_offset ) * hip_offset.z + time_offset * ads_offset.z);
	float zoom =  (1.0f/ads_time)  * ( ( ads_time - time_offset ) * zoom_min + time_offset * zoom_max);

	return glm::vec4(front, right, down, zoom);
}