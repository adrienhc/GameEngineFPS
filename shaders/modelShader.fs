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
uniform PointLight pointLight[NUM_POINT_LIGHTS];




vec3 CalcPointLight(PointLight pointLight, vec3 fragPos, vec3 norm, vec2 texCoords, vec3 viewDir)
{
	//ambient
	vec3 ambient = pointLight.ambient * vec3(texture(material.texture_diffuse1, texCoords).rgb);

	//diffuse
	vec3 lightDir = normalize(pointLight.position - fragPos);
	float diff = max(dot(norm, lightDir) , 0.0);
	vec3 diffuse = pointLight.diffuse * vec3(texture(material.texture_diffuse1, texCoords).rgb) * diff;


	//specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);  //Default MATERIAL Shininess of 32	
	vec3 specular = pointLight.specular * vec3(texture(material.texture_specular1, texCoords).r) * spec;

	//attenuation
	float distance = length(pointLight.position - fragPos);
	float attenuation = 1.0 / ( pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance) );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}




void main()
{   
	
	//USE NORMAL MAP
	//vec3 Norm = vec3(texture(material.texture_normal1, TexCoords)); //NEED TANGENT SPACE!

	vec3 Norm = normalize(FragNorm);
	vec3 viewDir = normalize(cameraPos - FragPos);

	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLight[i], FragPos, Norm, TexCoords, viewDir);

	FragColor = vec4(result, 1.0f);
   
}