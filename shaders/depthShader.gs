#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrices[6];
out vec4 FragPos; //output once per EmitVertex()

void main()
{
	for(int face = 0; face < 6; face++)
	{
		gl_Layer = face; //specifies which cubemap face we render to
		for(int i = 0; i < 3; i++)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}