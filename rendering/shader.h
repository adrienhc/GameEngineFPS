#ifndef SHADER_H
#define SHADER_H

#include "../pch.h"
#include "../config.h"
#include "camera.h"
#include "light.h"
#include "../material_system/material.h"

// #define TEXTURE_SLOTS 32

class PointLight; //as PointLight also includes Shader

class Shader 
{
public:
	unsigned int ID;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setIntArray(const std::string &name, int* values, int count) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setMat3(const std::string &name, glm::mat3 value) const;
	void setMat4(const std::string &name, glm::mat4 value) const;

	void setCamera(Camera* camera);
	void setMaterial(Material* material); //WHAT DO I NEED TO SET MATERIAL/TEXTURE
	void setLightInfo(int numLights);
	void setPointLight(PointLight* light, int index, int room_index = 0, bool active_shadow = true);

	void setTransform(glm::mat4 transform);

};

#endif 