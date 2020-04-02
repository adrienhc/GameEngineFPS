#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTex;

uniform mat4 model_transform;
uniform mat4 viewing;
uniform mat4 projection;

uniform mat3 normal_transform;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 fragTex;



void main()
{
	gl_Position = projection * viewing * model_transform * vec4 (vPos, 1.0);
	fragPos = vec3(model_transform * vec4 (vPos, 1.0));
	fragNorm = normal_transform * vNorm;
	fragTex = vTex;
}
