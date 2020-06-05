#ifndef LIGHT
#define LIGHT

#include "../pch.h"

#include "../geometry/geometry.h" //FOR CUBEMAP BLURRING
#include "shader.h"
#include "../config.h"

// #define RENDERER_TEXTURE_OFFSET 20

// extern const unsigned int WINDOW_WIDTH; //defined in config.h 
// extern const unsigned int WINDOW_HEIGHT; //used to reset viewport after shadow pass

class Shader; //since shader also includes PointLight

class PointLight
{
public:
	PointLight(int num_lights, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	~PointLight();
	void setTransform(glm::mat4 transform, int room_index = 0);
	void setRadius(float radius, int room_index = 0);

	static const unsigned int MAX_LIGHTS; 
	
	//Light Properties
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;	
	std::vector<glm::vec3> position;
	std::vector<float> radius;
	
	//Attenuation
	float constant;
	float linear;
	float quadratic;

	static void shadowPassBegin();
	static void shadowPassEnd();
	void shadowPassSetup(Shader* depthShader, int room_index);
	void bindShadowMap(Shader* shader, int index);
	void bindShadowMapBatch(Shader* shader, int room_index, int shader_index);

	void blurVsmCubemap(Shader* blurCubemapShader, int room_index);

private: 
	int NUM_LIGHTS;
	static const unsigned int SHADOW_WIDTH;
	static const unsigned int SHADOW_HEIGHT;
	std::vector<unsigned int> shadowFBO; //[NUM_LIGHTS];
	std::vector<unsigned int> shadowCubemap; //SHADOW MAP FOR PCF
	std::vector<unsigned int> vsmCubemap; //SHADOW MAP FOR VSM
	std::vector<bool> blurVsmStatus;
	
	static unsigned int tempBlurCubemap;
	static unsigned int tempDepthCubemap;
	static unsigned int tempBlurFBO;
	glm::mat4 cubemapProj;
	std::vector<glm::mat4> cubemapFaceProjection;
	cCube cubeContainer;
	
	float aspect;
	float near;
	float far;
	glm::mat4 shadowProj;
	std::vector<std::vector<glm::mat4>> shadowTransform;

};

#endif //LIGHT