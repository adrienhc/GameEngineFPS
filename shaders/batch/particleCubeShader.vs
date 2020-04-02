#version 450 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vCol;
layout (location = 2) in float vSize;

out VS_OUT
{
	vec4 fragPos;
	vec4 fragCol;
	float fragSize;
} vs_out;

void main()
{
	vs_out.fragPos = vPos;
	vs_out.fragCol = vCol;
	vs_out.fragSize = vSize;
}
