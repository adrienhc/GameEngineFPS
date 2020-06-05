#ifndef OUTLINE_H
#define OUTLINE_H

#include "../pch.h"

class Outline 
{
public:
	Outline(glm::vec4 _color, float _size)
	{
		SetColor(_color);
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

	void SetColor(glm::vec4 _color)
	{
		color = _color;
		int r = color.x * 255.0f;
		int g = color.y * 255.0f;
		int b = color.z * 255.0f;
		int a = color.w * 255.0f;
		colorPacked = ( a << 24 | b << 16 | g << 8 | r );

	}

	glm::vec4 GetColor()
	{
		return color;
	}
	
	unsigned int GetColorPacked()
	{
		return colorPacked;
	}


	bool status;
	glm::vec4 color;
	unsigned int colorPacked;
	float size;
};

#endif //OUTLINE_H