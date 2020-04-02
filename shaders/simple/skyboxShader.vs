#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 viewing;
uniform mat4 projection;

out vec3 TexCoords;

void main()
{
	vec4 pos = projection * viewing * vec4(aPos, 1.0f);
	gl_Position = pos.xyww;
	TexCoords = aPos;
}