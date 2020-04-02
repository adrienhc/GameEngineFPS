#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model_transform;
uniform mat4 viewing;
uniform mat4 projection;

void main()
{
	gl_Position = projection * viewing * model_transform * vec4(aPos + aNormal * 0.05f, 1.0);
}