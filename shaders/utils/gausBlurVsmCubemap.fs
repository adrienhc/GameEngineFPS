#version 450 core

in vec3 fragTex;
in vec3 fragTexOffset; //INCLUDES BLUR SCALE
out vec4 FragColor;

uniform samplerCube cubemapSrc; 

void main()
{	
	vec4 data = vec4(0.0f);
	vec2 data_rg = vec2(0.0f);
	float data_b = 1000.0f;

	data = texture( cubemapSrc, fragTex + (fragTexOffset * (-3.0))) * (1.0f/64.0f);
	data_rg += data.rg;
	data_b = min(data_b, data.b * 64.0f / 1.0f);  

	data = texture( cubemapSrc, fragTex + (fragTexOffset * (-2.0))) * (6.0f/64.0f);
	data_rg += data.rg;
	data_b = min(data_b, data.b * 64.0f / 6.0f);

	data = texture( cubemapSrc, fragTex + (fragTexOffset * (-1.0))) * (15.0f/64.0f);
	data_rg += data.rg;
	data_b = min(data_b, data.b * 64.0f / 15.0f);

	data = texture( cubemapSrc, fragTex + (fragTexOffset * (0.0))) * (20.0f/64.0f);
	data_rg += data.rg;
	data_b = min(data_b, data.b * 64.0f / 20.0f);

	data = texture( cubemapSrc, fragTex + (fragTexOffset * (1.0))) * (15.0f/64.0f);
	data_rg += data.rg;
	data_b = min(data_b, data.b * 64.0f / 15.0f);

	data = texture( cubemapSrc, fragTex + (fragTexOffset * (2.0))) * (6.0f/64.0f);
	data_rg += data.rg;
	data_b = min(data_b, data.b * 64.0f / 6.0f);

	data = texture( cubemapSrc, fragTex + (fragTexOffset * (3.0))) * (1.0f/64.0f);
	data_rg += data.rg;
	data_b = min(data_b, data.b * 64.0f / 1.0f);

	FragColor = vec4(data_rg, data_b, 0.0f);
} 


    