#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 projectionMatrices[6];

//output once per EmitVertex()
out vec3 fragTex; //Texture coordinate sample from SOURCE CUBEMAP
out vec3 fragTexOffset;

void main()
{
	for(int face = 0; face < 6; face++)
	{
		gl_Layer = face; //specifies which cubemap face we render to
		for(int i = 0; i < 3; i++)
		{
			fragPos = gl_in[i].gl_Position;
			gl_Position = projectionMatrices[face] * fragPos;
			fragTex = fragPos.xyz;
			fragTexOffset = vec3(0.0f); //FOR NOW, JUST PASS THROUGH, NO BLUR YET
			EmitVertex();
		}
		EndPrimitive();
	}
}



/*

vec2 cubeTex = vTex * 2.0 - 1.0; //[-1, 1] range

    switch (cubemapFace) 
    {
	    case 0:
	        fragTex = vec3(1.0, cubeTex.y, cubeTex.x);
	        break;
	    case 1:
	        fragTex = vec3(-1.0, cubeTex.y, -cubeTex.x);
	        break;
	    case 2:
	        fragTex = vec3(cubeTex.x, 1.0, cubeTex.y);
	        break;
	    case 3:
	        fragTex = vec3(cubeTex.x, -1.0, -cubeTex.y);
	        break;
	    case 4:
	        fragTex = vec3(-cubeTex.x, cubeTex.y, 1.0);
	        break;
	    case 5:
	        fragTex = vec3(cubeTex.x, cubeTex.y, -1.0);
	        break;
    };
 */