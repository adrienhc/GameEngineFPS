#version 330 core

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragTex;
out vec4 FragColor;

//CAMERA
uniform vec3 cameraPos;

//LIGHT OTHER
uniform vec3 lightColor;
uniform vec3 lightPos;

//MATERIAL
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;	//int btw 1, 256 (smaller = more scatter)

	bool has_texture;
	vec4 color;
};
uniform Material material;
uniform sampler2D imgTexture; //to pass in the 2D texture object to the fs

//LIGHTNING
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

//SHADOWS
uniform samplerCube depthMap0;
uniform samplerCube depthMap1;
uniform samplerCube depthMap2;
uniform float far_plane;

//BULLET HOLES
#define MAX_BULLET_HOLES 10
uniform float radiusImpact;
uniform vec3 bulletHoles[MAX_BULLET_HOLES];

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

	float closestDepth;
	vec3 fragToLight = fragPos - lightPos;

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
	
	
	//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);  
	//FragColor = vec4(closestDepth, 1.0f - closestDepth, 0.0f, 1.0f); //close is green, red is far 

	
}

vec3 CalcPointLight(PointLight pointLight, int i, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	//ambient
	vec3 ambient = pointLight.ambient * material.ambient;

	//diffuse
	vec3 lightDir = normalize(pointLight.position - fragPos);
	float diff = max(dot(norm, lightDir) , 0.0);
	vec3 diffuse = pointLight.diffuse * (material.diffuse * diff);


	//specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pointLight.specular * (material.specular * spec);

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
	for(int i = 0; i < MAX_BULLET_HOLES; i++)
	{
		if(length(bulletHoles[i] - fragPos) < radiusImpact)
		{
			FragColor = vec4(vec3(0.0f), 1.0f);
			return;
		}
	}

	vec3 norm = normalize(fragNorm);
	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < numLights; i++)
		result += CalcPointLight(pointLight[i], i, norm, fragPos, viewDir);
	
	if(material.has_texture)
	{
		result = result * texture(imgTexture, fragTex).xyz;
		FragColor = vec4(result, 1.0f);
	}
    else
    { 
    	result = result * material.color.xyz;
    	FragColor = vec4(result, 1.0f);
    	//FragColor = vec4(1.0f);
    }

    
    
} 



























/*

NVIDIA TUTORIAL

 #define SAMPLES_COUNT 64  
 #define SAMPLES_COUNT_DIV_2 32  
 #define INV_SAMPLES_COUNT (1.0f / SAMPLES_COUNT)    
 uniform sampler2D decal;   // decal texture  
 uniform sampler3D jitter;  // jitter map    
 uniform sampler2D spot;    // projected spotlight image  
 uniform sampler2DShadow shadowMap;  // shadow map    
 uniform float fwidth;  
 uniform vec2 jxyscale;      // these are passed down from vertex shader  
 varying vec4 shadowMapPos;  
 varying vec3 normal;  
 varying vec2 texCoord;  
 varying vec3 lightVec;  
 varying vec3 view;    

 void main(void)  
 {    
 	float shadow = 0;    
 	float fsize = shadowMapPos.w * fwidth;    
 	vec3 jcoord = vec3(gl_FragCoord.xy * jxyscale, 0);    
 	vec4 smCoord = shadowMapPos;      // take cheap "test" samples first      
	 for (int i = 0; i<4; i++) 
	 {      
		 vec4 offset = texture3D(jitter, jcoord);      
		 jcoord.z += 1.0f / SAMPLES_COUNT_DIV_2;        
		 smCoord.xy = offset.xy * fsize + shadowMapPos;      
		 shadow += texture2DProj(shadowMap, smCoord) / 8;        
		 smCoord.xy = offset.zw * fsize + shadowMapPos;      
		 shadow += texture2DProj(shadowMap, smCoord) / 8;    
	 }    

	 vec3 N = normalize(normal);  
	 vec3 L = normalize(lightVec);  
	 vec3 V = normalize(view);    
	 vec3 R = reflect(-V, N);      // calculate diffuse dot product  
	 float NdotL = max(dot(N, L), 0);    // if all the test samples are either zeroes or ones, or diffuse dot  
	 // product is zero, we skip expensive shadow-map filtering  
	 if ((shadow - 1) * shadow * NdotL != 0) 
	 {    
		 // most likely, we are in the penumbra      
		 shadow *= 1.0f / 8; // adjust running total      
		 // refine our shadow estimate    
		 for (int i = 0; i<SAMPLES_COUNT_DIV_2 - 4; i++) 
		 {      
		 vec4 offset = texture3D(jitter, jcoord);      
		 jcoord.z += 1.0f / SAMPLES_COUNT_DIV_2;        
		 smCoord.xy = offset.xy * fsize + shadowMapPos;      
		 shadow += texture2DProj(shadowMap, smCoord)* INV_SAMPLES_COUNT;        
		 smCoord.xy = offset.zw * fsize + shadowMapPos;      
		 shadow += texture2DProj(shadowMap, smCoord)* INV_SAMPLES_COUNT;    
		 }  
	 }    
	 // all done Ð modulate lighting with the computed shadow value  
	 vec3 color = texture2D(decal, texCoord).xyz;    
	 gl_FragColor.xyz = (color * NdotL + pow(max(dot(R, L), 0), 64)) * shadow * texture2DProj(spot, shadowMapPos) + color * 0.1;  
 }

 */