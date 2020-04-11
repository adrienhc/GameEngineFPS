#version 450 core

in vec3 fragTex;
in vec3 fragTexOffset; //INCLUDES BLUR SCALE
out vec4 FragColor;

uniform samplerCube cubemapSrc; 


float normpdf(float x, float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

void main()
{	
	float Sigma_Intensity = 0.2f;
	float Norm_Weight = 1.0/normpdf(0.0, Sigma_Intensity);

	vec4 center_kernel = texture(cubemapSrc, fragTex);
	vec4 value = vec4(0.0f);
	float factor = 0.0f;
	vec4 data = vec4(0.0f);

	value = texture( cubemapSrc, fragTex + (fragTexOffset * (-3.0)));
	factor = (1.0f/64.0f) * normpdf(abs(center_kernel.x - value.x), Sigma_Intensity);
	data += value * factor;

	value = texture( cubemapSrc, fragTex + (fragTexOffset * (-2.0)));
	factor = (6.0f/64.0f) * normpdf(abs(center_kernel.x - value.x), Sigma_Intensity);
	data += value * factor;
	
	value = texture( cubemapSrc, fragTex + (fragTexOffset * (-1.0)));
	factor = (15.0f/64.0f) * normpdf(abs(center_kernel.x - value.x), Sigma_Intensity);
	data += value * factor;
	
	value = texture( cubemapSrc, fragTex + (fragTexOffset * (0.0)));
	factor = (20.0f/64.0f) * normpdf(abs(center_kernel.x - value.x), Sigma_Intensity);
	data += value * factor;
	
	value = texture( cubemapSrc, fragTex + (fragTexOffset * (1.0))); 
	factor = (15.0f/64.0f) * normpdf(abs(center_kernel.x - value.x), Sigma_Intensity);
	data += value * factor;
	
	value = texture( cubemapSrc, fragTex + (fragTexOffset * (2.0))); 
	factor = (6.0f/64.0f) * normpdf(abs(center_kernel.x - value.x), Sigma_Intensity);
	data += value * factor;
	
	value = texture( cubemapSrc, fragTex + (fragTexOffset * (3.0)));
	factor = (1.0f/64.0f) * normpdf(abs(center_kernel.x - value.x), Sigma_Intensity);
	data += value * factor;
	
	FragColor = Norm_Weight * data;
} 


    