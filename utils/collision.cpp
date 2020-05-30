#include "collision.h"

bool Collision::pointCollide(glm::vec3 self_point, glm::vec3 min_bb, glm::vec3 max_bb)
{
		return  (min_bb.x <= self_point.x && self_point.x <= max_bb.x) &&
        		(min_bb.y <= self_point.y && self_point.y <= max_bb.y) &&
        		(min_bb.z <= self_point.z && self_point.z <= max_bb.z);
}


bool Collision::pointContained(glm::vec3 self_point, glm::vec3 min_bb, glm::vec3 max_bb)
{
		return  (min_bb.x < self_point.x && self_point.x < max_bb.x) &&
        		(min_bb.y < self_point.y && self_point.y < max_bb.y) &&
        		(min_bb.z < self_point.z && self_point.z < max_bb.z);
}


bool Collision::boxCollide(glm::vec3 self_min_bb, glm::vec3 self_max_bb, glm::vec3 min_bb, glm::vec3 max_bb)
{

	return ( self_max_bb.x > min_bb.x && self_min_bb.x < max_bb.x &&
			 self_max_bb.y > min_bb.y && self_min_bb.y < max_bb.y &&
			 self_max_bb.z > min_bb.z && self_min_bb.z < max_bb.z );
}

bool Collision::boxCollide(glm::vec2 self_min_bb, glm::vec2 self_max_bb, glm::vec2 min_bb, glm::vec2 max_bb)
{

	return ( self_max_bb.x > min_bb.x && self_min_bb.x < max_bb.x &&
			 self_max_bb.y > min_bb.y && self_min_bb.y < max_bb.y);
}

bool Collision::boxContained(glm::vec3 self_min_bb, glm::vec3 self_max_bb, glm::vec3 min_bb, glm::vec3 max_bb)
{
	return pointContained(self_min_bb, min_bb, max_bb) && pointContained(self_max_bb, min_bb, max_bb);
}


bool Collision::rayBoxCollide(glm::vec3 rayStart, glm::vec3 rayDir, glm::vec3 min_bb, glm::vec3 max_bb, bool min, float &dist)
{
	//from outside of box! tmin intersection 
	//from inside box, tmax intersection (if tmin less or eq 0)

	//float rayLength = 50.0f;
	//glm::vec3 rayEnd = rayStart + rayLength * rayDir;

	float txMin = (min_bb.x - rayStart.x) / rayDir.x;
	float txMax = (max_bb.x - rayStart.x) / rayDir.x;
	if(txMax < txMin)
	{
		float temp = txMax;
		txMax = txMin;
		txMin = temp; 
	}

	float tyMin = (min_bb.y - rayStart.y) / rayDir.y;
	float tyMax = (max_bb.y - rayStart.y) / rayDir.y;
	if(tyMax < tyMin)
	{
		float temp = tyMax;
		tyMax = tyMin;
		tyMin = temp; 
	}	

	float tzMin = (min_bb.z - rayStart.z) / rayDir.z;
	float tzMax = (max_bb.z - rayStart.z) / rayDir.z;
	if(tzMax < tzMin)
	{
		float temp = tzMax;
		tzMax = tzMin;
		tzMin = temp; 
	}

	//want Greatest Min and Smallest Max
	float tMin = (txMin > tyMin)? txMin : tyMin; 
	float tMax = (txMax < tyMax)? txMax : tyMax;

	if(txMin > tyMax || tyMin > txMax) 
		return false;
	if(tMin > tzMax || tzMin > tMax) 
		return false;

	//want Greatest Min and Smallest Max
	if(tzMin > tMin)
		tMin = tzMin;
	if(tzMax < tMax)
		tMax = tzMax;

	if(min)
		dist = tMin;
	else
		dist = tMax;
	//tMin and tMax scaling factors for rayDir
	return true;

}