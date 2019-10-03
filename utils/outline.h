#ifndef OUTLINE_H
#define OUTLINE_H

#include <glm/glm.hpp>

class Outline 
{
public:
	Outline(glm::vec3 _color, float _size)
	{
		color = _color;
		size = _size;
		status = true;
	}

	void Disable()
	{
		status = false;
	}

	void Enable()
	{
		status = true;
	}

	bool Enabled()
	{
		return status;
	}


	bool status;
	glm::vec3 color;
	float size;
};

#endif //OUTLINE_H