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

bool Collision::boxContained(glm::vec3 self_min_bb, glm::vec3 self_max_bb, glm::vec3 min_bb, glm::vec3 max_bb)
{
	return pointContained(self_min_bb, min_bb, max_bb) && pointContained(self_max_bb, min_bb, max_bb);
}


//OLD BOXCOLLIDE
	/*
	return pointCollide(self_min_bb, min_bb, max_bb) || pointCollide(self_max_bb, min_bb, max_bb) ||
	//COMPARE MAX Y FIRST, MOST LIKELY TO SUCCEED DUE TO GRAVITY
	pointCollide(glm::vec3 (self_max_bb.x, self_max_bb.y, self_min_bb.z), min_bb, max_bb) || 
	pointCollide(glm::vec3 (self_min_bb.x, self_max_bb.y, self_min_bb.z), min_bb, max_bb) || 
	pointCollide(glm::vec3 (self_min_bb.x, self_max_bb.y, self_max_bb.z), min_bb, max_bb) ||
	//MIN Y LESS LIKELY TO HAPPEN, DO LAST
	pointCollide(glm::vec3 (self_max_bb.x, self_min_bb.y, self_max_bb.z), min_bb, max_bb) || 
	pointCollide(glm::vec3 (self_max_bb.x, self_min_bb.y, self_min_bb.z), min_bb, max_bb) ||
	pointCollide(glm::vec3 (self_min_bb.x, self_min_bb.y, self_max_bb.z), min_bb, max_bb) ||
	
	//BOX WITHIN, 2D Collision, LEAST LIKELY -- REWORK
	pointCollide(min_bb, self_min_bb, self_max_bb) || pointCollide(max_bb, self_min_bb, self_max_bb);
	*/
