#include <glad/glad.h> //to get OPENGL functions
#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 1 -- retrieve shaders code from pathes
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//ensure ifstream objects can throw exceptions
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//open files 
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		//read file buffer contents into string streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//close file handlers
		vShaderFile.close();
		fShaderFile.close();

		//convert streams into C++ strings
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str(); 
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	//convert C++ string objects into C style strings -- As OpenGL is a C library
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	// 2 -- compile Shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "VERTEX SHADER COMPILATION ERROR: \n" << infoLog << std::endl; 
	}


	//fragmentShader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "FRAGMENT SHADER COMPILATION ERROR: \n" << infoLog << std::endl;
	}

	// 3 -- link Shaders into Program
	ID = glCreateProgram(); //ID is in Shader object
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "SHADER PROGRAM LINKING ERROR: \n" << infoLog << std::endl; 
	}

	// 4 -- cleanup
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


void Shader::use()
{
	glUseProgram(ID);
}


void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); //C++ bool is an int in C
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{																  //transpose matrix?
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string &name, glm::mat3 value) const
{																  //transpose matrix?
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::setCamera(Camera* camera, float ratio)
{
	use();
	setVec3("cameraPos", camera->Position);
	glm::mat4 view = camera->GetViewMatrix();  
    setMat4("viewing", view);
    glm::mat4 proj = glm::perspective(glm::radians(camera->Zoom), ratio, 0.1f, 100.0f);
    setMat4("projection", proj);
}

void Shader::setTransform(glm::mat4 transform)
{
	use();
	setMat4("model_transform", transform);
    glm::mat3 norm_transform = glm::mat3(glm::transpose(glm::inverse(transform)));
    setMat3("normal_transform", norm_transform);

}

void Shader::setPointLight(PointLight* light, int index) //of object in shader! (not class itself)
{
	use();
	std::string str_index = std::to_string(index);
	//set Light Properties
	setVec3("pointLight[" + str_index + "].position", light->position);
    setVec3("pointLight[" + str_index + "].ambient", light->ambient);
    setVec3("pointLight[" + str_index + "].diffuse", light->diffuse);
    setVec3("pointLight[" + str_index + "].specular", light->specular);
    //set Attenuation
    setFloat("pointLight[" + str_index + "].constant", light->constant);
    setFloat("pointLight[" + str_index + "].linear", light->linear);
    setFloat("pointLight[" + str_index + "].quadratic", light->quadratic);
}

void Shader::setMaterial(Material* material)
{
	use();
    setVec3("material.ambient", material->ambient);
    setVec3("material.diffuse", material->diffuse);
    setVec3("material.specular", material->specular);
    setFloat("material.shininess", material->shininess);
    setBool("material.has_texture", material->has_texture);
    if(material->has_texture)
    {
    	setInt("imgTexture", 0); 
    	material->getTexture()->Bind();
    }
    else
    	setVec3("material.color", material->color);
}