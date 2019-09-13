#version 330 core

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragTex;
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D imgTexture; //to pass in the 2D texture object to the fs
uniform vec3 cameraPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;	//int btw 1, 256 (smaller = more scatter)

	bool has_texture;
	vec3 color;
};

uniform Material material;

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




vec3 CalcPointLight(PointLight pointLight, vec3 norm, vec3 fragPos, vec3 viewDir)
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

	return (ambient + diffuse + specular);
}

void main()
{

	vec3 norm = normalize(fragNorm);
	vec3 viewDir = normalize(cameraPos - fragPos);


	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLight[i], norm, fragPos, viewDir);

	if(material.has_texture)
	{
		result = result * texture(imgTexture, fragTex).xyz;
	}
    else
    { 
    	result = result * material.color;
    }

    FragColor = vec4(result, 1.0f);

} 