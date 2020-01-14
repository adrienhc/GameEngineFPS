#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Collision
{
public:
	static bool pointCollide(glm::vec3 self_point, glm::vec3 min_bb, glm::vec3 max_bb);
	static bool pointContained(glm::vec3 self_point, glm::vec3 min_bb, glm::vec3 max_bb);
	static bool boxCollide(glm::vec3 self_min_bb, glm::vec3 self_max_bb, glm::vec3 min_bb, glm::vec3 max_bb);
	static bool boxContained(glm::vec3 self_min_bb, glm::vec3 self_max_bb, glm::vec3 min_bb, glm::vec3 max_bb);	
	static bool rayBoxCollide(glm::vec3 rayStart, glm::vec3 rayDir, glm::vec3 min_bb, glm::vec3 max_bb, bool min, float &dist); //do i want min or max value? set reference to it 
};

#endif //COLLISION