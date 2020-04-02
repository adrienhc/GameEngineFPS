#version 450 core 

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vCol;

uniform mat4 viewing;
uniform mat4 projection;

out vec4 fragColor;

void main()
{
	gl_Position = projection * viewing * vPos;
	fragColor = vCol;
}