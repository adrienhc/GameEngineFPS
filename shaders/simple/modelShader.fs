#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 TexCoords;


uniform vec3 cameraPos;


struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
};

uniform Material material;

uniform vec3 lightColor;
uniform vec3 lightPos;

struct PointLight
{
	vec3 position;	

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define NUM_POINT_LIGHTS 3
uniform int numLights;
uniform PointLight pointLight[NUM_POINT_LIGHTS];

uniform samplerCube depthMap0;
uniform samplerCube depthMap1;
uniform samplerCube depthMap2;
uniform float far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   


float ShadowCalculation(vec3 fragPos, vec3 lightPos, int i)
{

	float shadow = 0.0;
	float bias = 0.1f;
	int testSamples = 5;
	int samples = 20;
	float viewDistance = length(cameraPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 100.0; //how far away to sample from 

	vec3 fragToLight = fragPos - lightPos;
	float closestDepth;

	//sample around point of interest, average it all 
	for(int k = 0; k < samples; k++)
	{
		if(i == 0) 
			closestDepth = texture(depthMap0, fragToLight + sampleOffsetDirections[k] * diskRadius).r;
		else if(i == 1) 
			closestDepth = texture(depthMap1, fragToLight + sampleOffsetDirections[k] * diskRadius).r;
		else if(i == 2) 
			closestDepth = texture(depthMap2, fragToLight + sampleOffsetDirections[k] * diskRadius).r;
	
		closestDepth *= far_plane;
		float currentDepth = length(fragToLight);
		
		if (currentDepth - bias > closestDepth)
			shadow += 1.0;

		if(k == testSamples && (shadow == 0.0 || shadow == float(testSamples) + 1.0f))
			return shadow / (float(testSamples) + 1.0f);
	}
	return shadow / float(samples);
}

vec3 CalcPointLight(PointLight pointLight, int i, vec3 fragPos, vec3 norm, vec2 texCoords, vec3 viewDir)
{
	//ambient
	vec3 ambient = pointLight.ambient * vec3(texture(material.texture_diffuse1, texCoords).rgb);

	//diffuse
	vec3 lightDir = normalize(pointLight.position - fragPos);
	float diff = max(dot(norm, lightDir) , 0.0);
	vec3 diffuse = pointLight.diffuse * vec3(texture(material.texture_diffuse1, texCoords).rgb) * diff;


	//specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  //Default MATERIAL Shininess of 32	
	vec3 specular = pointLight.specular * vec3(texture(material.texture_specular1, texCoords).r) * spec;

	//attenuation
	float distance = length(pointLight.position - fragPos);
	float attenuation = 1.0 / ( pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance) );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float shadow = ShadowCalculation(fragPos, pointLight.position, i);

	return (ambient + (1.0f - shadow)*(diffuse + specular));
}


void main()
{   
	
	//USE NORMAL MAP
	//vec3 Norm = vec3(texture(material.texture_normal1, TexCoords)); //NEED TANGENT SPACE!

	vec3 Norm = normalize(FragNorm);
	vec3 viewDir = normalize(cameraPos - FragPos);

	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < numLights; i++)
		result += CalcPointLight(pointLight[i], i, FragPos, Norm, TexCoords, viewDir);

	FragColor = vec4(result, 1.0f);
   
}