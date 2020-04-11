#include "light.h"

const unsigned int PointLight::MAX_LIGHTS = 3;
const unsigned int PointLight::SHADOW_WIDTH = 1024;
const unsigned int PointLight::SHADOW_HEIGHT = 1024;
unsigned int PointLight::tempBlurCubemap = 0;
unsigned int PointLight::tempDepthCubemap = 0;
unsigned int PointLight::tempBlurFBO = 0; 

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
	far = 20.0f;
	shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far); 

	//Cubemap Face Projection Matrices
	cubemapProj = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 2.0f);
	cubemapFaceProjection.push_back(cubemapProj * glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	cubemapFaceProjection.push_back(cubemapProj * glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	cubemapFaceProjection.push_back(cubemapProj * glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	cubemapFaceProjection.push_back(cubemapProj * glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	cubemapFaceProjection.push_back(cubemapProj * glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	cubemapFaceProjection.push_back(cubemapProj * glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	
	//Temporary Cubemaps and FBO for blurring vbsm cubemap
	glGenTextures(1, &tempBlurCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tempBlurCubemap);
	for(GLuint i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG32F, SHADOW_WIDTH, SHADOW_HEIGHT,
							 0, GL_RGBA, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	
	glGenTextures(1, &tempDepthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tempDepthCubemap);
	for(GLuint i = 0; i < 6; i++)
	{
		//SHADOW MAPPING PCF AND VARIANCE SHADOW MAPPING
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
					 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);	
	
	glGenFramebuffers(1, &tempBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, tempBlurFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tempBlurCubemap, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tempDepthCubemap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	GLenum result_temp = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (GL_FRAMEBUFFER_COMPLETE != result_temp) {
			printf("ERROR: Temporary Framebuffer is not complete.\n");
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Create Shadow Maps CubeMaps and associated FBO
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		shadowFBO.push_back(0);
		shadowCubemap.push_back(0);
		vsmCubemap.push_back(0);
		blurVsmStatus.push_back(true);

		//generate depth cube map and create a texture on each of its faces 
		glGenTextures(1, &shadowCubemap[i]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemap[i]);
		for(GLuint i = 0; i < 6; i++)
		{
			//SHADOW MAPPING PCF AND VARIANCE SHADOW MAPPING
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
						 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);	


		glGenTextures(1, &vsmCubemap[i]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, vsmCubemap[i]);
		for(GLuint i = 0; i < 6; i++)
		{
			//VARIANCE SHADOW MAPPING ONLY
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG32F, SHADOW_WIDTH, SHADOW_HEIGHT,
						 0, GL_RGBA, GL_FLOAT, NULL);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);	
		
		//SHADOW MAPPING PCF AND VARIANCE SHADOW MAPPING
		glGenFramebuffers(1, &shadowFBO[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, vsmCubemap[i], 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubemap[i], 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (GL_FRAMEBUFFER_COMPLETE != result) {
				printf("ERROR: Framebuffer is not complete.\n");
			}
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

void PointLight::shadowPassSetup(Shader* depthShader, int room_index)
{
	//bind and clear depth map
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[room_index]);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	blurVsmStatus[room_index] = true; //going to overwrite vsm cubemap, so need to blur it again before binding

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

void PointLight::shadowPassBegin()
{
	//change viewport to fit shadow map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glDisable(GL_BLEND);
}

void PointLight::shadowPassEnd()
{
	glEnable(GL_BLEND);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointLight::bindShadowMap(Shader* shader, int index) //for SIMPLE SHADER
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


void PointLight::bindShadowMapBatch(Shader* shader, int room_index, int shader_index) //for BATCH SHADER
{
	shader->use();
	
	if(shader_index == 0) //only need set it once 
		shader->setFloat("far_plane", far);
	
	shader->setInt("depthMaps[" + std::to_string(shader_index) + "]", RENDERER_TEXTURE_OFFSET + shader_index);

	glActiveTexture(GL_TEXTURE0 + RENDERER_TEXTURE_OFFSET + shader_index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, vsmCubemap[room_index]); //tempBlurCubemap);//  

}

void PointLight::blurVsmCubemap(Shader* blurCubemapShader, int room_index)
{
	if(!blurVsmStatus[room_index]) //if already blurred, do not do it again
		return;

	//change viewport to fit cubemap face
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST); //?????????

	blurCubemapShader->use();
	for(int i = 0; i < cubemapFaceProjection.size(); i++)
	{
		std::string index = std::to_string(i);
		blurCubemapShader->setMat4("projectionMatrices[" + index + "]", cubemapFaceProjection[i]);
	}

	blurCubemapShader->setInt("cubemapSrc", 0);
	glActiveTexture(GL_TEXTURE0);

	// VSM TO TEMP
	glBindTexture(GL_TEXTURE_CUBE_MAP, vsmCubemap[room_index]); //SOURCE VSM 
	glBindFramebuffer(GL_FRAMEBUFFER, tempBlurFBO); //DEST TEMP
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//BLUR IN X DIRECTION 
	blurCubemapShader->setVec2("blurScale", glm::vec2(1.0f/SHADOW_WIDTH, 0.0f));
	cubeContainer.Render();

	// TEMP TO VSM
	glBindTexture(GL_TEXTURE_CUBE_MAP, tempBlurCubemap); //SOURCE TEMP
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[room_index]); //DEST VSM
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	
	//BLUR IN Y DIRECTION -- TEMP TO VSM
	blurCubemapShader->setVec2("blurScale", glm::vec2(0.0f, 1.0f/SHADOW_HEIGHT));
	cubeContainer.Render();

	glEnable(GL_BLEND);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// glEnable(GL_DEPTH_TEST); //???????????
}