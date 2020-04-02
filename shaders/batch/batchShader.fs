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

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define NUM_POINT_LIGHTS 9
uniform PointLight pointLight[NUM_POINT_LIGHTS];
uniform int numLights;

//SHADOWS
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
	float bias = 0.05f;
	int testSamples = 5;
	int samples = 10;
	float viewDistance = length(cameraPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 100.0; //how far away to sample from

	float closestDepth;
	vec3 fragToLight = fragPos - lightPos;

	
	//DEBUG
	//closestDepth = texture(depthMap0, fragToLight).r;
	//FragColor = vec4(closestDepth, 1.0f - closestDepth, 0.0f, 1.0f); //close is green, red is far 
	//return 0.0f;

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
			shadow += 1.0f;

		if(k == testSamples && (shadow == 0.0f || shadow == float(testSamples) + 1.0f))
			return shadow / (float(testSamples) + 1.0f);
	}
	
	//DEBUG
	//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);  
	//FragColor = vec4(closestDepth, 1.0f - closestDepth, 0.0f, 1.0f); //close is green, red is far 

	return shadow / float(samples);
}



vec3 CalcPointLight(PointLight pointLight, int i, vec3 fragAmb, vec3 fragDiff, vec3 fragSpec, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	//ambient
	vec3 ambient = pointLight.ambient * fragAmb;

	//diffuse
	vec3 lightDir = normalize(pointLight.position - fragPos);
	float diff = max(dot(norm, lightDir) , 0.0);
	vec3 diffuse = pointLight.diffuse * (fragDiff * diff);


	//specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragLighting.w);
	vec3 specular = pointLight.specular * (fragSpec * spec);

	//attenuation
	float distance = length(pointLight.position - fragPos);
	float attenuation = 1.0 / ( pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance) );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float cutoffDistance = 15; //Distance at which Light has no effect and assume shadow? 
	float shadow = 1.0f;

	if(abs(pointLight.position.z - fragPos.z) < cutoffDistance && abs(pointLight.position.x - fragPos.x) < cutoffDistance)
		shadow = ShadowCalculation(fragPos, pointLight.position, i);
	
	return (ambient + (1.0f - shadow)*(diffuse + specular));
}

void main()
{
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
	for(int i = 0; i < numLights; i++)
	{
	 	result += CalcPointLight(pointLight[i], i, fragAmb, fragDiff, fragSpec, norm, fragPos.xyz, viewDir);
	}

	FragColor = vec4(result, 1.0f) * fragCol;
	
}