#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 instanced_model_transform;

uniform bool isInstanced;
uniform mat4 model_transform;

void main()
{
	if(isInstanced)
		gl_Position = instanced_model_transform * vec4(aPos, 1.0f);
	else
		gl_Position = model_transform * vec4(aPos, 1.0f);
}