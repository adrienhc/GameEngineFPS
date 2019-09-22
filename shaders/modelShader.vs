#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 FragNorm;
out vec2 TexCoords;


uniform mat4 model_transform;
uniform mat4 viewing;
uniform mat4 projection;

uniform mat3 normal_transform;

void main()
{
	gl_Position = projection * viewing * model_transform * vec4(aPos, 1.0);
	FragPos = vec3(model_transform * vec4(aPos, 1.0f));
	FragNorm = normal_transform * aNormal;
    TexCoords = aTexCoords;    
}