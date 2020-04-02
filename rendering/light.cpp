#include "light.h"

int PointLight::MAX_LIGHTS = 3;

PointLight::PointLight(int num_lights, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float cst, float lin, float quad)
{
	NUM_LIGHTS = num_lights;

	ambient = amb;
	diffuse = diff;
	specular = spec;
	position = glm::vec3(0.0f, 0.0f, 0.0f);

	constant = cst;
	linear = lin;
	quadratic = quad;

	//create shadow pass view matrix
	aspect = (float) SHADOW_WIDTH/ (float) SHADOW_HEIGHT;
	near = 1.0f;
	far = 30.0f;
	shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far); 
	
	
	
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		shadowFBO.push_back(0);
		shadowCubemap.push_back(0);

		//generate depth cube map and create a texture on each of its faces 
		glGenTextures(1, &shadowCubemap[i]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemap[i]);
		for(GLuint i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
						 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);	
	
		//initialize Light's FrameBuffer, only depth component, no color nor stencil
		glGenFramebuffers(1, &shadowFBO[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubemap[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
}


void PointLight::setTransform(glm::mat4 trsf)
{
	glm::vec4 pos = trsf * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);  //Apply current scene graph transform
	position =  glm::vec3(pos.x, pos.y, pos.z);

	//Build shadowTransform Matrixes vector -- look along all axes
	shadowTransform.clear(); //remove all transforms for previous frame's position
	shadowTransform.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransform.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransform.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransform.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransform.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransform.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}	

void PointLight::shadowPassSetup(Shader* depthShader, int index)
{
	//change viewport to fit shadow map, bind and clear depth map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[index]);
	glClear(GL_DEPTH_BUFFER_BIT);

	//setup depthShader's values
	depthShader->use();
	for(int i = 0; i < shadowTransform.size(); i++)
	{
		std::string index = std::to_string(i);
		depthShader->setMat4("shadowMatrices[" + index + "]", shadowTransform[i]);
	}
	depthShader->setVec3("lightPosition", position);
	depthShader->setFloat("far_plane", far);
}

void PointLight::bindShadowMap(Shader* shader, int index)
{
	shader->use();
	
	if(index == 0) //only need set it once 
		shader->setFloat("far_plane", far);

	int depthMapIndex = index;
	int offset = 5; //so that 2d and cube map indexes do not overlap! -- 

	if(NUM_LIGHTS <= depthMapIndex && depthMapIndex < MAX_LIGHTS)
		index = 0;
	
	std::string dmi = std::to_string(depthMapIndex);
	//std::cout << "depthMap" + dmi << " " + std::to_string(offset + index) + " " << "Max Lights = " << NUM_LIGHTS << std::endl;
	shader->setInt("depthMap" + dmi, offset + index);

	if(depthMapIndex < NUM_LIGHTS)
	{
		glActiveTexture(GL_TEXTURE0 + offset + index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemap[index]);
	}
}


void PointLight::bindShadowMapBatch(Shader* shader, int room_index, int shader_index)
{
	shader->use();
	
	if(shader_index == 0) //only need set it once 
		shader->setFloat("far_plane", far);
	
	shader->setInt("depthMap" + std::to_string(shader_index), RENDERER_TEXTURE_OFFSET + shader_index);

	glActiveTexture(GL_TEXTURE0 + RENDERER_TEXTURE_OFFSET + shader_index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemap[room_index]);

}