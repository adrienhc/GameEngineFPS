#include "light.h"

PointLight::PointLight(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float cst, float lin, float quad)
{
	ambient = amb;
	diffuse = diff;
	specular = spec;
	position = glm::vec3(0.0f, 0.0f, 0.0f);

	constant = cst;
	linear = lin;
	quadratic = quad;
}


void PointLight::setTransform(glm::mat4 trsf)
{
	glm::vec4 pos = trsf * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);  //Apply current scene graph transform
	position =  glm::vec3(pos.x, pos.y, pos.z);
}	
