#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTex;
layout (location = 3) in mat4 instance_model_tr; //3 4 5 6
layout (location = 7) in mat3 instance_normal_tr; //7 8 9

uniform mat4 viewing;
uniform mat4 projection;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 fragTex;

void main()
{
	gl_Position = projection * viewing * instance_model_tr * vec4 (vPos, 1.0);
	fragPos = vec3(instance_model_tr * vec4 (vPos, 1.0));
	
	fragNorm = instance_normal_tr * vNorm;
	//fragNorm = mat3(instance_model_tr) * vNorm;
	//fragNorm = mat3(transpose(inverse(instance_model_tr))) * vNorm;
	
	fragTex = vTex;
}
