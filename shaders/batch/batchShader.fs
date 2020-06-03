#version 450 core 

//VERTEX SHADER DATA
in vec4 fragPos;
in vec3 fragNorm;
in vec2 fragTex;
flat in float fragTexDiffID;
flat in float fragTexSpecID;
flat in vec4 fragLighting;
in vec4 fragColor;

//FRAGMENT SHADER OUTPUT
out vec4 FragColor;

//TEXTURES
#define TEXTURE_SLOTS 20
uniform sampler2D textures[TEXTURE_SLOTS];

//CAMERA
uniform vec3 cameraPos;

//LIGHT
struct PointLight
{
	vec3 position;	
	float radius;
	int active_shadow;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};


#define NUM_POINT_LIGHTS 5
uniform int numLights;
uniform PointLight pointLight[NUM_POINT_LIGHTS];

//BULLET HOLES
#define MAX_BULLET_HOLES 5
uniform float radiusImpact;
uniform vec3 bulletHoles[MAX_BULLET_HOLES];

//SHADOWS
uniform samplerCube depthMaps[NUM_POINT_LIGHTS];
uniform float far_plane;

//DEBUG MODE VISUALIZE SHADOW MAP
#define DEBUG 0

float linstep(float x0, float x1, float xn)
{
	return (xn - x0) / (x1 - x0);
}

float DEBUG_ShadowCalculation(vec3 fragPos, vec3 lightPos, int i)
{
	vec3 fragToLight = fragPos - lightPos;

	float closestDepth = texture(depthMaps[i], fragToLight).x;
	FragColor = vec4(closestDepth, 1.0f - closestDepth, 0.0f, 1.0f); //close is green, red is far 

	return 0.0f;
}

float SIMPLE_ShadowCalculation(vec3 fragPos, vec3 lightPos, int i)
{
	float bias = 0.05f;
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(depthMaps[i], fragToLight).x;
	closestDepth *= far_plane;
	float currentDepth = length(fragToLight);
	
	if (currentDepth - bias > closestDepth)
		return 0.0f;
	else
		return 1.0f;
}

float VSM_ShadowCalculation(vec3 fragPos, vec3 lightPos, int i)
{
	vec3 moments;
	vec3 fragToLight = fragPos - lightPos;

	moments = texture(depthMaps[i], fragToLight).xyz;

	float currentDepth = length(fragToLight) / far_plane;

	float p = currentDepth <= moments.x ? 1.0f : 0.0f; 
	float variance = max(moments.y - moments.x * moments.x, 0.00002); 
	//variance = min(0.00005f, variance); //since Chebyshev has problem with high variance 

	float mean_dist = currentDepth - moments.x;
	float pMax = variance / (variance + mean_dist * mean_dist);
	
	float low = 0.2f;
	float high = 1.0f;
	pMax = clamp((pMax-low)/(high-low), 0.0f, 1.0f);
	return min(max(p, pMax), 1.0f);
}


vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

float PCF_ShadowCalculation(vec3 fragPos, vec3 lightPos, int i)
{
	float shadow = 0.0;
	float bias = 0.05f;
	int testSamples = 5;
	int samples = 10;
	float viewDistance = length(cameraPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 100.0; //how far away to sample from

	float closestDepth;
	vec3 fragToLight = fragPos - lightPos;
	
	//sample around point of interest, average it all 
	for(int k = 0; k < samples; k++)
	{	
		closestDepth = texture(depthMaps[i], fragToLight + sampleOffsetDirections[k] * diskRadius).r;
		closestDepth *= far_plane;
		float currentDepth = length(fragToLight);
		
		if (currentDepth - bias > closestDepth)
			shadow += 1.0f;

		if(k == testSamples && (shadow == 0.0f || shadow == float(testSamples) + 1.0f))
			return (1.0f - shadow / (float(testSamples) + 1.0f));
	}

	return (1.0f - shadow / float(samples));
	
}



vec3 CalcPointLight(PointLight pointLight, int i, vec3 fragAmb, vec3 fragDiff, vec3 fragSpec, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	float distance = length(pointLight.position - fragPos);
	float factor = (distance/pointLight.radius);
	if( factor > 1.0f)
		return vec3(0.0f);

	factor = factor * factor * factor * factor;
	factor = 1.0f - factor;

	//ambient
	vec3 ambient = pointLight.ambient * fragAmb;

	//diffuse
	vec3 lightDir = normalize(pointLight.position - fragPos);
	float diff = max(dot(norm, lightDir) , 0.0);
	vec3 diffuse = pointLight.diffuse * (fragDiff * diff);


	//PHONG
	//specular
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragLighting.w);

	//BLINN PHONG
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 2.0f* fragLighting.w);
	vec3 specular = pointLight.diffuse * (fragSpec * spec);

	//attenuation
	float attenuation = 1.0 / ( pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance) );

	ambient *= attenuation;
	// ambient /= numLights;
	diffuse *= attenuation;
	specular *= attenuation;

	//float cutoffDistance = 25; //Distance at which Light has no effect and assume shadow? 
	float shadow = 1.0f;
	//if(abs(pointLight.position.z - fragPos.z) < cutoffDistance && abs(pointLight.position.x - fragPos.x) < cutoffDistance)
	//if(length(pointLight.position - fragPos) < cutoffDistance)
	//{	
		//if(DEBUG == 1)
		//	shadow = DEBUG_ShadowCalculation(fragPos, pointLight.position, i);
		//else
	shadow = pointLight.active_shadow * VSM_ShadowCalculation(fragPos, pointLight.position, i);
	//}


	return factor * (ambient + (shadow)*(diffuse + specular));
}

void main()
{

	for(int i = 0; i < MAX_BULLET_HOLES; i++)
	{
		if(length(bulletHoles[i] - fragPos.xyz) < radiusImpact)
		{
			FragColor = vec4(vec3(0.0f), 1.0f);
			return;
		}
	}

	vec3 norm = normalize(fragNorm);
	vec3 viewDir = normalize(cameraPos - fragPos.xyz);
	
	vec3 fragAmb = vec3(fragLighting.x);
	vec3 fragDiff = vec3(fragLighting.y);
	vec3 fragSpec = vec3(fragLighting.z); 
	vec4 fragCol = fragColor;

	if(fragTexDiffID > 0.0)
	{
		int tid = int(fragTexDiffID - 0.5f);
		fragCol = texture(textures[tid], fragTex);

		if(fragLighting.x == 0.0)
			fragAmb = vec3(fragCol.rgb);
		if(fragLighting.y == 0.0)
			fragDiff = fragAmb;
	}

	if(fragTexSpecID > 0.0)
	{
		int tid = int(fragTexSpecID - 0.5f);
		fragSpec = vec3(texture(textures[tid], fragTex).r);
	}

	vec3 result = vec3(0.0f);
	
	//vec3 offset = 0.8f * norm * (1.0f-fragCol.x);
	//float factor = length(fragPos.xyz - cameraPos) / 50.0f;
	//offset *= (1.0f - factor);

	for(int i = 0; i < numLights; i++)
	{	 
																					//fragPos.xyz + offset
	 	result += CalcPointLight(pointLight[i], i, fragAmb, fragDiff, fragSpec, norm, fragPos.xyz, viewDir);
	}

	if(DEBUG == 0)
	{
		
		FragColor = vec4(result, 1.1f) * fragCol;
		float gamma = 1.0f;//2.0f; //Default 2.2
		FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
		//FragColor = vec4(fragTexDiffID/TEXTURE_SLOTS, 0.0f, 0.0f, 1.0f);
	}
	
}