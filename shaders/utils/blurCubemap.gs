#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 projectionMatrices[6];
uniform vec2 blurScale;

//output once per EmitVertex()
out vec3 fragTex; //Texture coordinate sample from SOURCE CUBEMAP
out vec3 fragTexOffset; //in 2D space as we are sampling from face of cubemap 

vec3 getTexOffset(int cubemapFace)
{
	//vec2 cubeTex = fragTex * 2.0 - 1.0; //[-1, 1] range
	vec3 texOffset = vec3(0.0f); 

    switch (cubemapFace) 
    {
	    case 0:
	        // fragTex = vec3(1.0, cubeTex.y, cubeTex.x);
	        texOffset = vec3(0.0f, blurScale.y, blurScale.x); 
	        break;
	    case 1:
	        // fragTex = vec3(-1.0, cubeTex.y, -cubeTex.x);
	        texOffset = vec3(0.0f, blurScale.y, -blurScale.x);  
	        break;
	    case 2:
	        // fragTex = vec3(cubeTex.x, 1.0, cubeTex.y);
	        texOffset = vec3(blurScale.x, 0.0f, blurScale.y);
	        break;
	    case 3:
	        // fragTex = vec3(cubeTex.x, -1.0, -cubeTex.y);
	        texOffset = vec3(blurScale.x, 0.0f, -blurScale.y);
	        break;
	    case 4:
	        // fragTex = vec3(-cubeTex.x, cubeTex.y, 1.0);
	        texOffset = vec3(-blurScale.x, blurScale.y, 0.0f);
	        break;
	    case 5:
	        // fragTex = vec3(cubeTex.x, cubeTex.y, -1.0);
	        texOffset = vec3(blurScale.x, blurScale.y, 0.0f);
	        break;
    };

    return texOffset;
}

void main()
{
	for(int face = 0; face < 6; face++)
	{
		gl_Layer = face; //specifies which cubemap face we render to
		fragTexOffset = getTexOffset(face); //DIRECTION OF BLURRING

		for(int i = 0; i < 3; i++)
		{
			gl_Position = projectionMatrices[face] * gl_in[i].gl_Position;
			fragTex = gl_in[i].gl_Position.xyz;
			// fragTexOffset = vec3(0.0f); //FOR NOW, JUST PASS THROUGH, NO BLUR YET
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


 //uniform samplerCube cubemapSrcDepth; //DEPTH ATTACHEMENT
	//uniform samplerCube cubemapSrcColor; //COLOR ATTACHEMENT
	//uniform vec3 blurScale;

    /*
    vec4 color = vec4(0.0);
	color += texture( cubemap, Texcoord.st + (vec2(-3.0) * blurScale.xy)) * (1.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(-2.0) * blurScale.xy)) * (6.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(-1.0) * blurScale.xy)) * (15.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(0.0) * blurScale.xy)) * (20.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(1.0) * blurScale.xy)) * (15.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(2.0) * blurScale.xy)) * (6.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(3.0) * blurScale.xy)) * (1.0f/64.0f);
	FragColor = vec4(color.xyz, 1.0);
	*/