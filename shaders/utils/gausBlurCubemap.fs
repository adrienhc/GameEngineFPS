#version 450 core

in vec3 fragTex;
in vec3 fragTexOffset; //INCLUDES BLUR SCALE
out vec4 FragColor;

uniform samplerCube cubemapSrc; 

void main()
{	
	vec4 data = vec4(0.0);
	data += texture( cubemapSrc, fragTex + (fragTexOffset * (-3.0))) * (1.0f/64.0f);
	data += texture( cubemapSrc, fragTex + (fragTexOffset * (-2.0))) * (6.0f/64.0f);
	data += texture( cubemapSrc, fragTex + (fragTexOffset * (-1.0))) * (15.0f/64.0f);
	data += texture( cubemapSrc, fragTex + (fragTexOffset * (0.0))) * (20.0f/64.0f);
	data += texture( cubemapSrc, fragTex + (fragTexOffset * (1.0))) * (15.0f/64.0f);
	data += texture( cubemapSrc, fragTex + (fragTexOffset * (2.0))) * (6.0f/64.0f);
	data += texture( cubemapSrc, fragTex + (fragTexOffset * (3.0))) * (1.0f/64.0f);
	FragColor = data;
} 


    