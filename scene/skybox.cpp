#include "skybox.h"

#include "../lib/stb_image.h"

Skybox::Skybox(std::string right, std::string left, std::string top, std::string bottom, std::string back, std::string front)
{

	//Create Cube Geometry
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);  //BIND VAO FIRST BEFORE VBO

	glGenBuffers(1, &VBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send data to GPU

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0); //setup Vertex Attributes 0 - POS
	glEnableVertexAttribArray(0);

	//Elements Buffer Object (Indices)
	glGenBuffers(1, &EBO); //create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sent Data to GPU

	//unbind VBO and VAO (don't do unless absolutely necessary here just as an example)
	glBindVertexArray(0); //UNBIND VAO FIRST
	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO


	//Create and Load Cube Map
	std::string images[6] = {right, left, top, bottom, back, front};

	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	//stbi_set_flip_vertically_on_load(false);
	int width, height, nrChannels;

	for(GLuint i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(images[i].c_str(), &width, &height, &nrChannels, 0);
		if(data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Skybox texture failed to load at path: " << images[i] << std::endl;
            stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Skybox::~Skybox()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &cubemap);

	glBindVertexArray(VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
}

void Skybox::Draw()
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);  
}