#version 450 core 

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTex;
layout (location = 3) in mat4 vModelTr;
layout (location = 7) in float vTexDiffID;
layout (location = 8) in float vTexSpecID;
layout (location = 9) in vec4 vLighting;
layout (location = 10) in vec4 vCol;

uniform mat4 viewing;
uniform mat4 projection;

out vec4 fragPos;
out vec3 fragNorm;
out vec2 fragTex;
flat out float fragTexDiffID;
flat out float fragTexSpecID;
flat out vec4 fragLighting;
out vec4 fragColor;

void main()
{
	fragPos = vModelTr * vec4(vPos, 1.0f);
	gl_Position = projection * viewing * fragPos;
	fragNorm = mat3(transpose(inverse(vModelTr))) * vNorm;
	fragTex = vTex;
	fragTexDiffID = vTexDiffID;
	fragTexSpecID = vTexSpecID;
	fragLighting = vLighting / vec4(255.0f, 255.0f, 255.0f, 1.0f);
	fragColor = vCol;
}

	