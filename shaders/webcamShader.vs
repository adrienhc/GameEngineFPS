#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model_transform;
uniform mat4 viewing;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
	//gl_Position = vec4(aPos, 1.0);
	gl_Position = projection * viewing * model_transform * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}