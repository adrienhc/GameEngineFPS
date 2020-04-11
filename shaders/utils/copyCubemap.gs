#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 projectionMatrices[6];

//output once per EmitVertex()
out vec3 fragTex; //Texture coordinate sample from SOURCE CUBEMAP

void main()
{
	for(int face = 0; face < 6; face++)
	{
		gl_Layer = face; //specifies which cubemap face we render to
		for(int i = 0; i < 3; i++)
		{
			gl_Position = projectionMatrices[face] * gl_in[i].gl_Position;
			fragTex = gl_in[i].gl_Position.xyz;
			EmitVertex();
		}
		EndPrimitive();
	}
}

